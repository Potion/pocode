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

#pragma once


/*	Created by Stephen Varga on 11/6/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poFileLocation.h"
#include "poThreadCenter.h"
#include <string>

namespace po {

    //------------------------------------------------------------------
    //File Loader, uses CURL to save files or return as strings
    namespace URLLoader {
        FilePath getFile(URL url, const FilePath &savePath=FilePath());
        void getFileAsync(URL url, Object* notify, const FilePath &savePath=FilePath());
        
        std::string getFileAsString(URL url);
        void getFileAsStringAsync(URL url, Object* notify);
    };


    //------------------------------------------------------------------
    //File Loader Worker
    //For async operations
    enum URLLoaderMode {
        PO_FILE_LOADER_MODE_SAVE,
        PO_FILE_LOADER_MODE_RETURN_AS_STRING
    };

    static const std::string URLLoaderCompleteMessage = "PO_FILE_LOADER_COMPLETE_MESSAGE";

    class URLLoaderWorker  : public Worker {
    public:
        URLLoaderWorker(URL url, URLLoaderMode mode=PO_FILE_LOADER_MODE_RETURN_AS_STRING, const FilePath &savePath=FilePath());
        ~URLLoaderWorker();
        
        void workerFunc();
    private:
        URL url;
        FilePath filePath;
        URLLoaderMode mode;
    };
} /* End po Namespace */