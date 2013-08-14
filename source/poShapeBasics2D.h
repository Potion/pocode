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

//
//  poShapeBasics2D.h
//  pocode
//
//  Created by Jared Schiffman on 5/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//
#pragma once


#include "poShape2D.h"
#include "poTexture.h"


// FILE NOTES
//
// All of the classes in this file derive from Shape2D, which derives from po::Object.


// CLASS NOTES (RectShape)
//
// RectShape is one of the most commonly used classes in pocode.
//
// A simple RectShape can be constructed as follows:
//      RectShape* S = new RectShape( 100,200 );
//
// A RectShape can also be constructed around an image as follows:
//      RectShape* S = new RectShape("myImage.jpg");
// This will load the image, generate the image texture, create a RectShape equal to the image size,
// and assign the texture to the shape.
//
// A RectShape may also have rounded corners:
//      RectShape* S = new RectShape( 100,200,10 );

namespace po {
    class RectShape : public Shape2D {
    public:
        RectShape();
        // rounded rect shape
        RectShape( float width, float height, float rad=0.f );
        // place an already-made texture
        RectShape( Texture *tex, TextureFitOption fit=po::TEX_FIT_NONE, Alignment align=po::ALIGN_TOP_LEFT );
        // make a texture from the specified filepath and place it
        RectShape( const FilePath &filePath, bool keepImage=false, TextureFitOption fit=po::TEX_FIT_NONE, Alignment align=po::ALIGN_TOP_LEFT);
        // make a texture from the specified url and place it
        RectShape( const URL &url, bool keepImage, TextureFitOption fit=po::TEX_FIT_NONE, Alignment align=po::ALIGN_TOP_LEFT);
        // make a shape of a certain size and put the texture in place inside it
        RectShape( float width, float height, Texture *tex, TextureFitOption fit=po::TEX_FIT_NONE, Alignment align=po::ALIGN_TOP_LEFT );
        
        virtual Object* copy();
        
        void	reshape( float width, float height, float rad=0.f );
        void	reshape( Rect r, float rad=0.f );
        
        void	setCornerRadius(float rad);
        float	getCornerRadius() const;
        
        // for alpha test
        bool doesAlphaTest() const;
        void setAlphaTest(bool b);
        
        bool pointInside(po::Point point, bool localize=false);
        
    protected:
        void	clone(RectShape* shp);
        void	construct( float _width, float _height, float rad );
        
        float	radius;
        
        //Do we need this? Can 
        po::Texture     *texture;
        bool            alphaTest;
    };


    // CLASS NOTES (OvalShape)
    //
    // An OvalShape is a Shape2D in the form of an oval or circle, and may also be used to form regular polygons.
    //
    // When an OvalShape is constructed, the third argument specifies the number of points to be used.
    // For a smooth oval or circle, this should generally be about 1/3 the width or height of the shape.
    // For example, a 100x100 circle, should use approximately 33 points.
    //
    // Regular polygons (triangles, squares, pentagons) can be constructed simply by using a small number of points
    //
    // A simple circular OvalShape can be constructed as follows:
    //      OvalShape* S = new OvalShape( 100,100,33 );
    //
    // An non-circular OvalShape can be constructed as follows:
    //      OvalShape* S = new OvalShape( 100,200,33 );


    class OvalShape : public Shape2D {
    public:
        OvalShape();
        OvalShape( float radius );
        OvalShape( float _width, float _height, int numPoints );
        virtual ~OvalShape() {}
        
        virtual Object *copy();
        
        void    reshape( float _width, float _height, int numPoints );
        
    protected:
        void	clone(OvalShape *shp);
        void    construct( float _width, float _height, int numPoints );
        
        float   width, height;
    };


    // CLASS NOTES (LineShape)
    //
    // A LineShape is a Shape2D in the form of a single line segment.
    // 
    // It is not recommenended to use LineShape for drawing 100's or 1000's of lines. A much more
    // efficient option is to use a single Shape2D and set the fillDrawStyle to GL_LINES. In that case,
    // each pair of points added will form an individual line segment.
    //
    // A simple LineShape can be constructed as follows:
    //      LineShape* S = new LineShape( Point(0,0), Point(100,100) );

    class LineShape : public Shape2D {
    public:
        LineShape();
        LineShape( Point A, Point B );
        virtual ~LineShape() {}
        
        void    reshape( Point A, Point B );
        
    protected:
        void    construct( Point A, Point B );
    };


    // CLASS NOTES (StarShape)
    //
    // A StarShape is a Shape2D in the form of a star.
    //
    // A simple StarShape can be constructed as follows:
    //      StarShape* S = new StarShape( 100, 5, 150 );

    class StarShape : public Shape2D {
    public:
        StarShape();
        StarShape( float _outerRadius, int numPoints, float depth );
        StarShape( float _width, float _height, int numPoints, float depth );
        
        virtual Object *copy();
        
        void	reshape( float _outerRadius, int numPoints, float depth );
        void	reshape( float _width, float _height, int numPoints, float depth );
        
    protected:
        void	clone(StarShape* shp);
        
        void	construct( float _outerRadius, int numPoints, float depth );
        void	construct( float _width, float _height, int numPoints, float depth );
        
        float	width, height, depth;
        float	outerRadius, innerRadius;
    };
}