//
//  poFileLocation.cpp
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#include "poFileLocation.h"
#include "poHelpers.h"

//------------------------------------------------------------------
//poFilePath
#pragma mark poFilePath
poFilePath::poFilePath() {}

poFilePath::~poFilePath() {}

poFilePath::poFilePath(const char *filepath) {
    set(filepath);
}

poFilePath::poFilePath(const std::string &filepath) {
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
poFilePath poFilePath::getScaled(float scale) const {
    //Add @scale x to item, see if it exists
    std::string stem = filepath.stem().string();
    stem += "@" + poToString(poToString(scale) + "x");
    std::string extension = filepath.extension().string();
    
    fs::path base(this->asString());
    poFilePath p(base.remove_filename().string() + "/" + stem + extension);
    
    if(!p.exists()) {
        p.set(this->asString());
    }
    
    return p;
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