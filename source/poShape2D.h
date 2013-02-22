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

#pragma once


#include "poObject.h"
#include "poTexture.h"
#include "poSimpleDrawing.h"


// CLASS NOTES
//
// po::Shape2D derives directly from po::Object and is the parent class of all 2D graphics objects
// in pocode, including RectShape, poOvalShape and poLineShape (see "poShapeBasics2D.h").
//
// All Shape2D objects and Shape2D subclasses:
//
// + Have all the properties and methods of po::Object.
// + Have a list of "points" that form the contour of the shape.
// + Have fillColor and strokeColor properties
// + Have fillEnabled and strokeEnabled properties.
// + Have a fillDrawStyle property that determines how OpenGL uses the points (GL_TRIANGLE_STRIP, GL_POLYGON, etc)
// + Have a generateStroke method for generating a high-quality thick stroke with multiple capping options.
// + Have a placeTexture and and transformTexture methods for assigning a Texture to the shape
// + Have a pointInside method for testing if a point is inside the shape.

namespace po {
    class Shape2D : public Object {
    public:
        Shape2D();
        virtual Object*		copy();

        // DRAWING
        // The draw() method is called automatically if a shape is added to the scene graph.
        // You should not need to call the draw() method yourself.
        virtual void            draw();

        // SHAPE POINTS
        // Shape2D maintains a list of points that define the contour the shape.
        // Points can be added one by one, or can be added as vector of points.
        // The "curveTo" method generates a curve formed by adding many points.
        Shape2D&				addPoint(Point p);
        Shape2D&				addPoint( float x, float y );
        Shape2D&				addPoints(const std::vector<Point> &points);
        Shape2D&				curveTo(Point pt, Point control, int resolution=10);
        Shape2D&				curveTo(Point pt, Point control1, Point control2, int resolution=10);
        
        Shape2D&				setPoints(const std::vector<Point> &points);
        Shape2D&				removeLastPoint();
        Shape2D&				clearPoints();

        size_t                  getNumPoints() const;
        Point                   getPoint(int idx);
        bool                    setPoint(int idx, Point p );
        const std::vector<Point> &getPoints();
        
        // SHAPE TEXTURE
        // A Shape2D can be assigned a Texture.
        // By default, the texture will be placed within the shape at actual size. This means that
        // the texture image may extend beyond the bounds of the shape or may not be large enough
        // to fill the shape. Use the TextureFitOption's to determine how a texture is placed
        // into the shape. These TextureFitOption's are listed in poEnums.h. Once a texture is placed,
        // you can use transform the placement, rotation and scale of the texture.
        Shape2D&                placeTexture(Texture *tex);
        Shape2D&                placeTexture(Texture *tex, TextureFitOption fit);
        Shape2D&                placeTexture(Texture *tex, TextureFitOption fit, Alignment align);
        Shape2D&                setTextureCoords(const std::vector<Point> &texCrds );
        Texture*                getTexture();
        void                    removeTexture(bool andDelete = false);
        Shape2D&                transformTexture(Point pt, Point scale, float rotate);
        
        // HIGH QUALITY SHAPE STROKE
        // By default, Shape2D's use a simple OpenGL stroke. Unfortunately, when the stroke width
        // is greater than 3 pixels, the OpenGL stroke appear broken at every corner. As a solution,
        // Shape2D includes a "generateStroke" method that creates a very high quality stroke.
        // This stroke is actually drawn as a very thin filled shape. These high quality strokes
        // scale with the shape, unlike simple strokes which do not scale.
        Shape2D&                generateStroke(int strokeWidth, StrokePlacementProperty place=po::STROKE_PLACE_CENTER, 
                                               StrokeJoinProperty join=po::STROKE_JOIN_MITRE, StrokeCapProperty cap=po::STROKE_CAP_BUTT);
        StrokeCapProperty       capStyle() const;
        StrokeJoinProperty      joinStyle() const;
        
        void                    setStrokeWidth(int width);
        int                     getStrokeWidth() const;

        // POINT INSIDE
        // pointInside assumes points are in window-native coordinates (0,0 is in the upper left).
        // The localize option will convert the point from global to local coordinates before
        // performing the test.
        virtual bool            pointInside(Point point, bool localize=false);
        virtual Rect            getBounds();
        
        // TWEEN MANAGEMENT
        virtual void            stopAllTweens(bool recurse=false);
        
        // MEMORY MANAGEMENT
        virtual int             getSizeInMemory();
        
        // SHAPE SERIALIZAATION
        virtual void            read(XMLNode node);
        virtual void            write(XMLNode &node);
        
        // SHAPE PROPERTIES (DIRECTLY ACCCESSIBLE)
        GLenum                  fillDrawStyle;
        Color                   fillColor;
        Color                   strokeColor;
        bool                    fillEnabled;
        bool                    strokeEnabled;
        bool                    useSimpleStroke;
        bool                    closed;
        
        // SHAPE SPECIFIC TWEEN
        // In addition to the five tweens in po::Object, Shape2D has a special tween for the fillColor.
        Tween<Color>            fillColorTween;


    protected:
        virtual void            updateAllTweens();
        void					clone(Shape2D *shp);
        void					setTexture(Texture* t);
    
    private:
        // SHAPE PROPERTIES (PRIVATE)
        std::vector<Point>    points;
        std::vector<Point>    texCoords;
        std::vector<Point>    stroke;

        Texture*              texture;

        StrokeCapProperty     cap;
        StrokeJoinProperty    join;
        int                   strokeWidth;
    };

    // SHAPE GENERATOR FROM SVG FILE
    std::vector<Shape2D*> createShapesFromSVGfile(const fs::path &svg);
} /* End po namespace */
