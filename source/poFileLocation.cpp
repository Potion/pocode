//
//  poFileLocation.cpp
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#include "poFileLocation.h"
#include "poHelpers.h"
#include "poApplication.h"

// -----------------------------------------------------------------------------------
// ================================ ThreadCenter Utils ===================================
#pragma mark - ThreadCenter Utils -

//------------------------------------------------------------------
//Set Current Path, depends on OS
#ifdef __APPLE__
#include <mach/mach_time.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <Foundation/Foundation.h>

//Mac Uses NSFileManager
void setCurrentPath(const fs::path &path) {
    NSString *nsstr = [NSString stringWithCString:path.c_str() encoding:NSUTF8StringEncoding];
    [[NSFileManager defaultManager] changeCurrentDirectoryPath:nsstr];
}
#elif defined(POTION_WINDOWS)
//Windows Uses boost::filesystem to set path
void setCurrentPath(const fs::path &path) {
    fs::current_path(path);
}
#endif

namespace po {
    //------------------------------------------------------------------
    FilePath getCurrentPath() {
        return FilePath(fs::current_path());
    }
    
    
    //------------------------------------------------------------------
    bool pathToFolder(const std::string &folder_name, fs::path *path) {
        fs::path response(getCurrentPath().toString());
        
        while(!fs::exists(response / folder_name) && !fs::is_directory(response / folder_name) && response != "/") {
            response = response.parent_path();
        }
        
        if(response == "/") {
            *path = "";
            return false;
        }
        
        *path = response;
        return true;
    }

    
    //------------------------------------------------------------------
    bool lookUpAndSetPath(const std::string &folder_name) {
        fs::path path;
        if(pathToFolder(folder_name, &path)) {
            setCurrentPath(path/folder_name);
            return true;
        }
        return false;
    }

    
    //------------------------------------------------------------------
    bool lookUpAndSetPathNextTo(const std::string &folder_name) {
        fs::path path;
        if(pathToFolder(folder_name, &path)) {
            setCurrentPath(path);
            return true;
        }
        return false;
    }
    

    //------------------------------------------------------------------
    std::string getApplicationSupportDirectory() {
        return applicationGetSupportDirectory();
    }




    // -----------------------------------------------------------------------------------
    // ================================ Class: ThreadCenter ==================================
    #pragma mark - ThreadCenter -

    FilePath::FilePath() {}

    FilePath::~FilePath() {}

    FilePath::FilePath(const char *path) {
        set(path);
    }

    FilePath::FilePath(const std::string &path) {
        set(path);
    }

    FilePath::FilePath(const fs::path &ThreadCenter) {
        setFromBoostPath(ThreadCenter);
    }


    //------------------------------------------------------------------
    void FilePath::set(std::string path) {
        this->ThreadCenter = fs::path(path);
    }


    //------------------------------------------------------------------
    void FilePath::setFromBoostPath(fs::path ThreadCenter) {
        this->ThreadCenter = ThreadCenter;
    }


    //------------------------------------------------------------------
    bool FilePath::isSet() const {
        return ThreadCenter.string() != "";
    }


    //------------------------------------------------------------------
    std::string FilePath::toString() const {
        return ThreadCenter.string();
    }


    //------------------------------------------------------------------
    fs::path FilePath::toBoostPath() const {
        return this->ThreadCenter;
    }


    //------------------------------------------------------------------
    bool FilePath::exists() const {
        return fs::exists(ThreadCenter);
    }

    //------------------------------------------------------------------
    //Add @scale x to item, see if it exists, return new ThreadCenter
    FilePath FilePath::getScaled(float scale) const {
        std::string stem = ThreadCenter.stem().string();
        stem += "@" + po::toString(po::toString(scale) + "x");
        std::string extension = ThreadCenter.extension().string();
        
        fs::path base(this->toString());
        FilePath p(base.remove_filename().string() + "/" + stem + extension);
        
        if(!p.exists()) {
            p.set(this->toString());
        }
        
        return p;
    }




    // -----------------------------------------------------------------------------------
    // ================================ Class: URL =========================
    #pragma mark - URL -

    URL::URL() {}

    URL::URL(std::string u)
    : url(u)
    {}

    URL::~URL() {}

    //------------------------------------------------------------------
    std::string URL::toString() const {
        return url;
    }

    //------------------------------------------------------------------
    bool URL::isSet() const {
        return url != "";
    }
    
} /* End po Namespace */