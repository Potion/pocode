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

class FilePath;

//Path Utils
namespace po {
    class FilePath;
    
    FilePath getCurrentPath();
    
    // search up the filesystem from pwd for folder
    bool pathToFolder(const std::string &folder_name, fs::path *path);
    
    // search up the filesystem from pwd and set to a folder if its there
    bool lookUpAndSetPath(const std::string &folder_name);
    bool lookUpAndSetPathNextTo(const std::string &folder_name);
    std::string getApplicationSupportDirectory();

    //------------------------------------------------------------------
    //FilePath
    class FilePath {
    public:
        FilePath();
        FilePath(const char* path);
        FilePath(const std::string &path);
        FilePath(const fs::path &filePath);
        ~FilePath();
        
        void set(std::string path);
        void setFromBoostPath(fs::path filePath);
        bool isSet() const;
        
        bool exists() const;
        
        std::string toString() const;
        fs::path    toBoostPath() const;
        
        FilePath getScaled(float scale) const;
    private:
        fs::path filePath;
    };


    //------------------------------------------------------------------
    //URL
    class URL {
    public:
        URL();
        explicit URL(std::string path);
        ~URL();
        
        void set(std::string url);
        bool isSet() const;
        
        std::string toString() const;
    private:
        std::string url;
    };
}/* End po Namespace */