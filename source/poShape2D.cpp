#include "poShape2D.h"
#include "poHelpers.h"
#include "poLineExtruder.h"
#include "nanosvg.h"
#include "poApplication.h"
#include "poResourceStore.h"
#include "poCamera.h"

#include "poOpenGLState.h"
#include "poBasicRenderer.h"

poShape2D::poShape2D()
:	fillEnabled(true)
,	strokeEnabled(false)
,	strokeWidth(1)
,	fillColor(1,1,1,1)
,	strokeColor(1,1,1,1)
,   useSimpleStroke(true)
,	fillDrawStyle(GL_TRIANGLE_FAN)
,	closed(true)
,	texture(NULL)
,	fillColorTween(&fillColor)
//,	alphaTestTexture(false)
{}

poObject* poShape2D::copy() {
	poShape2D *shp = new poShape2D();
	clone(shp);
	return shp;
}

void poShape2D::clone(poShape2D *shp) {
	shp->fillDrawStyle = fillDrawStyle;
	shp->fillColor = fillColor;
	shp->strokeColor = strokeColor;
	shp->fillEnabled = fillEnabled;
	shp->strokeEnabled = strokeEnabled;
	shp->strokeWidth = strokeWidth;
	shp->useSimpleStroke = useSimpleStroke;
	shp->closed = closed;
	shp->fillColorTween = fillColorTween;
	shp->points = points;
	shp->texCoords = texCoords;
	shp->stroke = stroke;
	shp->texture = texture;
	shp->cap = cap;
	shp->join = join;
	poObject::clone(shp);
}

void poShape2D::draw() {
	// do shape fill
	if ( fillEnabled ) {
		// set the color
		po::setColor(fillColor, getAppliedAlpha());

		if(texture && texture->isValid()) {
			po::drawPoints(points, texture, texCoords, fillDrawStyle);
		}
		else {
			po::drawPoints(points, fillDrawStyle);
		}
	}
	
	// do shape stroke
	if(strokeEnabled) {
		po::setColor(strokeColor, getAppliedAlpha());

		if(useSimpleStroke) {
			// use crappy OpenGL stroke
			glLineWidth( strokeWidth );
			po::drawPoints(points, closed ? GL_LINE_LOOP : GL_LINE_STRIP);
		}
		else {
			po::drawPoints(stroke, GL_TRIANGLE_STRIP);
		}
	}	
}


poShape2D& poShape2D::addPoint(poPoint p) {

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

size_t poShape2D::getNumPoints() const {
	return points.size();
}

poPoint poShape2D::getPoint(int idx) {
	return points[idx];
}

bool poShape2D::setPoint(int idx, poPoint p ) {
	if ( idx < 0 || idx >= getNumPoints() )
		return false;
	points[idx] = p;
	return true;
}

poShape2D& poShape2D::placeTexture(poTexture *tex) {
	return placeTexture(tex, PO_TEX_FIT_NONE, PO_ALIGN_CENTER_CENTER);
}

poShape2D& poShape2D::placeTexture(poTexture *tex, poTextureFitOption fit) {
	return placeTexture(tex, fit, PO_ALIGN_CENTER_CENTER);
}

poShape2D& poShape2D::placeTexture(poTexture *tex, poTextureFitOption fit, poAlignment align) {
	if(tex && tex->isValid()) {
		poRect rect = getBounds();
		
		texCoords.clear();
		texCoords.resize(points.size());
		textureFit(rect, tex, fit, align, texCoords, points);
	}
	
	texture = tex;
	return *this;
}

poTexture* poShape2D::getTexture() {
    return texture;
}

void poShape2D::removeTexture(bool andDelete) {
    if(andDelete && texture) delete texture;
    texture = NULL;
}

poShape2D& poShape2D::transformTexture(poPoint pt, poPoint scale, float rotate) {
	return *this;
}

void poShape2D::setStrokeWidth(int width) {
	strokeWidth = (width > 0) ? width : 0;
}

int poShape2D::getStrokeWidth() const {
	return strokeWidth;
}

poStrokeCapProperty poShape2D::capStyle() const {
	return cap;
}

poStrokeJoinProperty poShape2D::joinStyle() const {
	return join;
}

poShape2D& poShape2D::generateStroke(int strokeWidth, poStrokePlacementProperty place, poStrokeJoinProperty join, poStrokeCapProperty cap) {
    useSimpleStroke = false;
	this->strokeWidth = strokeWidth;
	this->cap = cap;
	this->join = join;
	
	strokeEnabled = strokeWidth > 0;
	if(!strokeEnabled)
		return *this;
	
	stroke.clear();
	
	if(strokeEnabled) {
		std::vector<poExtrudedLineSeg> segments;
		
		poPoint p1, p2, p3, p4, tmp;
		
		for(int i=0; i<points.size()-1; i++) {
			p1 = points[i];
			p2 = points[i+1];
			segments.push_back(poExtrudedLineSeg(p1, p2, strokeWidth, place));
		}
		
		if(closed) {
			segments.push_back(poExtrudedLineSeg(points.back(), points.front(), strokeWidth, place));
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, strokeWidth);
		}	
		else {
			// add the first cap
			stroke.push_back(segments[0].p2);
			stroke.push_back(segments[0].p1);
		}
		
		// generate middle points
		for(int i=0; i<segments.size()-1; i++) {
			makeStrokeForJoint(stroke, segments[i], segments[i+1], join, strokeWidth);
		}
		
		if(closed) {
			makeStrokeForJoint(stroke, segments.back(), segments.front(), join, strokeWidth);
		}
		else {
			stroke.push_back(segments.back().p4);
			stroke.push_back(segments.back().p3);
		}
	}
	
	return *this;
}

// localize will convert global to local first
// otherwise, point is assumed to be local
bool poShape2D::pointInside(poPoint point, bool localize ) {
	if(!visible)
		return false;
	
    // DO POINT INSIDE TEST FOR 2D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_2D )
    {
        if(localize) {
            point.y = getWindowHeight() - point.y;
            point = globalToLocal(point);
        }
        
        // test point inside for given drawstyle
        if ( fillDrawStyle == GL_TRIANGLE_FAN && points.size() >= 3 ) {
            for( int i=1; i<points.size()-1; i++ )
                if ( pointInTriangle( point, points[0], points[i], points[i+1] ) )
                    return true;
            if (fillDrawStyle == GL_TRIANGLE_FAN)
                if ( pointInTriangle( point, points[0], points[1], points.back() ))
                    return true;
        }
        else if (fillDrawStyle == GL_TRIANGLE_STRIP && points.size() >= 3 ) {
            for( int i=0; i<points.size()-2; i++ )
                if ( pointInTriangle( point, points[i], points[i+1], points[i+2] ) )
                    return true;
        }
    }
	
    // DO POINT INSIDE TEST FOR 3D
    if ( poCamera::getCurrentCameraType() == PO_CAMERA_3D )
    {
        if(localize) {
            point.y = getWindowHeight() - point.y;
        }
        
        // test point inside for given drawstyle
        if ( fillDrawStyle == GL_TRIANGLE_FAN && points.size() >= 3 ) {
            for( int i=1; i<points.size()-1; i++ )
                if ( pointInTriangle3D( point, getMatrixSet(), points[0], points[i], points[i+1] ) )
                    return true;
            if (fillDrawStyle == GL_TRIANGLE_FAN)
                if ( pointInTriangle3D( point, getMatrixSet(), points[0], points[1], points.back() ))
                    return true;
        }
        else if (fillDrawStyle == GL_TRIANGLE_STRIP && points.size() >= 3 ) {
            for( int i=0; i<points.size()-2; i++ )
                if ( pointInTriangle3D( point, getMatrixSet(), points[i], points[i+1], points[i+2] ) )
                    return true;
        }
    }
    
	return false;
}


poRect  poShape2D::getBounds()
{
    poRect rect;
    
    // must initialize rect with first point
    if ( points.size() > 0 )
        rect.set( points[0].x, points[0].y, 0, 0 );
    
    // include all other points
    BOOST_FOREACH(poPoint &p, points) {
        rect.include(p);
    }
    
    return rect;
}

void poShape2D::stopAllTweens(bool recurse) {
	poObject::stopAllTweens(recurse);
	fillColorTween.stop();
}

void poShape2D::updateAllTweens() {
	poObject::updateAllTweens();
	fillColorTween.update();
}

void poShape2D::read(poXMLNode node) {
	fillDrawStyle = node.getChild("fillDrawStyle").getInnerInt();
	fillColor.set(node.getChild("fillColor").getInnerString());
	strokeColor.set(node.getChild("strokeColor").getInnerString());
	fillEnabled = node.getChild("fillEnabled").getInnerInt();
	strokeEnabled = node.getChild("strokeEnabled").getInnerInt();
	useSimpleStroke = node.getChild("useSimpleStroke").getInnerInt();
	closed = node.getChild("closed").getInnerInt();
	drawBounds = node.getChild("drawBounds").getInnerInt();
//	alphaTestTexture = node.getChild("alphaTestTexture").innerInt();
	cap = poStrokeCapProperty(node.getChild("cap").getInnerInt());
	join = poStrokeJoinProperty(node.getChild("join").getInnerInt());
	strokeWidth = node.getChild("stroke_width").getInnerInt();

	std::string pstr = node.getChild("points").getInnerString();
	std::string str = base64_decode(pstr);
	
	points.resize(str.size() / sizeof(poPoint));
	memcpy(&points[0],str.c_str(),str.size());
	
//	poXMLNode tex = node.getChild("texture");
//	if(tex) {
//		std::string url = tex.stringAttribute("url");
//		texture = getImage(url)->texture();
//		str = base64_decode(node.getChild("tex_coords").innerString());
//		tex_coords.resize(str.size() / sizeof(poPoint));
//		memcpy(&tex_coords[0],str.c_str(),str.size());
//	}

	poObject::read(node);
	generateStroke(strokeWidth);
}

void poShape2D::write(poXMLNode &node) {
	node.addChild("fillDrawStyle").setInnerInt(fillDrawStyle);
	node.addChild("fillColor").setInnerString(fillColor.toString());
	node.addChild("strokeColor").setInnerString(strokeColor.toString());
	node.addChild("fillEnabled").setInnerInt(fillEnabled);
	node.addChild("strokeEnabled").setInnerInt(strokeEnabled);
	node.addChild("useSimpleStroke").setInnerInt(useSimpleStroke);
	node.addChild("closed").setInnerInt(closed);
	node.addChild("drawBounds").setInnerInt(drawBounds);
//	node.addChild("alphaTestTexture").setInnerInt(alphaTestTexture);
	node.addChild("cap").setInnerInt(cap);
	node.addChild("join").setInnerInt(join);
	node.addChild("stroke_width").setInnerInt(strokeWidth);

	int points_sz = sizeof(poPoint)*points.size();
	const unsigned char *points_ptr = (const unsigned char*)&points[0];
	node.addChild("points").getHandle().append_child(pugi::node_cdata).set_value(base64_encode(points_ptr, points_sz).c_str());
		
//	if(texture && texture->image() && texture->image()->isValid()) {
//		poXMLNode tex = node.addChild("texture");
//		// this is only going to work in the most simple case
//		tex.addAttribute("url",texture->image()->url());
//		
//		// write points out as binary
//		points_sz = sizeof(poPoint)*tex_coords.size();
//		points_ptr = (const unsigned char*)&tex_coords[0];
//		tex.handle().append_child(pugi::node_cdata).set_value(base64_encode(points_ptr, points_sz).c_str());
//	}
	
	poObject::write(node);
	node.setAttribute("type","poShape2D");
}

std::vector<poShape2D*> createShapesFromSVGfile(const fs::path &svg) {
	std::vector<poShape2D*> response;

	if(!fs::exists(svg)) {
		log("poShape2D: svg file doesn't exist (%s)", svg.string().c_str());
		return response;
	}
	
	SVGPath *result = svgParseFromFile(svg.string().c_str());
	if(!result) {
		log("poShape2D: invalid svg file (%s)", svg.string().c_str());
		return response;
	}
	
	while(result) {
		poShape2D *shape = new poShape2D();
		
		for(int i=0; i<result->npts; i++) {
			shape->addPoint(result->pts[i*2], result->pts[i*2+1]);
		}
		shape->closed = result->closed;
		
		shape->fillEnabled = result->hasFill;
		shape->strokeEnabled = result->hasStroke;
		
		if(shape->fillEnabled) {
			shape->fillColor = poColor().set255((result->fillColor>>16)&0xFF, (result->fillColor>>8)&0xFF, result->fillColor&0xFF);
		}
		
		if(shape->strokeEnabled) {
			shape->generateStroke(result->strokeWidth);
			shape->strokeColor = poColor().set255((result->strokeColor>>16)&0xFF, (result->strokeColor>>8)&0xFF, result->strokeColor&0xFF);
		}
		
		response.push_back(shape);
		result = result->next;
	}
	
	svgDelete(result);
	return response;
}


