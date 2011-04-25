#include "poShape2D.h"

poShape2D::poShape2D()
:	enable_fill(true)
,	enable_stroke(false)
,	stroke_width(0)
,	fill_color(1,1,1,1)
,	stroke_color(0,0,0,0)
,	fill_draw_style(GL_LINE_LOOP)
,	enabled_attributes(poVertex::ATTRIB_POINT)
{}

void poShape2D::draw() {
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(poVertex), &(vertices[0].point));
	
	if(isAttributeEnabled(poVertex::ATTRIB_COLOR)) {
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(poVertex), &(vertices[0].color));
	}
	else {
		glColor4f(fill_color.red, fill_color.green, fill_color.blue, fill_color.alpha*master_alpha);
	}
	
	if(isAttributeEnabled(poVertex::ATTRIB_TEX_COORD)) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(poVertex), &(vertices[0].texCoord));
	}
	
	if(isAttributeEnabled(poVertex::ATTRIB_NORMAL)) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, sizeof(poVertex), &(vertices[0].normal));
	}
	
	glDrawArrays(fill_draw_style, 0, (int)vertices.size());
	
	// these lines go together because drawSolidRect modifies the enabled state
	//	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	//	glColor4f(1,0,0,master_alpha);
	//	drawRect(-offset.x-3, -offset.x+3, -offset.y+3, -offset.y-3);
	//	glPopClientAttrib();
	
	if(enable_stroke) {
		// make sure this stuff is off
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glColor4f(stroke_color.red, stroke_color.green, stroke_color.blue, stroke_color.alpha*master_alpha);
		glVertexPointer(3, GL_FLOAT, sizeof(poPoint), &(stroke[0]));
		glDrawArrays(GL_LINE_STRIP, 0, (int)stroke.size());
		
		// draw drag point
		glColor3f(1,1,1);
		glRectf(vertices[2].point.x-5,vertices[2].point.y-5,
				vertices[2].point.x+5,vertices[2].point.y+5);
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

bool poShape2D::isFillEnabled() const {return enable_fill;}
void poShape2D::enableFill(bool b) {enable_fill = b;}

bool poShape2D::isStrokeEnabled() const {return enable_stroke;}
void poShape2D::enableStroke(bool b) {
	enable_stroke = b; 
	generateStroke();
}

int poShape2D::strokeWidth() const {return stroke_width;}
void poShape2D::strokeWidth(int w) {
	stroke_width = w; 
	enableStroke(stroke_width > 0);
}

poColor poShape2D::fillColor() const {return fill_color;}
void poShape2D::fillColor(poColor c) {fill_color = c;}

poColor poShape2D::strokeColor() const {return stroke_color;}
void poShape2D::strokeColor(poColor c) {stroke_color = c;}

GLenum poShape2D::fillDrawStyle() const {return fill_draw_style;}
void poShape2D::fillDrawStyle(GLenum e) {fill_draw_style = e;}

bool poShape2D::isAttributeEnabled(poVertex::Attribute a) const {return (enabled_attributes & a) != 0;}
void poShape2D::enableAttribute(poVertex::Attribute a) {enabled_attributes |= a;}

poShape2D::StrokeCapProperty poShape2D::capStyle() const {return cap;}
void poShape2D::capStyle(StrokeCapProperty p) {
	cap = p;
	generateStroke();
}

poShape2D::StrokeJoinProperty poShape2D::joinStyle() const {return join;}

void poShape2D::joinStyle(StrokeJoinProperty p) {
	join = p;
	generateStroke();
}

void poShape2D::generateStroke() {
	stroke.clear();
	
	if(enable_stroke) {
		std::vector<LineSeg> segments;
		
		poPoint p1, p2, p3, p4, tmp;
		
		for(int i=0; i<vertices.size()-1; i++) {
			p1 = vertices[i].point;
			p2 = vertices[i+1].point;
			tmp = p2 - p1;
			LineSeg seg = extrudeLineSegment(p1, p2, stroke_width);
			segments.push_back(seg);
		}
		
		// add the first cap
		stroke.push_back(segments[0].p1);
		stroke.push_back(segments[0].p2);

		// generate middle points
		for(int i=0; i<segments.size()-1; i++) {
			poPoint top, bottom;
			float angle = combineExtrudedLineSegments(segments[i], segments[i+1], &top, &bottom);
			bool top_outside = angle < 0.f;
			
//			printf("%d,%d: %f (%d) (%f,%f,%f) (%f,%f,%f)\n", 
//				   i, i+1, rad2deg(angle), top_outside,
//				   top.x, top.y, top.z,
//				   bottom.x, bottom.y, bottom.z);
			
			switch(join) {
				case STROKE_JOIN_MITRE:
					break;
					
				case STROKE_JOIN_BEVEL:
					if(top_outside) {
						stroke.push_back(segments[i].p3);
						stroke.push_back(bottom);
						stroke.push_back(segments[i+1].p1);
						stroke.push_back(bottom);
					}
					else {
						stroke.push_back(top);
						stroke.push_back(segments[i].p4);
						stroke.push_back(top);
						stroke.push_back(segments[i+1].p2);
					}
					break;
					
				case STROKE_JOIN_ROUND:
				{
					float halfW = stroke_width / 2.f;
					
					if(top_outside) {
						p1 = bottom;
						p2 = segments[i].p3;
						p3 = vertices[i+1].point;
						p4 = segments[i+1].p1;
						
						stroke.push_back(p2);
						stroke.push_back(p1);
						
						float a1 = angleBetweenPoints(p2, p3, p4);
						if(a1 < 0)
							a1 = M_PI + a1;
						
						poPoint diff = p2 - p3;
						float a2 = atan2f(diff.y, diff.x);
						
						float step = a1 / 9.f;
						float a = a2;
						
						for(int j=0; j<10; j++) {
							tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
							stroke.push_back(tmp+p3);
							stroke.push_back(p3);
							a -= step;
						}
						
						stroke.push_back(p4);
						stroke.push_back(p1);
					}
					else {
						p1 = top;
						p2 = segments[i].p4;
						p3 = vertices[i+1].point;
						p4 = segments[i+1].p2;
					
						stroke.push_back(p1);
						stroke.push_back(p2);
						
						float a1 = angleBetweenPoints(p2, p3, p4);
						if(a1 > 0)
							a1 = -(M_PI - a1);
						
						poPoint diff = p2 - p3;
						float a2 = atan2f(diff.y, diff.x);
						
						float step = a1 / 9.f;
						float a = a2;
						
						for(int j=0; j<10; j++) {
							tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
							stroke.push_back(p3);
							stroke.push_back(tmp+p3);
							a -= step;
						}
						
						stroke.push_back(p1);
						stroke.push_back(p4);
					}
					
					break;
				}
			}
		}

		// add the last cap
		stroke.push_back(segments[segments.size()-1].p3);
		stroke.push_back(segments[segments.size()-1].p4);
	}
}

LineSeg extrudeLineSegment(poPoint p1, poPoint p2, float width) {
	poPoint diff = p2 - p1;
	float angle = atan2(diff.y, diff.x);
	float halfW = width / 2.f;
	float c1 = cosf(angle + M_HALF_PI) * halfW;
	float s1 = sinf(angle + M_HALF_PI) * halfW;
	float c2 = cosf(angle - M_HALF_PI) * halfW;
	float s2 = sinf(angle - M_HALF_PI) * halfW;
	return LineSeg(p1 + poPoint(c1,s1),
				   p1 + poPoint(c2,s2),
				   p2 + poPoint(c1,s1),
				   p2 + poPoint(c2,s2));
}

float determinant(poPoint row1, poPoint row2, poPoint row3) {
	return (row1.x*row2.y*row3.z + row1.y*row2.z*row3.x + row1.z*row2.x*row3.y -
			row1.x*row2.z*row3.y - row1.y*row2.x*row3.z - row1.z*row2.y*row3.x);
}

bool rayIntersection(Ray r1, Ray r2, poPoint *p1, poPoint *p2) {
	poPoint c1 = r1.dir.cross(r2.dir);
	float len_sqr = c1.lengthSquared();
	
	if(compare(len_sqr, 0.f)) {
		return false;
	}
	
	poPoint diff = r2.origin - r1.origin;
	float t1 = determinant(diff, r2.dir, c1) / len_sqr;
	float t2 = determinant(diff, r1.dir, c1) / len_sqr;
	
	*p1 = r1.origin + r1.dir*t1;
	*p2 = r2.origin + r2.dir*t2;
	
	return *p1 == *p2;
}

float angleBetweenPoints(poPoint a, poPoint b, poPoint c) {
	poPoint ba = b - a;
	poPoint bc = b - c;
	
	float ret = ::fmodf(atan2f(ba.y,ba.x) - atan2f(bc.y,bc.x), M_PI);
	if(compare(ret,0.f) || compare(fabsf(ret),M_PI))
		ret = 0.f;
	return ret;
}

float angleBetweenSegments(LineSeg seg1, LineSeg seg2) {
	poPoint p1 = (seg1.p2 - seg1.p1) / 2.f + seg1.p1;
	poPoint p2 = (seg1.p4 - seg1.p3) / 2.f + seg1.p3;
	poPoint p3 = (seg2.p4 - seg2.p3) / 2.f + seg2.p3;
	
	return angleBetweenPoints(p1, p2, p3);
}

float combineExtrudedLineSegments(LineSeg seg1, LineSeg seg2, poPoint *top, poPoint *bottom) {
	poPoint i1, i2, i3, i4;
	
	float angle = angleBetweenSegments(seg1, seg2);
	
	if(compare(angle,0.f)) {
		*top = seg1.p3;
		*bottom = seg2.p4;
	}
	else {
		rayIntersection(Ray(seg1.p2, seg1.p4-seg1.p2),
						Ray(seg2.p4, seg2.p2-seg2.p4),
						&i1, &i2);
		
		rayIntersection(Ray(seg1.p1, seg1.p3-seg1.p1),
						Ray(seg2.p3, seg2.p1-seg2.p3),
						&i3, &i4);
		
		*top = i3;
		*bottom = i1;
	}
	
	return angle;
}
