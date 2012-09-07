//
//  poFileLocation.cpp
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#include "poFileLocation.h"

//------------------------------------------------------------------
//poFilePath
#pragma mark poFilePath
poFilePath::poFilePath() {}

poFilePath::~poFilePath() {}

poFilePath::poFilePath(std::string filepath) {
    set(filepath);
}


//------------------------------------------------------------------
void poFilePath::set(std::string filepath) {
    this->filepath = fs::path(filepath);
}

//------------------------------------------------------------------
bool poFilePath::isSet() const {
    return filepath.string() != "";
}

//------------------------------------------------------------------
std::string poFilePath::asString() const {
    return filepath.string();
}

//------------------------------------------------------------------
bool poFilePath::exists() const {
    return fs::exists(filepath);
}



//------------------------------------------------------------------
//------------------------------------------------------------------
//poURL
#pragma mark poURL
poURL::poURL() {}

poURL::poURL(std::string u)
: url(u)
{}

poURL::~poURL() {}

//------------------------------------------------------------------
std::string poURL::asString() const {
    return url;
}

//------------------------------------------------------------------
bool poURL::isSet() const {
    return url != "";
}