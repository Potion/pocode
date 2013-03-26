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
// ================================ filePath Utils ===================================
#pragma mark - filePath Utils -

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

    // -----------------------------------------------------------------------------------
    // ================================ Class: filePath ==================================
    #pragma mark - filePath -

    FilePath::FilePath() {}

    FilePath::~FilePath() {}

    FilePath::FilePath(const char *path) {
        set(path);
    }

    FilePath::FilePath(const std::string &path) {
        set(path);
    }

    FilePath::FilePath(const fs::path &filePath) {
        setFromBoostPath(filePath);
    }


    //------------------------------------------------------------------
    void FilePath::set(std::string path) {
        this->filePath = fs::path(path);
    }


    //------------------------------------------------------------------
    void FilePath::setFromBoostPath(fs::path filePath) {
        this->filePath = filePath;
    }


    //------------------------------------------------------------------
    bool FilePath::isSet() const {
        return filePath.string() != "";
    }


    //------------------------------------------------------------------
    std::string FilePath::toString() const {
        return filePath.string();
    }


    //------------------------------------------------------------------
    fs::path FilePath::toBoostPath() const {
        return this->filePath;
    }


    //------------------------------------------------------------------
    bool FilePath::exists() const {
        return fs::exists(filePath);
    }

    //------------------------------------------------------------------
    //Add @scale x to item, see if it exists, return new filePath
    FilePath FilePath::getScaled(float scale) const {
        std::string stem = filePath.stem().string();
        stem += "@" + po::toString(po::toString(scale) + "x");
        std::string extension = filePath.extension().string();
        
        fs::path base(this->toString());
        std::string pathString = base.remove_filename().string();
        
        if(pathString != "") pathString += "/";
        pathString += stem + extension;
        
        FilePath p(pathString);
        
        if(!p.exists()) {
            p.set(this->toString());
        }
        return p;
    }




    // -----------------------------------------------------------------------------------
    // ================================ Class: URL =======================================
    #pragma mark - URL -

    URL::URL() {}

    URL::URL(std::string u)
    : url(u)
    {}

    URL::~URL() {}
    
    //------------------------------------------------------------------
    void URL::set(std::string url) {
        this->url = url;
    }
    

    //------------------------------------------------------------------
    bool URL::isSet() const {
        return url != "";
    }
    
    
    //------------------------------------------------------------------
    void URL::addHeader(std::string value) {
        headers.push_back(value);
    }
    
    
    //------------------------------------------------------------------
    const std::vector <std::string> &URL::getHeaders() {
        return headers;
    }
    
    
    //------------------------------------------------------------------
    void URL::setUsername(std::string username) {
        this->username = username;
    }
    
    
    //------------------------------------------------------------------
    const std::string URL::getUsername() {
        return this->username;
    }
    
    
    //------------------------------------------------------------------
    void URL::setPassword(std::string password) {
        this->password = password;
    }
    
    
    //------------------------------------------------------------------
    const std::string URL::getPassword() {
        return this->password;
    }
    
    
    //------------------------------------------------------------------
    std::string URL::toString() const {
        return url;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Utils =====================================
    #pragma mark - Utils -
    
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
    
    
    //------------------------------------------------------------------
    void listDirectory(po::FilePath dir) {
        boost::filesystem::path p = dir.toBoostPath();
        std::vector<boost::filesystem::path> files;
        
        boost::filesystem::directory_iterator beg(p);
        boost::filesystem::directory_iterator end;
        std::copy(beg, end, std::back_inserter(files));
        
        std::cout << files.size() << std::endl;
        
        std::vector<boost::filesystem::path>::iterator iter;
        for(iter = files.begin(); iter != files.end(); ++iter) {
            std::cout << (boost::filesystem::is_directory((*iter)) ? 'D' : ' ');
            std::cout << "  " << (*iter).filename() << std::endl;
        }
    }

    
} /* End po Namespace */