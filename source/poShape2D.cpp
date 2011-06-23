#include "poShape2D.h"
#include "Helpers.h"
#include "LineExtruder.h"

poShape2D::poShape2D()
:	enable_fill(true)
,	enable_stroke(false)
,	stroke_width(0)
,	fill_color(1,1,1,1)
,	stroke_color(1,1,1,1)
,   use_simple_stroke(false)
,	fill_draw_style(GL_TRIANGLE_FAN)
,	enabled_attributes(ATTRIB_POINT)
,	closed_(true)
,	textures(MAX_TEXTURE_UNITS)
,	tex_combo_func(MAX_TEXTURE_UNITS, GL_MODULATE)
,	draw_bounds(false)
,	fill_color_tween(&fill_color)
,	alpha_test_textures(false)
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
			applyColor(poColor(fill_color,true_alpha));
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
		applyColor(poColor(stroke_color, true_alpha));
		
		// use standard OpenGL stroke or custom generated stroke
		if ( !use_simple_stroke )
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
	
	if(draw_bounds) {
		applyColor(poColor::red);
		drawStroke(bounds());
		drawRect(poRect(-offset()-poPoint(2,2), poPoint(4,4)));
	}
}


poShape2D& poShape2D::addPoint(poPoint p) {
	if(points.empty())
		bounds(poRect(p,poPoint()));
	else
		bounds(bounds().include(p));
	
	points.push_back(p);
	return *this;
}

poShape2D& poShape2D::addPoint( float x, float y ) {
	addPoint( poPoint(x,y) );
	return *this;
}

poShape2D& poShape2D::addPoints(const std::vector<poPoint> &pts) {
	std::for_each(pts.begin(), pts.end(), boost::bind(&poShape2D::addPoint, this, _1));
	return *this;
}

poShape2D& poShape2D::curveTo(poPoint pt, poPoint control, int resolution) {
	if(points.empty()) {
		addPoint(poPoint(0,0,0));
	}
	
	std::vector<poPoint> pts = quadTo(points.back(), pt, control, resolution);
	addPoints(pts);
	return *this;
}

poShape2D& poShape2D::curveTo(poPoint pt, poPoint control1, poPoint control2, int resolution) {
	if(points.empty())
		addPoint(poPoint(0,0,0));
	
	std::vector<poPoint> pts = cubeTo(points.back(), pt, control1, control2, resolution);
	addPoints(pts);
	return *this;
}

const std::vector<poPoint> &poShape2D::getPoints() {
	return points;
}

poShape2D &poShape2D::setPoints(const std::vector<poPoint> &pts) {
	clearPoints();
	addPoints(pts);
	return *this;
}

poShape2D& poShape2D::clearPoints() {
	points.clear();
	return *this;
}

size_t poShape2D::numPoints() const {
	return points.size();
}

poPoint poShape2D::getPoint(int idx) {
	return points[idx];
}

bool poShape2D::setPoint(int idx, poPoint p )
{
	if ( idx < 0 || idx >= numPoints() )
		return false;
	points[idx] = p;
	return true;
}

poPoint poShape2D::getTexCoord(int idx, uint unit) {
	return tex_coords[unit][idx];
}

poColor poShape2D::getColor(int idx) {
	return colors[idx];
}

void fitNone(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / tex->width() * tex->s();
		float t = points[i].y / tex->height() * tex->t();
		coords[i].set(s,t,0.f);
	}
}

void fitExact(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width() * tex->s();
		float t = points[i].y / rect.height() * tex->t();
		coords[i].set(s,t,0.f);
	}
}

void fitHorizontal(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float asp = rect.width() / tex->width();
	float diff = rect.height() - tex->height()*asp;
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width() * tex->s();
		float t = (points[i].y-diff/2.f) / rect.height() * (tex->t() / asp);
		coords[i].set(s,t,0.f);
	}
}

void fitVertical(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	float asp = rect.height() / tex->height();
	float diff = rect.width() - tex->width()*asp;
	for(int i=0; i<points.size(); i++) {
		float s = (points[i].x-diff/2.f) / rect.width() * (tex->s() / asp);
		float t = points[i].y / rect.height() * tex->t(); 
		coords[i].set(s,t,0.f);
	}
}

poShape2D& poShape2D::placeTexture(poTexture *tex, uint unit) {
	return placeTexture(tex, PO_TEX_FIT_NONE, unit);
}

poShape2D& poShape2D::placeTexture(poTexture *tex, poTextureFitOption fit, uint unit) {
	if(textures[unit])
		delete resources.remove(textures[unit]);
	
	textures[unit] = resources.add(tex->copy());
	
	if(tex) {
		poRect rect = calculateBounds();
		enableAttribute(ATTRIB_TEX_COORD);
		tex_coords[unit].resize(points.size());
		
		switch(fit) {
			case PO_TEX_FIT_NONE:
				fitNone(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_EXACT:
				fitExact(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_H:
				fitHorizontal(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_V:
				fitVertical(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_MIN:
				if(tex->width() < tex->height())
					fitHorizontal(rect, tex, tex_coords[unit], points);
				else if(tex->height() < tex->width())
					fitVertical(rect, tex, tex_coords[unit], points);
				else
					fitExact(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_MAX:
				if(tex->width() > tex->height())
					fitHorizontal(rect, tex, tex_coords[unit], points);
				else if(tex->height() > tex->width())
					fitVertical(rect, tex, tex_coords[unit], points);
				else
					fitExact(rect, tex, tex_coords[unit], points);
				break;
		}
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

poShape2D& poShape2D::placeImage(poImage *img, uint unit) {
	poTexture tex(img);
	placeTexture(&tex, unit);
}

poShape2D& poShape2D::placeImage(poImage *img, poTextureFitOption fit, uint unit) {
	poTexture tex(img);
	placeTexture(&tex, fit, unit);
}

poShape2D&  poShape2D::enableFill(bool b) {enable_fill = b; return *this;}
bool        poShape2D::isFillEnabled() const {return enable_fill;}

poShape2D&  poShape2D::enableStroke(bool b) {enable_stroke = b; return *this;}
bool        poShape2D::isStrokeEnabled() const {return enable_stroke;}

int         poShape2D::strokeWidth() const {return stroke_width;}

poShape2D&  poShape2D::fillColor(poColor c) {fill_color = c; return *this; }
poShape2D&  poShape2D::fillColor( float r, float g, float b, float a ) { fill_color.set(r,g,b,a); return *this; };
poColor     poShape2D::fillColor() const {return fill_color;}

poColor     poShape2D::strokeColor() const {return stroke_color;}
poShape2D&  poShape2D::strokeColor( float r, float g, float b, float a ) { stroke_color.set(r,g,b,a); return *this; };
poShape2D&  poShape2D::strokeColor(poColor c) {stroke_color = c; return *this; }

poShape2D&  poShape2D::fillDrawStyle(GLenum e) {fill_draw_style = e; return *this; }
GLenum      poShape2D::fillDrawStyle() const {return fill_draw_style;}

poShape2D&  poShape2D::useSimpleStroke( bool b ) { use_simple_stroke = b; return *this; };
bool        poShape2D::isSimpleStrokeEnabled() const { return use_simple_stroke; };

poShape2D&  poShape2D::enableAttribute(VertexAttribute a) {enabled_attributes |= a; return *this; }
poShape2D&  poShape2D::disableAttribute(VertexAttribute a) {enabled_attributes &= ~a; return *this; }
bool        poShape2D::isAttributeEnabled(VertexAttribute a) const {return (enabled_attributes & a) != 0;}

StrokeCapProperty poShape2D::capStyle() const {return cap;}

StrokeJoinProperty poShape2D::joinStyle() const {return join;}

poShape2D&  poShape2D::closed(bool b) {closed_ = b; return *this; }
bool        poShape2D::isClosed() const {return closed_;}

poShape2D&	poShape2D::drawBounds(bool b) {draw_bounds = b;}
bool		poShape2D::drawBounds() const {return draw_bounds;}

GLenum      poShape2D::textureCombineFunction(uint unit) const {return tex_combo_func[unit];}
poShape2D&  poShape2D::textureCombineFunction(GLenum func, uint unit) {tex_combo_func[unit] = func; return *this; }
poTexture const* poShape2D::texture(uint unit) const {return textures[unit];}

bool		poShape2D::alphaTestTextures() const {return alpha_test_textures;}
poShape2D&	poShape2D::alphaTestTextures(bool b) {alpha_test_textures = b; return *this;}

void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, StrokeJoinProperty join, float stroke_width);

poShape2D& poShape2D::generateStroke(int strokeWidth, StrokeJoinProperty join, StrokeCapProperty cap) {
	stroke_width = strokeWidth;
	this->cap = cap;
	this->join = join;
	
	enable_stroke = stroke_width > 0;
	if(!enable_stroke)
		return *this;
	
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
	
	return *this;
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

// localize will convert global to local first
// otherwise, point is assumed to be local
bool        poShape2D::pointInside(poPoint point, bool localize )
{
	if(!visible())
		return false;
	
	if(localize)
		point = globalToLocal(point);
	
	if(alpha_test_textures && isAttributeEnabled(ATTRIB_TEX_COORD)) {
		bool opaque = false;
		for(int i=0; i<MAX_TEXTURE_UNITS; i++) {
			if(textures[i]) {
				opaque |= textures[i]->opaqueAtPoint(point);
			}
		}
		return opaque;
	}
	
	// test point inside for given drawstyle
	if ( (fill_draw_style == GL_POLYGON || fill_draw_style == GL_TRIANGLE_FAN) && points.size() >= 3 )
	{
		for( int i=1; i<points.size()-1; i++ )
			if ( pointInTriangle( point, points[0], points[i], points[i+1] ) )
				return true;
		if (fill_draw_style == GL_TRIANGLE_FAN)
			if ( pointInTriangle( point, points[0], points[1], points.back() ))
				return true;
	}
	else if (fill_draw_style == GL_TRIANGLE_STRIP && points.size() >= 3 )
	{
		for( int i=0; i<points.size()-2; i++ )
			if ( pointInTriangle( point, points[i], points[i+1], points[i+2] ) )
				return true;
	}
	
	return false;
}

void poShape2D::updateAllTweens() {
	poObject::updateAllTweens();
	fill_color_tween.update();
}



