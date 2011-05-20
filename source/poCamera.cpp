//
//  poCamera.cpp
//  poFramework4
//
//  Created by Jared Schiffman on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poCamera.h"
#include "poApplication.h"

poCamera2D::poCamera2D()
{
    clearBackground = true;
    backgroundColor.set( 0,0,0 );
}

void poCamera2D::setUp( poObject* obj ) {
    // setup orthographic camera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glPushMatrix();
	glOrtho(0, getWindowWidth(), getWindowHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    // disable the depth test
    glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	
    // enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // enable anti-aliasing
    glEnable( GL_LINE_SMOOTH );
    
    // clear background
    if ( clearBackground )
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor( backgroundColor.R, backgroundColor.G, backgroundColor.B, 1 );
    }
}

void poCamera2D::setDown( poObject* obj ) {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}