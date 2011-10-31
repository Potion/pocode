#pragma once

#include "poColor.h"
#include "poRect.h"
#include "poEnums.h"
#include "poTexture.h"
#include "poObject.h"
#include "poFont.h"

namespace po {
	
	std::vector<poPoint> generateStroke(std::vector<poPoint> &points, 
										int strokeWidth, 
										bool close = false,
										poStrokePlacementProperty place = PO_STROKE_PLACE_CENTER, 
										poStrokeJoinProperty join = PO_STROKE_JOIN_MITRE, 
										poStrokeCapProperty cap = PO_STROKE_CAP_BUTT);
	std::vector<poPoint> generateOval(float xRad, float yRad, uint resolution=50);
    
    void setColor(poColor color);
	void setColor(poColor color, float new_alpha);
    
	void drawStroke(float x, float y, float w, float h);
	void drawStroke(poRect rect);
    
	void drawLine(poPoint a, poPoint b);
    
	void drawRect(float x, float y, float w, float h);
	void drawRect(poRect rect);

	void drawRect(poTexture tex);
	void drawRect(poTexture tex, float x, float y, float w, float h);
	void drawRect(poTexture tex, poRect rect);
	void drawRect(poTexture tex, poRect rect, poRect coords);
	void drawRect(poTexture tex, poRect rect, poTextureFitOption fit);
    
	void drawPoints(const std::vector<poPoint> &points, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, poTexture tex, const std::vector<poPoint> &tex_coords, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture tex, const std::vector<poPoint> &tex_coords, GLenum type);
    
	void drawString(const std::string &str, poFont font, poPoint position, int ptSize=0, float tracking=1.f);

}


