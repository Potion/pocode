/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poShape2D.h"

std::vector<poPoint> quadTo(poPoint p1, poPoint p2, poPoint control, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*p1 + 2*invt*t*control + t*t*p2;
		response.push_back(pt);
	}
	return response;
}

std::vector<poPoint> cubeTo(poPoint p1, poPoint p2, poPoint c1, poPoint c2, int resolution) {
	std::vector<poPoint> response;
	for(int i=0; i<resolution; i++) {
		float t = i / float(resolution-1);
		float invt = 1.f - t;
		poPoint pt = invt*invt*invt*p1 + 3*invt*invt*t*c1 + 3*invt*t*t*c2 + t*t*t*p2;
		response.push_back(pt);
	}
	return response;
}

poShape2D::poShape2D()
:	fillColor(1,1,1,1),
	strokeColor(0,0,0,0),
	strokeWidth(0),
	drawStyle(GL_POLYGON)
{}

void poShape2D::draw() {
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(VertexPack), &(vertices[0].point));
	
//	glEnableClientState(GL_COLOR_ARRAY);
//	glColorPointer(4, GL_FLOAT, sizeof(VertexPack), &(vertices[0].color));
	
	if(texture) {
		// make sure its on
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(VertexPack), &(vertices[0].texCoord));
		
		// save the current texture state
		glPushAttrib(GL_TEXTURE_BIT);
		texture.enableAndBind();
	}
	
	glColor4f(fillColor.red, fillColor.green, fillColor.blue, fillColor.alpha*master_alpha);
	glDrawArrays(drawStyle, 0, vertices.size());
	
	if(texture) {
		glPopAttrib();
	}
	
	// these lines go together because drawSolidRect modifies the enabled state
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	ci::gl::color(ci::ColorA(1,0,0,master_alpha));
	ci::gl::drawSolidRect(ci::Rectf(-offset.x-3, -offset.y-3, -offset.x+3, -offset.y+3));
	glPopClientAttrib();
	
	if(strokeWidth) {
		// make sure this is off
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		// turn on all the smoothing we need
		glPushAttrib(GL_LINE_BIT | GL_POINT_BIT);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		
		// set the line props
		glLineWidth(strokeWidth);
		glPointSize(strokeWidth);
		
		glColor4f(strokeColor.red, strokeColor.green, strokeColor.blue, strokeColor.alpha*master_alpha);
		// this vertex pointer was set above
		glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
		glDrawArrays(GL_POINTS, 0, vertices.size());
		
		glPopAttrib();
	}

	glPopClientAttrib();
}

void poShape2D::moveTo(poPoint pt) {
	lineTo(pt);
}

void poShape2D::moveTo(float x, float y) {
	lineTo(x, y);
}

void poShape2D::lineTo(poPoint pt) {
	vertices.push_back(pt);
}

void poShape2D::lineTo(float x, float y) {
	vertices.push_back(poPoint(x,y));
}

void poShape2D::quadTo(poPoint pt, poPoint control, int resolution) {
	std::vector<poPoint> points = ::quadTo(vertices.back().point, pt, control, resolution);
	// leave out the first one
	for(std::vector<poPoint>::iterator iter = points.begin() + 1;
		iter != points.end();
		iter++)
	{
		vertices.push_back(*iter);
	}
}

void poShape2D::cubeTo(poPoint pt, poPoint control1, poPoint control2, int resolution) {
	std::vector<poPoint> points = ::cubeTo(vertices.back().point, pt, control1, control2, resolution);
	// leave out the first one
	for(std::vector<poPoint>::iterator iter = points.begin() + 1;
		iter != points.end();
		iter++)
	{
		vertices.push_back(*iter);
	}
}

void poShape2D::close() {
	vertices.push_back(vertices.front());
}

void poShape2D::setAlignment(poAlignment align) {
	poObject::setAlignment(align);
	
	calculateBounds();
	
	switch(align) {
		case PO_ALIGN_TOP_LEFT:
			offset.set(0,0,0); break;
		case PO_ALIGN_TOP_CENTER:
			offset.set(-bounds.width()/2.f,0,0); break;
		case PO_ALIGN_TOP_RIGHT:
			offset.set(-bounds.width(),0,0); break;
		case PO_ALIGN_CENTER_LEFT:
			offset.set(0,-bounds.height()/2.f,0); break;
		case PO_ALIGN_CENTER_CENTER:
			offset.set(-bounds.width()/2.f,-bounds.height()/2.f,0); break;
		case PO_ALIGN_CENTER_RIGHT:
			offset.set(-bounds.width(),-bounds.height()/2.f,0); break;
		case PO_ALIGN_BOTTOM_LEFT:
			offset.set(0,-bounds.height(),0); break;
		case PO_ALIGN_BOTTOM_CENTER:
			offset.set(-bounds.width()/2.f,-bounds.height(),0); break;
		case PO_ALIGN_BOTTOM_RIGHT:
			offset.set(-bounds.width(),-bounds.height(),0); break;
	}
}

poRect poShape2D::calculateBounds(bool include_children) {
	poObject::calculateBounds(include_children);
	
	BOOST_FOREACH(VertexPack &vertex, vertices) {
		bounds.include(vertex.point);
	}
	
	return bounds;
}

int poShape2D::numPoints() const {
	return vertices.size();
}

poPoint &poShape2D::getPoint(int idx) {
	static poPoint invalid(0,0);
	if(vertices.empty() || idx < 0 || idx >= vertices.size())
		return invalid;
	return vertices[idx].point;
}

VertexPack &poShape2D::getPack(int idx) {
	static VertexPack invalid;
	if(vertices.empty() || idx < 0 || idx >= vertices.size())
		return invalid;
	return vertices[idx];
}

void poShape2D::placeTexture(ci::gl::Texture tex, float x, float y, float scale, float rot) {}
void poShape2D::placeTexture(ci::gl::Texture tex, poTextureScaleOption scaleOption) {}
