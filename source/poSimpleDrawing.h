#pragma once

#include "poColor.h"
#include "poRect.h"

class poTexture;

void applyColor(poColor color);
void drawStroke(poRect rect);
void drawStroke(float x, float y, float w, float h);
void drawRect(poRect rect);
void drawRect(float x, float y, float w, float h);
void drawRect(poRect rect, poTexture* texture, bool flip=false);
void drawRect(float x, float y, float w, float h, poTexture* texture, bool flip=false);
void drawRect(poRect rect, poRect coords, poTexture *texture, bool flip=false);
void drawRect(poTexture* tex, bool flip=false);
void drawPoints(const std::vector<poPoint> &points);
