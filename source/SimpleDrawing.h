#pragma once

#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poTexture.h"
#include "poObject.h"
#include "poFont.h"

void applyColor(poColor color);
void applyColor(poColor color, float alpha);
poColor currentColor();
void drawStroke(poRect rect);
void drawStroke(float x, float y, float w, float h);
void drawLine(poPoint a, poPoint b);
void drawRect(poRect rect);
void drawRect(float x, float y, float w, float h);
void drawRect(poRect rect, poTexture* texture, bool flip=false);
void drawRect(float x, float y, float w, float h, poTexture* texture, bool flip=false);
void drawRect(poRect rect, poRect coords, poTexture *texture, bool flip=false);
void drawRect(poRect rect, poTexture *tex, poTextureFitOption fit);
void drawRect(poTexture* tex, bool flip=false);
void drawPoints(GLenum type, const std::vector<poPoint> &points);
void drawString(const std::string &str, poFont *font, poPoint position, int ptSize=0, float tracking=1.f);

// figures out tex coords to fit texture in rect
std::vector<poPoint> textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align);
// these do the same but make coordinates for each point in points array
// returns texture coordinates thru coords
void textureFit(poRect rect, poTexture *tex, poTextureFitOption fit, poAlignment align, std::vector<poPoint> &coords, const std::vector<poPoint> &points);
