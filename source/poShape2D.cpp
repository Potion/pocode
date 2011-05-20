#include "poShape2D.h"

poShape2D::poShape2D()
:	enable_fill(true)
,	enable_stroke(false)
,	stroke_width(0)
,	fill_color(1,1,1,1)
,	stroke_color(1,1,1,1)
,   useGenSroke(false)
,	fill_draw_style(GL_TRIANGLE_FAN)
,	enabled_attributes(ATTRIB_POINT)
,	closed_(true)
,	textures(MAX_TEXTURE_UNITS)
,	tex_combo_func(MAX_TEXTURE_UNITS, GL_REPLACE)
{}

void poShape2D::draw() {
    
    // push attributes
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    
    // do shape fill
    if ( enable_fill ) {
        // load shape point vertex array
        glVertexPointer(3, GL_FLOAT, 0, &(points[0].x));
        
        // use fillColor or per point color
        if(isAttributeEnabled(ATTRIB_COLOR)) {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, 0, &(colors[0].red));
        }
        else {
            glColor4f(fill_color.R, fill_color.G, fill_color.B, fill_color.A*true_alpha);
        }
        
        // setup textures and texture coordinates
        if(isAttributeEnabled(ATTRIB_TEX_COORD)) {
            glPushAttrib(GL_TEXTURE_BIT);
            
            for(int i=0; i<MAX_TEXTURE_UNITS; i++) {
                if(textures[i]) {
                    glClientActiveTexture(GL_TEXTURE0+i);
                    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    glTexCoordPointer(2, GL_FLOAT, sizeof(poPoint), &(tex_coords[i][0].x));

                    textures[i]->bind(i);
                    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex_combo_func[i]);
                    glEnable(GL_TEXTURE_2D);
                }
            }
        }
        
        // actually draw the filled shape
        glDrawArrays(fill_draw_style, 0, (int)points.size());
        
        // disable textures
        if(isAttributeEnabled(ATTRIB_TEX_COORD)) {
            for(int i=0; i<MAX_TEXTURE_UNITS; i++) {
                if(textures[i]) {
                    textures[i]->unbind(i);
                }
            }
            glPopAttrib();
        }
    }
    
    // do shape stroke
	if(enable_stroke) {
        // make sure this stuff is off
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        // set stroke color
		glColor4f(stroke_color.R, stroke_color.G, stroke_color.B, stroke_color.A*true_alpha);
		
        // use standard OpenGL stroke or custom generated stroke
        if ( useGenSroke )
        {
            glVertexPointer(3, GL_FLOAT, sizeof(poPoint), &(stroke[0]));
            glDrawArrays(GL_TRIANGLE_STRIP, 0, (int)stroke.size());  
        }
        else
        {
            glLineWidth( stroke_width );
            glVertexPointer(3, GL_FLOAT, 0, &(points[0].x));
            glDrawArrays(GL_LINE_STRIP, 0, (int)points.size());
            if ( closed_ && points.size() > 2)
            {
                glBegin( GL_LINE_STRIP );
                glVertex2f( points[0].x,points[0].y );
                glVertex2f( points.back().x, points.back().y );
                glEnd();
            }
        }
		
	}
	
    // pop attributes
	glPopClientAttrib();
}


poShape2D& poShape2D::addPoint(poPoint p) {
	points.push_back(p);
	return *this;
}

poShape2D& poShape2D::addPoint( float x, float y ) {
    points.push_back( poPoint(x,y) );
	return *this;
}

poShape2D& poShape2D::addPoints(const std::vector<poPoint> &pts) {
	points.insert(points.end(), pts.begin(), pts.end());
	return *this;
}

poShape2D& poShape2D::curveTo(poPoint pt, poPoint control, int resolution) {
	if(points.empty())
		points.push_back(poPoint(0,0,0));
	
	std::vector<poPoint> pts = quadTo(points.back(), pt, control, resolution);
	pts.insert(points.end(), pts.begin(), pts.end());
	return *this;
}

poShape2D& poShape2D::curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution) {
	if(points.empty())
		points.push_back(poPoint(0,0,0));
	
	std::vector<poPoint> pts = cubeTo(points.back(), pt, control1, control2, resolution);
	points.insert(points.end(), pts.begin(), pts.end());
	return *this;
}

void poShape2D::setPoints(const std::vector<poPoint> &pts) {
	points.assign(pts.begin(), pts.end());
}

poShape2D& poShape2D::clearPoints() {
	points.clear();
	return *this;
}

size_t poShape2D::numPoints() const {
	return points.size();
}

poPoint& poShape2D::getPoint(int idx) {
	return points[idx];
}

bool    poShape2D::setPoint(int idx, poPoint p )
{
    if ( idx < 0 || idx >= numPoints() )
        return false;
    points[idx] = p;
    return true;
}

poPoint& poShape2D::getTexCoord(int idx, uint unit) {
	return tex_coords[unit][idx];
}

poColor& poShape2D::getColor(int idx) {
	return colors[idx];
}



void poShape2D::setAlignment(poAlignment align) {
	poObject::setAlignment(align);
	
    // first calculate bounds
	calculateBounds();
	
    // then set offset based upon bounds and alignment
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
    // calculate the boounds
	poObject::calculateBounds(include_children);
	
    // calculate our children's bounds
	BOOST_FOREACH(poPoint &point, points) {
		bounds.include(point);
	}
	
	return bounds;
}

void poShape2D::placeTexture(poTexture *tex, uint unit) {
	textures[unit] = tex;

    // set texture coordinates, stretched to bounds of shape
	if(tex) {
		poRect rect = calculateBounds();
		
		tex_coords[unit].resize(points.size());
		for(int i=0; i<points.size(); i++) {
			float s = points[i].x / rect.width() * tex->s();
			float t = points[i].y / rect.height() * tex->t();
			tex_coords[unit][i].set(s,t,0.f);
		}
		
		enableAttribute(ATTRIB_TEX_COORD);
	}
	else    // if texture is NULL, turn off texture
    {
		bool found_one = false;
		for(int i=0; i<GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS; i++)
			found_one |= (textures[i] != NULL);
		if(!found_one)
			disableAttribute(ATTRIB_TEX_COORD);
	}
}



poShape2D&  poShape2D::enableFill(bool b) {enable_fill = b; return *this;}
bool        poShape2D::isFillEnabled() const {return enable_fill;}

poShape2D&  poShape2D::enableStroke(bool b) {enable_stroke = b; return *this;}
bool        poShape2D::isStrokeEnabled() const {return enable_stroke;}

poShape2D&  poShape2D::strokeWidth(int w) {
	stroke_width = w; 
	enableStroke(stroke_width > 0);
    return *this;
}
int         poShape2D::strokeWidth() const {return stroke_width;}

poShape2D&  poShape2D::fillColor(poColor c) {fill_color = c; return *this; }
poShape2D&  poShape2D::fillColor( float r, float g, float b, float a ) { fill_color.set(r,g,b,a); return *this; };
poColor     poShape2D::fillColor() const {return fill_color;}

poColor     poShape2D::strokeColor() const {return stroke_color;}
poShape2D&  poShape2D::strokeColor( float r, float g, float b, float a ) { stroke_color.set(r,g,b,a); return *this; };
poShape2D&  poShape2D::strokeColor(poColor c) {stroke_color = c; return *this; }

poShape2D&  poShape2D::fillDrawStyle(GLenum e) {fill_draw_style = e; return *this; }
GLenum      poShape2D::fillDrawStyle() const {return fill_draw_style;}

poShape2D&  poShape2D::useGeneratedStroke( bool b ) { useGenSroke = b; return *this; };
bool        poShape2D::isStrokeGenerationEnabled() { return useGenSroke; };

poShape2D&  poShape2D::enableAttribute(VertexAttribute a) {enabled_attributes |= a; return *this; }
poShape2D&  poShape2D::disableAttribute(VertexAttribute a) {enabled_attributes &= ~a; return *this; }
bool        poShape2D::isAttributeEnabled(VertexAttribute a) const {return (enabled_attributes & a) != 0;}

poShape2D&  poShape2D::capStyle(StrokeCapProperty p) {cap = p; return *this; }
StrokeCapProperty poShape2D::capStyle() const {return cap;}

poShape2D&  poShape2D::joinStyle(StrokeJoinProperty p) {join = p; return *this; }
StrokeJoinProperty poShape2D::joinStyle() const {return join;}

poShape2D&  poShape2D::closed(bool b) {closed_ = b; return *this; }
bool        poShape2D::isClosed() const {return closed_;}

GLenum      poShape2D::textureCombineFunction(uint unit) const {return tex_combo_func[unit];}
poShape2D&  poShape2D::textureCombineFunction(GLenum func, uint unit) {tex_combo_func[unit] = func; return *this; }




void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, StrokeJoinProperty join, float stroke_width);

void poShape2D::generateStroke() {
	stroke.clear();
	
	if(enable_stroke) {
		std::vector<poExtrudedLineSeg> segments;
		
		poPoint p1, p2, p3, p4, tmp;
		
		for(int i=0; i<points.size()-1; i++) {
			p1 = points[i];
			p2 = points[i+1];
			segments.push_back(poExtrudedLineSeg(p1, p2, stroke_width));
		}
		
		if(closed_) {
			poExtrudedLineSeg seg1(points[points.size()-1], points[0], stroke_width);
			poExtrudedLineSeg seg2(points[0], points[1], stroke_width);
			segments.push_back(seg1);
			segments.push_back(seg2);
			
			poPoint top, bottom;
			bool top_outside = combineExtrudedLineSegments(seg1, seg2, &top, &bottom);
			
			if(top_outside) {
				if(join == STROKE_JOIN_MITRE) {
					p1 = top;
					p2 = bottom;
				}
				else {
					p1 = seg2.p1;
					p2 = bottom;
				}
			}
			else {
				if(join == STROKE_JOIN_MITRE) {
					p1 = bottom;
					p2 = top;
				}
				else {
					p1 = top;
					p2 = seg2.p2;
				}
			}
			
			stroke.push_back(p1);
			stroke.push_back(p2);
		}
		else {
			// add the first cap
			stroke.push_back(segments[0].p1);
			stroke.push_back(segments[0].p2);
		}
		
		// generate middle points
		for(int i=0; i<segments.size()-1; i++) {
			makeStrokeForJoint(stroke, segments[i], segments[i+1], join, stroke_width);
		}
        
		if(!closed_) {
			stroke.push_back(segments[segments.size()-1].p3);
			stroke.push_back(segments[segments.size()-1].p4);
		}
	}
}


void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, StrokeJoinProperty join, float stroke_width) {
	poPoint top, bottom;
	poPoint p1, p2, p3, p4, tmp;
	
	bool top_outside = combineExtrudedLineSegments(seg1, seg2, &top, &bottom);
	poPoint joint = (seg1.p4 + seg1.p3) / 2.f;
	
	switch(join) {
		case STROKE_JOIN_MITRE:
			if(top_outside) {
				stroke.push_back(top);
				stroke.push_back(bottom);
			}
			else {
				stroke.push_back(bottom);
				stroke.push_back(top);
			}
			break;
			
		case STROKE_JOIN_BEVEL:
			if(top_outside) {
				stroke.push_back(seg1.p3);
				stroke.push_back(bottom);
				stroke.push_back(seg2.p1);
				stroke.push_back(bottom);
			}
			else {
				stroke.push_back(top);
				stroke.push_back(seg1.p4);
				stroke.push_back(top);
				stroke.push_back(seg2.p2);
			}
			break;
			
		case STROKE_JOIN_ROUND:
		{
			float halfW = stroke_width / 2.f;
			
			if(top_outside) {
				p1 = bottom;
				p2 = seg1.p3;
				p3 = joint;
				p4 = seg2.p1;
				
				stroke.push_back(p2);
				stroke.push_back(p1);
				
				float a1 = angleBetweenPoints(p2, p3, p4);
				
				poPoint diff = p2 - p3;
				float a2 = atan2f(diff.y, diff.x);
				
				float step = a1 / 9.f;
				float a = a2;
				
				for(int j=0; j<10; j++) {
					tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
					stroke.push_back(tmp+p3);
					stroke.push_back(p3);
					a += step;
				}
				
				stroke.push_back(p4);
				stroke.push_back(p1);
			}
			else {
				p1 = top;
				p2 = seg1.p4;
				p3 = joint;
				p4 = seg2.p2;
				
				stroke.push_back(p1);
				stroke.push_back(p2);
				
				float a1 = angleBetweenPoints(p2, p3, p4);
				
				poPoint diff = p2 - p3;
				float a2 = atan2f(diff.y, diff.x);
				
				float step = a1 / 9.f;
				float a = a2;
				
				for(int j=0; j<10; j++) {
					tmp.set(cosf(a)*halfW, sinf(a)*halfW, 0.f);
					stroke.push_back(p3);
					stroke.push_back(tmp+p3);
					a += step;
				}
				
				stroke.push_back(p1);
				stroke.push_back(p4);
			}
			
			break;
		}
	}
}

