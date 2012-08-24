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

#include "poFileLoader.h"
#include "poThreadCenter.h"


#ifdef _WIN32
#include <windows.h>
#endif
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

size_t write_data( void *ptr, size_t size, size_t nmeb, FILE *stream) {
    return std::fwrite(ptr,size,nmeb,stream);
}

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
    ((std::string*)stream)->append((char*)ptr, 0, size*count);
    return size*count;
}


//------------------------------------------------------------------
//File Loader
#pragma mark poFileLoader
namespace poFileLoader {

    //------------------------------------------------------------------
    std::string getFile(std::string url, std::string filename) {
        if(filename == "") {
            boost::char_separator<char> sep("/");
            boost::tokenizer< boost::char_separator<char> > tokens(url, sep);
            BOOST_FOREACH(const std::string &t, tokens) {
                filename = t;
            }
        }
        
        FILE * file = (FILE *)fopen(filename.c_str(),"w+");
        if(!file){
            perror("File Open:");
        }
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle,CURLOPT_URL,url.c_str()); /*Using the http protocol*/
        curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(handle,CURLOPT_WRITEDATA, file);
        curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        
        fclose(file);
        return filename;
    }

    //------------------------------------------------------------------
    void getFileAsync(std::string url, poObject* notify, std::string filename) {
        poThreadCenter::get()->addItem(new poFileLoaderWorker(url, PO_FILE_LOADER_MODE_SAVE, filename), notify);
    }

    //------------------------------------------------------------------
    std::string getFileAsString(std::string url) {
        std::string response;
        
        CURL *handle = curl_easy_init();
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle,CURLOPT_URL, url.c_str()); /*Using the http protocol*/
        curl_easy_setopt(handle,CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(handle,CURLOPT_WRITEDATA, &response);
        curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        
        return response;
    }

    //------------------------------------------------------------------
    void getFileAsStringAsync(std::string url, poObject* notify) {
        poThreadCenter::get()->addItem(new poFileLoaderWorker(url, PO_FILE_LOADER_MODE_RETURN_AS_STRING), notify);
    }
};

//------------------------------------------------------------------
//File Loader Worker
#pragma mark poFileLoaderWorker
poFileLoaderWorker::poFileLoaderWorker(std::string url, poFileLoaderMode mode, std::string filename) {
    this->url       = url;
    this->mode      = mode;
    this->filename  = filename;
}

poFileLoaderWorker::~poFileLoaderWorker() {
}


//------------------------------------------------------------------
void poFileLoaderWorker::workerFunc() {
    switch(mode) {
        case PO_FILE_LOADER_MODE_SAVE: {
            //Save the file
            std::string savedFileName = poFileLoader::getFile(url, filename);
            
            dict.set("mode", mode);
            dict.set("url", url);
            dict.set("filename", savedFileName);
            break;
        }
            
        case PO_FILE_LOADER_MODE_RETURN_AS_STRING:
            //Get File as string
            std::string fileContents = poFileLoader::getFileAsString(url);
            
            //Set Dictionary with contents
            dict.set("mode", mode);
            dict.set("url", url);
            dict.set("content", fileContents);
            break;
    }
    
    workerMessage = PoFileLoaderCompleteMessage;
}

