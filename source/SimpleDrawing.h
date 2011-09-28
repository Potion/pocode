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
    
    void fill();
    void noFill();
    
    
    // HIGH QUALITY SHAPE STROKE
	std::vector<poPoint> generateStroke(std::vector<poPoint> &points, int strokeWidth, poStrokePlacementProperty place=PO_STROKE_PLACE_CENTER, 
                                           poStrokeJoinProperty join=PO_STROKE_JOIN_MITRE, poStrokeCapProperty cap=PO_STROKE_CAP_BUTT);
    
    void stroke(bool bSimpleStroke = true);
    void noStroke();
    
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
    
    void drawCircle(float x, float y, float radius);
    void drawOval(float x, float y, float width, float height);
    
	void drawPoints(GLenum type, const std::vector<poPoint> &points);
	void drawPoints(GLenum type, poTexture *tex, const std::vector<poPoint> &points, const std::vector<poPoint> &tex_coords);
    
	void drawString(const std::string &str, poFont *font, poPoint position, int ptSize=0, float tracking=1.f);
	
	void enableBlending();
	void disableBlending();
    
    //Settings
    static int circleResolution = 50;
    
    static bool bFill = true;
    
    static bool bClosed = true; //Has the path been closed?
    
    static bool bStroke = false;
    static bool useSimpleStroke = true;
	static int  stroke_width;
    
    static poStrokeCapProperty  cap;
	static poStrokeJoinProperty join;
}
