#include "poShape2D.h"
#include "Helpers.h"
#include "LineExtruder.h"
//#include "nanosvg.h"

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

		// set the color
		applyColor(poColor(fill_color,appliedAlpha()));
		// actually draw the filled shape
		glDrawArrays(fill_draw_style, 0, (int)points.size());
		
		// disable textures
		if(isAttributeEnabled(ATTRIB_TEX_COORD)) {
			for(int i=0; i<MAX_TEXTURE_UNITS; i++) {
				if(textures[i]) {
					glActiveTexture(GL_TEXTURE0+i);
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
		applyColor(poColor(stroke_color, appliedAlpha()));
		
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
		drawRect(poRect(-offset()-poPoint(5,5), poPoint(10,10)));
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

void fitNone(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points, poAlignment align) {
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / tex->width();
		float t = points[i].y / tex->height();

//		switch(align) {
//			case PO_ALIGN_BOTTOM_LEFT:
//				break;
//			case PO_ALIGN_BOTTOM_CENTER:
//				s = (1.f - s) / 2.f;
//				break;
//			case PO_ALIGN_BOTTOM_RIGHT:
//				s = 1.f - s;
//				break;
//			case PO_ALIGN_CENTER_LEFT:
//				t = (1.f - t) / 2.f;
//				break;
//			case PO_ALIGN_CENTER_CENTER:
//				s = (1.f - s) / 2.f;
//				t = (1.f - t) / 2.f;
//				break;
//			case PO_ALIGN_CENTER_RIGHT:
//				s = 1.f - s;
//				t = (1.f - t) / 2.f;
//				break;
//			case PO_ALIGN_TOP_LEFT:
//				t = 1.f - t;
//				break;
//			case PO_ALIGN_TOP_CENTER:
//				s = (1.f - s) / 2.f;
//				t = 1.f - t;
//				break;
//			case PO_ALIGN_TOP_RIGHT:
//				s = (1.f - s);
//				t = 1.f - t;
//				break;
//		}
		
		coords[i].set(s*tex->s(),t*tex->t(),0.f);
	}
}

void fitExact(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points) {
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width();
		float t = points[i].y / rect.height();
		coords[i].set(s*tex->s(),t*tex->t(),0.f);
	}
}

void fitHorizontal(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points, poAlignment align) {
	float h_mod = rect.height() / (float)tex->height();
	float scale = tex->width() / (float)tex->height();
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width();
		float t = points[i].y / rect.height();
		
		t *= h_mod * scale;
		
//		if(align >= PO_ALIGN_CENTER_LEFT && align <= PO_ALIGN_CENTER_RIGHT) {
//			t += t / 2.f;
//		}
//		else 
//		if(align >= PO_ALIGN_TOP_LEFT && align <= PO_ALIGN_TOP_RIGHT) {
//			t += 1.f - t;
//		}
		
		coords[i].set(s*tex->s(),t*tex->t(),0.f);
	}
}

void fitVertical(poRect rect, poTexture *tex, std::vector<poPoint> &coords, const std::vector<poPoint> &points, poAlignment align) {
	float w_mod = rect.width() / (float)tex->width();
	float scale = tex->height() / (float)tex->width();
	
	for(int i=0; i<points.size(); i++) {
		float s = points[i].x / rect.width();
		float t = points[i].y / rect.height();
		
		s *= w_mod * scale;
		
//		if(align && !(align % 2)) {
//			s += s / 2.f;
//		}
//		else 
//		if(align && !(align % 3)) {
//			s += 1.f - s;
//		}
		coords[i].set(s*tex->s(),t*tex->t(),0.f);
	}
}

poShape2D& poShape2D::placeTexture(poTexture *tex, uint unit) {
	return placeTexture(tex, PO_TEX_FIT_NONE, PO_ALIGN_CENTER_CENTER, unit);
}

poShape2D& poShape2D::placeTexture(const poTexture &tex, uint unit) {
	return placeTexture(tex, PO_TEX_FIT_NONE, PO_ALIGN_CENTER_CENTER, unit);
}

poShape2D& poShape2D::placeTexture(poTexture *tex, poTextureFitOption fit, uint unit) {
	return placeTexture(tex, fit, PO_ALIGN_CENTER_CENTER, unit);
}

poShape2D& poShape2D::placeTexture(const poTexture &tex, poTextureFitOption fit, uint unit) {
	return placeTexture(const_cast<poTexture*>(&tex), fit, PO_ALIGN_CENTER_CENTER, unit);
}

poShape2D& poShape2D::placeTexture(const poTexture &tex, poTextureFitOption fit, poAlignment align, uint unit) {
	return placeTexture(const_cast<poTexture*>(&tex), fit, align, unit);
}

poShape2D& poShape2D::placeTexture(poTexture *tex, poTextureFitOption fit, poAlignment align, uint unit) {
	if(textures[unit]) {
		delete resources.remove(textures[unit]);
		textures[unit] = NULL;
	}
	
	if(tex) {
		textures[unit] = resources.add(tex->copy());

		poRect rect = calculateBounds();
		enableAttribute(ATTRIB_TEX_COORD);
		
		tex_coords[unit].clear();
		tex_coords[unit].resize(points.size());
		
		switch(fit) {
			case PO_TEX_FIT_NONE:
				fitNone(rect, tex, tex_coords[unit], points, align);
				break;
				
			case PO_TEX_FIT_EXACT:
				fitExact(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_H:
				fitHorizontal(rect, tex, tex_coords[unit], points, align);
				break;
				
			case PO_TEX_FIT_V:
				fitVertical(rect, tex, tex_coords[unit], points, align);
				break;
				
			case PO_TEX_FIT_MIN:
				if(tex->width() < tex->height())
					fitHorizontal(rect, tex, tex_coords[unit], points, align);
				else if(tex->height() < tex->width())
					fitVertical(rect, tex, tex_coords[unit], points, align);
				else
					fitExact(rect, tex, tex_coords[unit], points);
				break;
				
			case PO_TEX_FIT_MAX:
				if(tex->width() > tex->height())
					fitHorizontal(rect, tex, tex_coords[unit], points, align);
				else if(tex->height() > tex->width())
					fitVertical(rect, tex, tex_coords[unit], points, align);
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
	
	return *this;
}

poShape2D& poShape2D::placeImage(poImage *img, uint unit) {
	poTexture tex(img);
	placeTexture(&tex, unit);
}

poShape2D& poShape2D::placeImage(poImage *img, poTextureFitOption fit, poAlignment align, uint unit) {
	poTexture tex(img);
	placeTexture(&tex, fit, align, unit);
}

poShape2D& poShape2D::transformTexture(poPoint pt, poPoint scale, float rotate, uint unit) {
	tex_transforms[unit].translate = -pt;
	tex_transforms[unit].scale = scale;
	tex_transforms[unit].rotate = rotate;
	tex_transforms[unit].in_use = true;
	return *this;
}

poShape2D& poShape2D::disableTransformTexture(uint unit) {
	tex_transforms[unit].in_use = false;
	return *this;
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

poShape2D&	poShape2D::drawBounds(bool b) {draw_bounds = b; return *this;}
bool		poShape2D::drawBounds() const {return draw_bounds;}

GLenum      poShape2D::textureCombineFunction(uint unit) const {return tex_combo_func[unit];}
poShape2D&  poShape2D::textureCombineFunction(GLenum func, uint unit) {tex_combo_func[unit] = func; return *this; }
poTexture const* poShape2D::texture(uint unit) const {return textures[unit];}

bool		poShape2D::alphaTestTextures() const {return alpha_test_textures;}
poShape2D&	poShape2D::alphaTestTextures(bool b) {alpha_test_textures = b; return *this;}

void makeStrokeForJoint(std::vector<poPoint> &stroke, poExtrudedLineSeg &seg1, poExtrudedLineSeg &seg2, StrokeJoinProperty join, float stroke_width);

poShape2D& poShape2D::generateStroke(int strokeWidth, StrokePlacementProperty place, StrokeJoinProperty join, StrokeCapProperty cap) {
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
			segments.push_back(poExtrudedLineSeg(p1, p2, stroke_width, place));
		}
		
		if(closed_) {
			segments.push_back(poExtrudedLineSeg(points.back(), points.front(), stroke_width, place));
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, stroke_width);
		}	
		else {
			// add the first cap
			stroke.push_back(segments[0].p2);
			stroke.push_back(segments[0].p1);
		}
		
		// generate middle points
		for(int i=0; i<segments.size()-1; i++) {
			makeStrokeForJoint(stroke, segments[i], segments[i+1], join, stroke_width);
		}
		
		if(closed_) {
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, stroke_width);
		}
		else {
			stroke.push_back(segments.back().p4);
			stroke.push_back(segments.back().p3);
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

std::vector<poShape2D*> createShapesFromSVGfile(const fs::path &svg) {
	std::vector<poShape2D*> response;

	/*if(!fs::exists(svg)) {
		log("poShape2D: svg file doesn't exist (%s)", svg.string().c_str());
		return;
	}
	
	SVGPath *result = svgParseFromFile(svg.string().c_str());
	if(!result) {
		log("poShape2D: invalid svg file (%s)", svg.string().c_str());
		return;
	}
	
	while(result) {
		poShape2D *shape = new poShape2D();
		
		for(int i=0; i<result->npts; i++) {
			shape->addPoint(result->pts[i*2], result->pts[i*2+1]);
		}
		shape->closed(result->closed);
		
		shape->enableFill(result->hasFill);
		shape->enableStroke(result->hasStroke);
		
		if(shape->isFillEnabled()) {
			shape->fillColor(poColor().set255((result->fillColor>>16)&0xFF, (result->fillColor>>8)&0xFF, result->fillColor&0xFF));
		}
		
		if(shape->isStrokeEnabled()) {
			shape->generateStroke(result->strokeWidth);
			shape->strokeColor(poColor().set255((result->strokeColor>>16)&0xFF, (result->strokeColor>>8)&0xFF, result->strokeColor&0xFF));
		}
		
		response.push_back(shape);
		result = result->next;
	}
	
	svgDelete(result);*/
	return response;
}


