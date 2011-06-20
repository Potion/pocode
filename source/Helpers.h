#pragma once

#include "poRect.h"
#include "poFont.h"
#include "poTexture.h"

// in seconds
double getTime();
unsigned int getNumCpus();
poPoint deviceResolution();

fs::path currentPath();
void setCurrentPath(const fs::path &path);
// search up the filesystem from pwd for folder
bool pathToFolder(const std::string &folder_name, fs::path *path);
