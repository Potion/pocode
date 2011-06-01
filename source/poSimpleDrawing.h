#pragma once

#include "poColor.h"
#include "poRect.h"

class poTexture;

void applyColor(poColor color);
void drawStroke(poRect rect);
void drawStroke(float x, float y, float w, float h);
void drawRect(poRect rect);
void drawRect(float x, float y, float w, float h);
void drawRect(poRect rect, poTexture* texture);
void drawRect(float x, float y, float w, float h, poTexture* texture);
void drawRect(poTexture* tex);
void drawPoints(const std::vector<poPoint> &points);
