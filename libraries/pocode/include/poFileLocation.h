//
//  poFileLocation.h
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#pragma once

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class poFilePath;

//Path Utils
namespace po {
    poFilePath getCurrentPath();
    
    // search up the filesystem from pwd for folder
    bool pathToFolder(const std::string &folder_name, fs::path *path);
    
    // search up the filesystem from pwd and set to a folder if its there
    bool lookUpAndSetPath(const std::string &folder_name);
    bool lookUpAndSetPathNextTo(const std::string &folder_name);
    std::string getApplicationSupportDirectory();
}

//------------------------------------------------------------------
//poFilePath
class poFilePath {
public:
    poFilePath();
    poFilePath(const char* path);
    poFilePath(const std::string &path);
    poFilePath(const fs::path &filePath);
    ~poFilePath();
    
    void set(std::string path);
    void setFromBoostPath(fs::path filePath);
    bool isSet() const;
    
    bool exists() const;
    
    std::string toString() const;
    fs::path    toBoostPath() const;
    
    poFilePath getScaled(float scale) const;
private:
    fs::path filePath;
};


//------------------------------------------------------------------
//poURL
class poURL {
public:
    poURL();
    explicit poURL(std::string path);
    ~poURL();
    
    void set(std::string url);
    bool isSet() const;
    
    std::string toString() const;
private:
    std::string url;
};
