#pragma once

#include "poColor.h"
#include "poRect.h"

class poTexture;

void applyColor(poColor color);
void drawStroke(poRect rect);
void drawStroke(float l, float r, float b, float t);
void drawRect(poRect rect);
void drawRect(float l, float r, float b, float t);
void drawRect(poRect rect, poTexture* texture);
void drawRect(float l, float r, float b, float t, poTexture* texture);
