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
<<<<<<< HEAD
// A RectShape may also have rounded corners:
//      RectShape* S = new RectShape( 100,200,10 );

namespace po {
    class RectShape : public Shape2D {
    public:
        RectShape();
        // rounded rect shape
        RectShape( float width, float height, float rad=0.f );
        // place an already-made texture
        RectShape( Texture *tex, TextureFitOption fit=PO_TEX_FIT_NONE, Alignment align=PO_ALIGN_TOP_LEFT );
        // make a texture from the specified url and place it
        RectShape( const FilePath &filePath, TextureFitOption fit=PO_TEX_FIT_NONE, Alignment align=PO_ALIGN_TOP_LEFT);
        // make a shape of a certain size and put the texture in place inside it
        RectShape( float width, float height, Texture *tex, TextureFitOption fit=PO_TEX_FIT_NONE, Alignment align=PO_ALIGN_TOP_LEFT );
        
        virtual Object* copy();
        
        void	reshape( float width, float height, float rad=0.f );
        void	reshape( Rect r, float rad=0.f );
        
        void	setCornerRadius(float rad);
        float	getCornerRadius() const;
        
    protected:
        void	clone(RectShape* shp);
        void	construct( float _width, float _height, float rad );
        
        float	radius;
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
} /* End po namespace */
=======
// A poRectShape may also have rounded corners:
//      poRectShape* S = new poRectShape( 100,200,10 );

class poRectShape : public poShape2D {
public:
	poRectShape();
	// rounded rect shape
	poRectShape( float width, float height, float rad=0.f );
	// place an already-made texture
	poRectShape( poTexture *tex, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT );
	// make a texture from the specified url and place it
	poRectShape( const poFilePath &filePath, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT);
	// make a shape of a certain size and put the texture in place inside it
	poRectShape( float width, float height, poTexture *tex, poTextureFitOption fit=PO_TEX_FIT_NONE, poAlignment align=PO_ALIGN_TOP_LEFT );
	
	virtual poObject* copy();
	
    void	reshape( float width, float height, float rad=0.f );
	void	reshape( poRect r, float rad=0.f );
	
	void	setCornerRadius(float rad);
	float	getCornerRadius() const;
    
protected:
	void	clone(poRectShape* shp);
    void	construct( float _width, float _height, float rad );
    
    float	radius;
};


// CLASS NOTES (poOvalShape)
//
// A poOvalShape is a poShape2D in the form of an oval or circle, and may also be used to form regular polygons.
//
// When a poOvalShape is constructed, the third argument specifies the number of points to be used.
// For a smooth oval or circle, this should generally be about 1/3 the width or height of the shape.
// For example, a 100x100 circle, should use approximately 33 points.
//
// Regular polygons (triangles, squares, pentagons) can be constructed simply by using a small number of points
//
// A simple circular poOvalShape can be constructed as follows:
//      poOvalShape* S = new poOvalShape( 100,100,33 );
//
// An non-circular poOvalShape can be constructed as follows:
//      poOvalShape* S = new poOvalShape( 100,200,33 );


class poOvalShape : public poShape2D {
public:
    poOvalShape();
	poOvalShape( float radius );
    poOvalShape( float _width, float _height, int numPoints );
	virtual ~poOvalShape() {}
	
	virtual poObject *copy();
    
    void    reshape( float _width, float _height, int numPoints );
    
protected:
	void	clone(poOvalShape *shp);
    void    construct( float _width, float _height, int numPoints );
    
    float   width, height;
};


// CLASS NOTES (poLineShape)
//
// A poLineShape is a poShape2D in the form of a single line segment.
// 
// It is not recommenended to use poLineShape for drawing 100's or 1000's of lines. A much more
// efficient option is to use a single poShape2D and set the fillDrawStyle to GL_LINES. In that case,
// each pair of points added will form an individual line segment.
//
// A simple poLineShape can be constructed as follows:
//      poLineShape* S = new poLineShape( poPoint(0,0), poPoint(100,100) );

class poLineShape : public poShape2D {
public:
    poLineShape();
    poLineShape( poPoint A, poPoint B );
	virtual ~poLineShape() {}
    
    void    reshape( poPoint A, poPoint B );
    
protected:
    void    construct( poPoint A, poPoint B );
};


// CLASS NOTES (poStarShape)
//
// A poStarShape is a poShape2D in the form of a star.
//
// A simple poStarShape can be constructed as follows:
//      poStarShape* S = new poStarShape( 100, 5, 150 );

class poStarShape : public poShape2D {
public:
    poStarShape();
    poStarShape( float _outerRadius, int numPoints, float depth );
    poStarShape( float _width, float _height, int numPoints, float depth );
	
	virtual poObject *copy();
    
    void	reshape( float _outerRadius, int numPoints, float depth );
    void	reshape( float _width, float _height, int numPoints, float depth );
    
protected:
	void	clone(poStarShape* shp);
	
    void	construct( float _outerRadius, int numPoints, float depth );
    void	construct( float _width, float _height, int numPoints, float depth );
    
    float	width, height, depth;
    float	outerRadius, innerRadius;
};
>>>>>>> master
