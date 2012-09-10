//
//  poFileLocation.h
//  pocode
//
//  Created by Steve Varga on 9/6/12.
//  Copyright (c) 2012 Potion Design. All rights reserved.
//

#pragma once

//------------------------------------------------------------------
//poFilePath
class poFilePath {
public:
    poFilePath();
    poFilePath(const char* filePath);
    poFilePath(const std::string &filepath);
    ~poFilePath();
    
    void set(std::string path);
    bool isSet() const;
    
    bool exists() const;
    
    std::string asString() const;
    
    poFilePath getScaled(float scale) const;
private:
    fs::path filepath;
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
    
    std::string asString() const;
private:
    std::string url;
};
