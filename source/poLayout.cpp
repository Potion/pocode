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

#include "poLayout.h"

namespace po {

    Layout::Layout(Orientation orientation) :
    layoutOrientation(orientation),
    refreshEveryFrame(true),
    layoutDone(false)
    {}

    Layout::~Layout() {}
    
    
    //------------------------------------------------------------------------
    void Layout::setSpacing(float f) {
        setSpacing(f, f);
    }
    
    
    //------------------------------------------------------------------------
    void Layout::setSpacing(float w, float h) {
        spacing[SPACING_H] = w;
        spacing[SPACING_V] = h;
    }
    
    
    //------------------------------------------------------------------------
    po::Orientation Layout::getOrientation() {
        return layoutOrientation;
    }
    
    
    //------------------------------------------------------------------------
    void Layout::setOrientation(po::Orientation orient) {
        if(layoutOrientation != orient) layoutDone = false;
        layoutOrientation = orient;
    }
    
    
    //------------------------------------------------------------------------
    void Layout::refresh() {
        layoutDone = false;
    }
    
    
    //------------------------------------------------------------------------
    bool Layout::doesRefreshEveryFrame() {
        return refreshEveryFrame;
    }
    
    
    //------------------------------------------------------------------------
    void Layout::setRefreshEveryFrame( bool b ) {
        refreshEveryFrame = b;
    }
    
    
    //------------------------------------------------------------------------
    void Layout::doLayout( Object* obj ) {
        Point pos(0,0);
        for(int i=0; i < obj->getNumChildren(); i++) {
            
            Object* child = obj->getChild(i);
            child->position = pos;
            
            if( layoutOrientation == PO_HORIZONTAL || 
                layoutOrientation == PO_HORIZONTAL_RIGHT ||
                layoutOrientation == PO_HORIZONTAL_LEFT ) {
                
                child->position.x -= child->offset.x;
                float direction = 1.f;
                if( layoutOrientation == PO_HORIZONTAL_LEFT ) {
                    child->position.x -= child->getBounds().width;
                    direction = -1.f;
                }
                pos.x += child->getBounds().width * direction;
                pos.x += spacing[SPACING_H] * direction;
            }
            else 
                if(	layoutOrientation == PO_VERTICAL ||
                    layoutOrientation == PO_VERTICAL_DOWN ||
                    layoutOrientation == PO_VERTICAL_UP ) {
                    
                    child->position.y -= child->offset.y;
                    float direction = 1.f;
                    if( layoutOrientation == PO_VERTICAL_UP ) {
                        child->position.y -= child->getBounds().height;
                        direction = -1.f;
                    }
                    pos.y += child->getBounds().height * direction;
                    pos.y += spacing[SPACING_V] * direction;
                }
        }
    }
    
    
    //------------------------------------------------------------------------
    void Layout::doSetUp( Object* obj ) {
        if(refreshEveryFrame || !layoutDone) {
            doLayout( obj );
            layoutDone = true;
        }
    }
}