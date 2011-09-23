#pragma once

#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poTexture.h"
#include "poObject.h"
#include "poFont.h"

namespace po {

	void setColor(poColor color);
	void setColor(poColor color, float new_alpha);

	void drawStroke(poRect rect);
	void drawStroke(float x, float y, float w, float h);
	void drawLine(poPoint a, poPoint b);
	void drawRect(poRect rect);
	void drawRect(float x, float y, float w, float h);
	void drawRect(poTexture* tex);
	void drawRect(poRect rect, poTexture* texture);
	void drawRect(float x, float y, float w, float h, poTexture* texture);
	void drawRect(poRect rect, poRect coords, poTexture *texture);
	void drawRect(poRect rect, poTexture *tex, poTextureFitOption fit);
	void drawPoints(GLenum type, const std::vector<poPoint> &points);
	void drawPoints(GLenum type, GLuint tex_id, const std::vector<poPoint> &points, const std::vector<poPoint> &tex_coords);
	void drawString(const std::string &str, poFont *font, poPoint position, int ptSize=0, float tracking=1.f);
	
	void enableBlending();
	void disableBlending();
	
}
