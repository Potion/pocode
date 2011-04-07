#include "poShape2D.h"

poShape2D::poShape2D()
:	enableFill(true)
,	strokeWidth(0)
,	fillColor(1,1,1,1)
,	strokeColor(0,0,0,0)
,	fillDrawStyle(GL_POLYGON)
,	enabledAttributes(poVertex::POINT)
{}

void poShape2D::draw() {
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(poVertex), &(vertices[0].point));
	
	if(enabledAttributes & poVertex::COLOR) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(poVertex), &(vertices[0].color));
	}
	else {
		glColor4f(fillColor.red, fillColor.green, fillColor.blue, fillColor.alpha*master_alpha);
	}
	
	if(enabledAttributes & poVertex::TEX_COORD) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(poVertex), &(vertices[0].texCoord));
	}
	
	if(enabledAttributes & poVertex::NORMAL) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(poVertex), &(vertices[0].normal));
	}
	
	glDrawArrays(fillDrawStyle, 0, (int)vertices.size());
	
	// these lines go together because drawSolidRect modifies the enabled state
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glColor4f(1,0,0,master_alpha);
    drawRect(-offset.x-3, -offset.x+3, -offset.y+3, -offset.y-3);
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
		glDrawArrays(GL_LINE_LOOP, 0, (int)vertices.size());
		glDrawArrays(GL_POINTS, 0, (int)vertices.size());
		
		glPopAttrib();
	}

	glPopClientAttrib();
}

void poShape2D::addPoint(poPoint p) {
	vertices.push_back(poVertex(p));
}

void poShape2D::addVertex(poVertex v) {
	vertices.push_back(v);
}

void poShape2D::addPoints(const std::vector<poPoint> &points) {
	vertices.insert(vertices.end(), points.begin(), points.end());
}

void poShape2D::addVertices(const std::vector<poVertex> &vertices) {
	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
}

void poShape2D::curveTo(poPoint pt, poPoint control, int resolution) {
	if(vertices.empty())
		vertices.push_back(poPoint(0,0,0));
	
	std::vector<poPoint> points = quadTo(vertices.back().point, pt, control, resolution);
	vertices.insert(vertices.end(), points.begin(), points.end());
}

void poShape2D::curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution) {
	if(vertices.empty())
		vertices.push_back(poPoint(0,0,0));

	std::vector<poPoint> points = cubeTo(vertices.back().point, pt, control1, control2, resolution);
	vertices.insert(vertices.end(), points.begin(), points.end());
}

void poShape2D::setPoints(const std::vector<poPoint> &points) {
	vertices.assign(points.begin(), points.end());
}

void poShape2D::setPoints(const std::vector<poVertex> &vertices) {
	this->vertices.assign(vertices.begin(), vertices.end());
}

void poShape2D::clearPoints() {
	vertices.clear();
}

size_t poShape2D::numPoints() const {
	return vertices.size();
}

poPoint &poShape2D::getPoint(int idx) {
	return vertices[idx].point;
}

poVertex &poShape2D::getVertex(int idx) {
	return vertices[idx];
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
            
        case PO_ALIGN_NUM_OPTIONS:
            // just to shut up the compiler warning
            break;
	}
}

poRect poShape2D::calculateBounds(bool include_children) {
	poObject::calculateBounds(include_children);
	
	BOOST_FOREACH(poVertex &vertex, vertices) {
		bounds.include(vertex.point);
	}
	
	return bounds;
}
