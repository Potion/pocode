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
//  ObjectModifier.h
//  pocode
//
//  Created by Joshua Fisher on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

namespace po {
    class Object;

    // CLASS NOTES
    //
    // A ObjectModifer can be attached to a Object and can change that Object
    // on every frame. ObjectModifier, as a base class, does not change anything.
    //
    // A ObjectModifier has two virtual methods, doSetUp() and doSetDown(). These are
    // are called before and after the draw() method of the attached object.
    //
    // A ObjectModifier can alter any of the public properties of an object and may
    // also call any of its public methods, either before or after drawing. Oftentimes, 
    // ObjectModifiers are used to enable and disable OpenGL state.
    // 
    // Examples of ObjectModifiers the poCamera and poGeometry Mask. Disabling a
    // ObjectModifier just tempoarily turns it off, and can be switched back  on
    // at any time.
    // 


    class ObjectModifier {
    public:
        ObjectModifier() : enabled(true) {}

        virtual ObjectModifier* copy() {return new ObjectModifier();}
        
        // ENABLE & DISABLE
        bool            isEnabled() const {return enabled;}
        void            setEnabled(bool b) {enabled = b;}
        
        // SETUP 
        // This is called just BEFORE the draw() method of the Object it is attached to.
        void setUp( Object* obj ) {
            if(isEnabled())
                doSetUp(obj);
        }
        
        // SETDOWN
        // This is called just AFTER the draw() method of the Object it is attached to.
        void setDown( Object* obj ) { 
            if(isEnabled())
                doSetDown(obj);
        }
        
    protected:
        // VIRTUAL METHODS
        // THESE METHODS ARE THE ONES THAT WILL GET CALLED SO SUBCLASS THEM
        virtual void    doSetUp(Object*) {}
        virtual void    doSetDown(Object*) {}
        
        void			clone(ObjectModifier *obj) {
            obj->enabled = enabled;
        }
        
    private:
        bool            enabled;
    };
}/* End po namespace */

