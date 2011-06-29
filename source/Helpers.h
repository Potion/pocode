#pragma once

#include "poRect.h"
#include "poFont.h"
#include "poTexture.h"

// in seconds
double getTime();
unsigned int getNumCpus();
poPoint deviceResolution();

// path related
fs::path currentPath();
void setCurrentPath(const fs::path &path);
// search up the filesystem from pwd for folder
bool pathToFolder(const std::string &folder_name, fs::path *path);
// search up the filesystem from pwd and set to a folder if its there
bool lookUpAndSetPath(const std::string &folder_name);

// point related
std::vector<poPoint> roundedRect(float w, float h, float r);
std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution);
std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution);
float curveLength(const std::vector<poPoint> &curve);

class poObject;
class poShape2D;

void applyObjTransform(poObject *obj);
void startMasking(poShape2D *mask);
void stopMasking();

void log(const char *format, ...);