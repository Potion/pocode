/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poURLLoader.h"
#include "poThreadCenter.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

//------------------------------------------------------------------
//------------------------------------------------------------------
//File Loader
#pragma mark poURLLoader

size_t write_data( void *ptr, size_t size, size_t nmeb, FILE *stream) {
    return std::fwrite(ptr,size,nmeb,stream);
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string*)stream)->append((char*)ptr, 0, size*count);
    return size*count;
}


namespace poURLLoader {
    //------------------------------------------------------------------
    poFilePath getFile(poURL url, const poFilePath &savePath) {
        poFilePath p(savePath.asString());
        
        if(!savePath.isSet()) {
            boost::char_separator<char> sep("/");
            boost::tokenizer< boost::char_separator<char> > tokens(url.asString(), sep);
            BOOST_FOREACH(const std::string &t, tokens) {
                p.set(t);
            }
        }
        
        FILE * file = (FILE *)fopen(p.asString().c_str(),"w+");
        if(!file){
            perror("File Open:");
        }
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle,CURLOPT_URL,url.asString().c_str()); /*Using the http protocol*/
        curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(handle,CURLOPT_WRITEDATA, file);
        curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        
        fclose(file);
        return p;
    }

    //------------------------------------------------------------------
    void getFileAsync(poURL url, poObject* notify, const poFilePath &filepath) {
        poThreadCenter::addItem(new poURLLoaderWorker(url, PO_FILE_LOADER_MODE_SAVE, filepath), notify);
    }

    //------------------------------------------------------------------
    std::string getFileAsString(poURL url) {
        std::string response;
        
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle,CURLOPT_URL, url.asString().c_str()); /*Using the http protocol*/
        curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(handle,CURLOPT_WRITEDATA, &response);
        curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        
        return response;
    }

    //------------------------------------------------------------------
    void getFileAsStringAsync(poURL url, poObject* notify) {
        poThreadCenter::addItem(new poURLLoaderWorker(url, PO_FILE_LOADER_MODE_RETURN_AS_STRING), notify);
    }
};



//------------------------------------------------------------------
//------------------------------------------------------------------
//File Loader Worker
#pragma mark poURLLoaderWorker
poURLLoaderWorker::poURLLoaderWorker(poURL url, poURLLoaderMode mode, const poFilePath &savePath)
: url(url)
, mode(mode)
, savePath(savePath)
{};

poURLLoaderWorker::~poURLLoaderWorker() {
}


//------------------------------------------------------------------
void poURLLoaderWorker::workerFunc() {
    switch(mode) {
        case PO_FILE_LOADER_MODE_SAVE: {
            //Save the file
            //if the save path is "" we get back the name of the file, otherwise the savepath is passed around
            savePath = poURLLoader::getFile(url, savePath);
            
            dict.set("mode", mode);
            dict.set("url", url.asString());
            dict.set("savePath", savePath.asString());
            break;
        }
            
        case PO_FILE_LOADER_MODE_RETURN_AS_STRING:
            //Get File as string
            std::string fileContents = poURLLoader::getFileAsString(url);
            
            //Set Dictionary with contents
            dict.set("mode", mode);
            dict.set("url", url.asString());
            dict.set("content", fileContents);
            break;
    }
    
    workerMessage = poURLLoaderCompleteMessage;
}

