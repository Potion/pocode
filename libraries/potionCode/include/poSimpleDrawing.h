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
	
	void drawLine(poPoint a, poPoint b);
    
	void drawStrokedRect(float x, float y, float w, float h);
	void drawStrokedRect(poRect rect);
	void drawFilledRect(float x, float y, float w, float h);
	void drawFilledRect(poRect rect);
	
	void drawTexturedRect(poTexture tex);
	void drawTexturedRect(poTexture tex, float x, float y, float w, float h);
	void drawTexturedRect(poTexture tex, poRect rect);
	void drawTexturedRect(poTexture tex, poRect rect, poRect coords);
	void drawTexturedRect(poTexture tex, poRect rect, poTextureFitOption fit);
	
	void drawStrokedPolygon(const std::vector<poPoint> &points);
	void drawStrokedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	void drawFilledPolygon(const std::vector<poPoint> &points);
	void drawFilledPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices);
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture tex, const std::vector<poPoint> &texCoords);
	void drawTexturedPolygon(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture tex, const std::vector<poPoint> &texCoords);
	
	void drawPoints(const std::vector<poPoint> &points, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, poTexture tex, const std::vector<poPoint> &texCoords, GLenum type);
	void drawPoints(const std::vector<poPoint> &points, const std::vector<unsigned short> &indices, poTexture tex, const std::vector<poPoint> &texCoords, GLenum type);
    
	void drawString(const std::string &str, poFont font, poPoint position, int ptSize=0, float tracking=1.f);

}


