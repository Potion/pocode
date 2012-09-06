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

//------------------------------------------------------------------
//File Loader, uses CURL to save files or return as strings
namespace poURLLoader {
    poFilePath getFile(poURL url, const poFilePath &savePath=poFilePath());
    void getFileAsync(poURL url, poObject* notify, const poFilePath &savePath=poFilePath());
    
    std::string getFileAsString(poURL url);
    void getFileAsStringAsync(poURL url, poObject* notify);
};


//------------------------------------------------------------------
//File Loader Worker
//For async operations
enum poURLLoaderMode {
    PO_FILE_LOADER_MODE_SAVE,
    PO_FILE_LOADER_MODE_RETURN_AS_STRING
};

static const std::string poURLLoaderCompleteMessage    = "PO_FILE_LOADER_COMPLETE_MESSAGE";

class poURLLoaderWorker  : public poWorker {
public:
    poURLLoaderWorker(poURL url, poURLLoaderMode mode=PO_FILE_LOADER_MODE_RETURN_AS_STRING, const poFilePath &savePath=poFilePath());
    ~poURLLoaderWorker();
    
    void workerFunc();
private:
    poURL url;
    poFilePath savePath;
    poURLLoaderMode mode;
};