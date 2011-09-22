#pragma once

#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poTexture.h"
#include "poObject.h"
#include "poFont.h"

<<<<<<< HEAD
=======
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
>>>>>>> acc9a8be33b5d658ff29543c2bfe023cc2331641

namespace po {

	void setColor(poColor color);
	void setColor(poColor color, float new_alpha);

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

}
