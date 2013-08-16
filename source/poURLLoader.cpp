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

#ifdef _WIN32
#include <windows.h>
#endif
#include <curl/curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

namespace po {
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: URLLoader =================================
    #pragma mark - URLLoader -

    size_t write_data( void *ptr, size_t size, size_t nmeb, FILE *stream) {
        return std::fwrite(ptr,size,nmeb,stream);
    }

    size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
        ((std::string*)stream)->append((char*)ptr, 0, size*count);
        return size*count;
    }


    namespace URLLoader {
        //------------------------------------------------------------------
        FilePath getFile(URL url, const FilePath &savePath) {
            //Check for blank URLs or SavePaths
            if(!url.toString().length() || !savePath.toString().length()) {
                std::cout << "po::URL::getFile error, url or savePath not set for URL:" << url.toString() << " FilePath: " <<savePath.toString() << std::endl;
                return savePath;
            }
            
            FilePath p(savePath.toString());
            
            if(!savePath.isSet()) {
                boost::char_separator<char> sep("/");
                boost::tokenizer< boost::char_separator<char> > tokens(url.toString(), sep);
                BOOST_FOREACH(const std::string &t, tokens) {
                    p.set(t);
                }
            }

            FILE * file = (FILE *)fopen(p.toString().c_str(),"w+");
            if(!file){
                perror("URLLoader:: File Open:: ");
                return;
            }
            
            
            //Create CURL handle
            CURL *handle = curl_easy_init();
            
            //Set Headers
            struct curl_slist *headers=NULL;
            
            for(int i=0; i<url.getHeaders().size(); i++) {
                headers = curl_slist_append(headers, url.getHeaders()[i].c_str());
            }
            
            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
            
            //Set Options
            curl_easy_setopt(handle,    CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(handle,    CURLOPT_URL,url.toString().c_str()); /*Using the http protocol*/
            curl_easy_setopt(handle,    CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(handle,    CURLOPT_WRITEDATA, file);
            
            if(url.getUsername() != "") {
                curl_easy_setopt(handle, CURLOPT_USERNAME, url.getUsername().c_str());
            }
            
            if(url.getPassword() != "") {
                curl_easy_setopt(handle, CURLOPT_PASSWORD, url.getPassword().c_str());
            }
            
            //Set the Error Buffer
            char* pErrorBuffer = NULL;
            pErrorBuffer = (char*)malloc(512);
            memset(pErrorBuffer, 0, 512);
            curl_easy_setopt(handle,    CURLOPT_ERRORBUFFER, pErrorBuffer );
            
            //Set Timeout
            curl_easy_setopt(handle,    CURLOPT_CONNECTTIMEOUT, url.getTimeout());
            
            //Do Request
            CURLcode status = curl_easy_perform(handle);
            if(status != CURLE_OK) {
                // pErrorBuffer contains error string returned by cURL
                pErrorBuffer[511] = '\0';
                printf( "cURL returned: %s", pErrorBuffer );
            }
            
            //Cleanup
            curl_easy_cleanup(handle);
            curl_slist_free_all(headers);
            fclose(file);
            free(pErrorBuffer);
            
            return p;
        }
        
        
        //------------------------------------------------------------------
        void getFileAsync(URL url, Object* notify, const FilePath &filepath) {
            URLLoaderWorker *worker = new URLLoaderWorker(url, PO_FILE_LOADER_MODE_SAVE, filepath);
            ThreadCenter::addItem(worker, notify);
        }
        
        
        //------------------------------------------------------------------
        std::string getFileAsString(URL url) {
            //Check for blank url
            if(!url.toString().length()) {
                std::cout << "po::URL::getFileAsString error, url not set" << std::endl;
                return "";
            }
            
            std::string response;
            
            //Create CURL handle
            CURL *handle = curl_easy_init();
            
            //Set Headers
            struct curl_slist *headers=NULL;
            for(int i=0; i<url.getHeaders().size(); i++) {
                headers = curl_slist_append(headers, url.getHeaders()[i].c_str());
            }
            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
            
            //Set Options
            curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(handle, CURLOPT_URL, url.toString().c_str()); /*Using the http protocol*/
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_to_string);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response);
            
            if(url.getUsername() != "") {
                curl_easy_setopt(handle, CURLOPT_USERNAME, url.getUsername().c_str());
            }
            
            if(url.getPassword() != "") {
                curl_easy_setopt(handle, CURLOPT_PASSWORD, url.getPassword().c_str());
            }
            
            //Set the Error Buffer
            char* pErrorBuffer = NULL;
            pErrorBuffer = (char*)malloc(512);
            memset(pErrorBuffer, 0, 512);
            curl_easy_setopt(handle,    CURLOPT_ERRORBUFFER, pErrorBuffer );
            
            //Set Timeout
            curl_easy_setopt(handle,    CURLOPT_CONNECTTIMEOUT, url.getTimeout());
            
            //Do Request
            CURLcode status = curl_easy_perform(handle);
            if(status != CURLE_OK) {
                // pErrorBuffer contains error string returned by cURL
                pErrorBuffer[511] = '\0';
                printf( "cURL returned: %s", pErrorBuffer );
            }
            
            //Cleanup
            curl_easy_cleanup(handle);
            curl_slist_free_all(headers);
            free(pErrorBuffer);
            return response;
        }
        
        
        //------------------------------------------------------------------
        void getFileAsStringAsync(URL url, Object* notify) {
            ThreadCenter::addItem(new URLLoaderWorker(url, PO_FILE_LOADER_MODE_RETURN_AS_STRING), notify);
        }
    };
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: URLLoader Worker ==========================
    #pragma mark - URLLoader Worker -

    URLLoaderWorker::URLLoaderWorker(URL url, URLLoaderMode mode, const FilePath &savePath)
    : url(url)
    , mode(mode)
    , filePath(savePath)
    {};

    URLLoaderWorker::~URLLoaderWorker() {
    }
    
    
    //------------------------------------------------------------------
    void URLLoaderWorker::workerFunc() {
        switch(mode) {
            case PO_FILE_LOADER_MODE_SAVE: {
                //Save the file
                //if the save path is "" we get back the name of the file, otherwise the savepath is passed around
                filePath = URLLoader::getFile(url, filePath);
                
                dict.set("mode", mode);
                dict.set("url", url.toString());
                dict.set("filePath", filePath.toString());
                break;
            }
                
            case PO_FILE_LOADER_MODE_RETURN_AS_STRING:
                //Get File as string
                std::string fileContents = URLLoader::getFileAsString(url);
                
                //Set Dictionary with contents
                dict.set("mode", mode);
                dict.set("url", url.toString());
                dict.set("content", fileContents);
                break;
        }
        
        workerMessage = URLLoaderCompleteMessage;
    }
} /* End po Namespace */
