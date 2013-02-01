/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poShape2D.h"
#include "poHelpers.h"
#include "poLineExtruder.h"
#include "poApplication.h"
#include "poResourceStore.h"
#include "poCamera.h"
#include "nanosvg.h"
#include "poOpenGLState.h"
#include <glm/gtc/type_ptr.hpp>

#include "poSimpleDrawing.h"

namespace po {
    Shape2D::Shape2D()
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
    
    
    //------------------------------------------------------------------------
    Object* Shape2D::copy() {
        Shape2D *shp = new Shape2D();
        clone(shp);
        return shp;

    }
    
    
    //------------------------------------------------------------------------
    void Shape2D::clone(Shape2D *shp) {
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
        Object::clone(shp);
    }
    
    
    //------------------------------------------------------------------------
    void Shape2D::draw() {
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
                po::setLineWidth( strokeWidth );
                po::drawPoints(points, closed ? GL_LINE_LOOP : GL_LINE_STRIP);
            }
            else {
                po::drawPoints(stroke, GL_TRIANGLE_STRIP);
            }
        }
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Points ============================================
    #pragma mark Points
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::addPoint(Point p) {
        points.push_back(p);
        setAlignment( getAlignment() );
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::addPoint( float x, float y ) {
        addPoint( Point(x,y) );
        return *this;
    }
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::removeLastPoint() {
        if(!points.empty())
            points.pop_back();
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::addPoints(const std::vector<Point> &pts) {
        std::for_each(pts.begin(), pts.end(), boost::bind(&Shape2D::addPoint, this, _1));
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D &Shape2D::setPoints(const std::vector<Point> &pts) {
        clearPoints();
        addPoints(pts);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    bool Shape2D::setPoint(int idx, Point p ) {
        if ( idx < 0 || idx >= getNumPoints() )
            return false;
        points[idx] = p;
        return true;
    }
    
    
    //------------------------------------------------------------------------
    size_t Shape2D::getNumPoints() const {
        return points.size();
    }
    
    
    //------------------------------------------------------------------------
    const std::vector<Point> &Shape2D::getPoints() {
        return points;
    }
    
    
    //------------------------------------------------------------------------
    Point Shape2D::getPoint(int idx) {
        return points[idx];
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::clearPoints() {
        points.clear();
        return *this;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Curves ============================================
    #pragma mark Curves
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::curveTo(Point pt, Point control, int resolution) {
        if(points.empty()) {
            addPoint(Point(0,0,0));
        }
        
        std::vector<Point> pts = po::quadTo(points.back(), pt, control, resolution);
        addPoints(pts);
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::curveTo(Point pt, Point control1, Point control2, int resolution) {
        if(points.empty())
            addPoint(Point(0,0,0));
        
        std::vector<Point> pts = po::cubeTo(points.back(), pt, control1, control2, resolution);
        addPoints(pts);
        return *this;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Textures ==========================================
    #pragma mark Textures
    
    //------------------------------------------------------------------------
    void Shape2D::setTexture(Texture* tex) {
        texture = tex;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::placeTexture(Texture *tex) {
        return placeTexture(tex, po::TEX_FIT_NONE, po::ALIGN_CENTER_CENTER);
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::placeTexture(Texture *tex, TextureFitOption fit) {
        return placeTexture(tex, fit, po::ALIGN_CENTER_CENTER);
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::placeTexture(Texture *tex, TextureFitOption fit, Alignment align) {
        if(tex && tex->isValid()) {
            Rect rect = getBounds();
            
            texCoords.clear();
            texCoords.resize(points.size());
            textureFit(rect, tex, fit, align, texCoords, points);
        }
        
        texture = tex;
        return *this;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D&  Shape2D::setTextureCoords(const std::vector<Point> &texCrds ) {
        if ( texCrds.size() != points.size() )
            printf("ERROR: mistmatch vector size in Shape2D::setTextureCoords\n");
        texCoords = texCrds;
        return* this;
    }
    
    
    //------------------------------------------------------------------------
    Texture* Shape2D::getTexture() {
        return texture;
    }
    
    
    //------------------------------------------------------------------------
    void Shape2D::removeTexture(bool andDelete) {
        if(andDelete && texture) {
            delete texture;
            texture = NULL;
        }
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::transformTexture(Point pt, Point scale, float rotate) {

        for( int i=0; i<texCoords.size(); i++ )
        {
            texCoords[i].x += pt.x;
            texCoords[i].y -= pt.y;
            
            texCoords[i].x *= scale.x;
            texCoords[i].y *= scale.y;
            
            if ( rotate > 0.01 || rotate < -0.01 )
                texCoords[i] = texCoords[i].getRotate2D(rotate);
        }
        return *this;
    }
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Lines/Strokes =====================================
    #pragma mark Lines/Strokes
    
    //------------------------------------------------------------------------
    void Shape2D::setStrokeWidth(int width) {
        strokeWidth = (width > 0) ? width : 0;
    }
    
    
    //------------------------------------------------------------------------
    int Shape2D::getStrokeWidth() const {
        return strokeWidth;
    }
    
    
    //------------------------------------------------------------------------
    StrokeCapProperty Shape2D::capStyle() const {
        return cap;
    }
    
    
    //------------------------------------------------------------------------
    StrokeJoinProperty Shape2D::joinStyle() const {
        return join;
    }
    
    
    //------------------------------------------------------------------------
    Shape2D& Shape2D::generateStroke(int strokeWidth, StrokePlacementProperty place, StrokeJoinProperty join, StrokeCapProperty cap) {
        useSimpleStroke = false;
        this->strokeWidth = strokeWidth;
        this->cap = cap;
        this->join = join;
        
        strokeEnabled = strokeWidth > 0;
        if(!strokeEnabled)
            return *this;
        
        stroke.clear();
        
        if(strokeEnabled) {
            std::vector<ExtrudedLineSeg> segments;
            
            Point p1, p2, p3, p4, tmp;
            
            for(int i=0; i<points.size()-1; i++) {
                p1 = points[i];
                p2 = points[i+1];
                segments.push_back(ExtrudedLineSeg(p1, p2, strokeWidth, place));
            }
            
            if(closed) {
                segments.push_back(ExtrudedLineSeg(points.back(), points.front(), strokeWidth, place));
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
    
    
    
    
    // ------------------------------------------------------------------------------------
    // =============================== Hit Testing ========================================
    #pragma mark Hit Testing
    
    //------------------------------------------------------------------------
    // localize will convert global to local first
    // otherwise, point is assumed to be local
    bool Shape2D::pointInside(Point point, bool localize ) {
        if(!visible)
            return false;
        
        // DO POINT INSIDE TEST FOR 2D
        if ( matrices.camType == po::CAMERA_2D )
        {
            if(localize) {
                //point.y = po::getWindowHeight() - point.y;
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
        if ( matrices.camType == po::CAMERA_3D )
        {
            if(localize) {
                point.y = po::getWindowHeight() - point.y;
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
    
    
    
    
    // -----------------------------------------------------------------------------------
    // =============================== Dimensions ========================================
    #pragma mark Dimensions
    
    //------------------------------------------------------------------------
    Rect  Shape2D::getBounds() {
        Rect rect;
        
        // must initialize rect with first point
        if ( points.size() > 0 )
            rect.set( points[0].x, points[0].y, 0, 0 );
        
        // include all other points
        BOOST_FOREACH(Point &p, points) {
            rect.include(p);
        }
        
        return rect;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Tweens ===========================================
    #pragma mark Tweens
    
    //------------------------------------------------------------------------
    void Shape2D::updateAllTweens() {
        Object::updateAllTweens();
        fillColorTween.update();
    }
    
    
    //------------------------------------------------------------------------
    void Shape2D::stopAllTweens(bool recurse) {
        Object::stopAllTweens(recurse);
        fillColorTween.stop();
    }
    
    
    //------------------------------------------------------------------------
    int Shape2D::getSizeInMemory() {
        int S = sizeof(Shape2D);

        S += points.capacity() * sizeof(Point);
        S += texCoords.capacity() * sizeof(Point);
        S += stroke.capacity() * sizeof(Point);
        
        return S;
    }
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Reading/Saving =====================================
    #pragma mark Reading/Saving
    
    //------------------------------------------------------------------------
    void Shape2D::read(XMLNode node) {
        fillDrawStyle = node.getChild("fillDrawStyle").getInnerInt();
        fillColor.set(node.getChild("fillColor").getInnerString());
        strokeColor.set(node.getChild("strokeColor").getInnerString());
        fillEnabled = node.getChild("fillEnabled").getInnerInt();
        strokeEnabled = node.getChild("strokeEnabled").getInnerInt();
        useSimpleStroke = node.getChild("useSimpleStroke").getInnerInt();
        closed = node.getChild("closed").getInnerInt();
        drawBounds = node.getChild("drawBounds").getInnerInt();
    //	alphaTestTexture = node.getChild("alphaTestTexture").innerInt();
        cap = StrokeCapProperty(node.getChild("cap").getInnerInt());
        join = StrokeJoinProperty(node.getChild("join").getInnerInt());
        strokeWidth = node.getChild("stroke_width").getInnerInt();

        std::string pstr = node.getChild("points").getInnerString();
        std::string str = po::base64_decode(pstr);
        
        points.resize(str.size() / sizeof(Point));
        memcpy(&points[0],str.c_str(),str.size());
        
    //	XMLNode tex = node.getChild("texture");
    //	if(tex) {
    //		std::string url = tex.stringAttribute("url");
    //		texture = getImage(url)->texture();
    //		str = base64_decode(node.getChild("tex_coords").innerString());
    //		tex_coords.resize(str.size() / sizeof(Point));
    //		memcpy(&tex_coords[0],str.c_str(),str.size());
    //	}

        Object::read(node);
        generateStroke(strokeWidth);
    }
    
    
    //------------------------------------------------------------------------
    void Shape2D::write(XMLNode &node) {
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

        int points_sz = sizeof(Point)*points.size();
        const unsigned char *points_ptr = (const unsigned char*)&points[0];
        node.addChild("points").getHandle().append_child(pugi::node_cdata).set_value(po::base64_encode(points_ptr, points_sz).c_str());
            
    //	if(texture && texture->image() && texture->image()->isValid()) {
    //		XMLNode tex = node.addChild("texture");
    //		// this is only going to work in the most simple case
    //		tex.addAttribute("url",texture->image()->url());
    //		
    //		// write points out as binary
    //		points_sz = sizeof(Point)*tex_coords.size();
    //		points_ptr = (const unsigned char*)&tex_coords[0];
    //		tex.handle().append_child(pugi::node_cdata).set_value(base64_encode(points_ptr, points_sz).c_str());
    //	}
        
        Object::write(node);
        node.setAttribute("type","Shape2D");
    }
    
    
    //------------------------------------------------------------------------
    std::vector<Shape2D*> createShapesFromSVGfile(const fs::path &svg) {

        std::vector<Shape2D*> response;

        #ifdef POTION_APPLE

            if(!fs::exists(svg)) {
                po::log("Shape2D: svg file doesn't exist (%s)", svg.string().c_str());
                return response;
            }
            
            SVGPath *result = svgParseFromFile(svg.string().c_str());
            if(!result) {
                po::log("Shape2D: invalid svg file (%s)", svg.string().c_str());
                return response;
            }
            
            while(result) {
                Shape2D *shape = new Shape2D();
                
                for(int i=0; i<result->npts; i++) {
                    shape->addPoint(result->pts[i*2], result->pts[i*2+1]);
                }
                shape->closed = result->closed;
                
                shape->fillEnabled = result->hasFill;
                shape->strokeEnabled = result->hasStroke;
                
                if(shape->fillEnabled) {
                    shape->fillColor = Color().set255((result->fillColor>>16)&0xFF, (result->fillColor>>8)&0xFF, result->fillColor&0xFF);
                }
                
                if(shape->strokeEnabled) {
                    shape->generateStroke(result->strokeWidth);
                    shape->strokeColor = Color().set255((result->strokeColor>>16)&0xFF, (result->strokeColor>>8)&0xFF, result->strokeColor&0xFF);
                }
                
                response.push_back(shape);
                result = result->next;
            }
            
            svgDelete(result);
        #endif

        return response;
    }
} /* End po namespace */

