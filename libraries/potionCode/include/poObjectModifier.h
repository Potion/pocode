//
//  poObjectModifier.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


class poObject;

// CLASS NOTES
//
// A poObjectModifer can be attached to a poObject and can change that poObject
// on every frame. poObjectModifier, as a base class, does not change anything.
//
// A poObjectModifier has two virtual methods, doSetUp() and doSetDown(). These are
// are called before and after the draw() method of the attached object.
//
// A poObjectModifier can alter any of the public properties of an object and may
// also call any of its public methods, either before or after drawing. Oftentimes, 
// poObjectModifiers are used to enable and disable OpenGL state.
// 
// Examples of poObjectModifiers the poCamera and poGeometry Mask. Disabling a
// poObjectModifier just tempoarily turns it off, and can be switched back  on
// at any time.
// 

class poObjectModifier
{
public:
    poObjectModifier() : enabled(true) {}

	virtual poObjectModifier* copy() {return new poObjectModifier();}
    
    // ENABLE & DISABLE
	bool            isEnabled() const {return enabled;}
	void            setEnabled(bool b) {enabled = b;}
	
    // SETUP 
    // This is called just BEFORE the draw() method of the poObject it is attached to.
    void setUp( poObject* obj ) {
		if(isEnabled())
			doSetUp(obj);
	}
    
    // SETDOWN
    // This is called just AFTER the draw() method of the poObject it is attached to.
    void setDown( poObject* obj ) { 
		if(isEnabled())
			doSetDown(obj);
	}
	
protected:
    // VIRTUAL METHODS
    // THESE METHODS ARE THE ONES THAT WILL GET CALLED SO SUBCLASS THEM
	virtual void    doSetUp(poObject*) {}
	virtual void    doSetDown(poObject*) {}
	
	void			clone(poObjectModifier *obj) {
		obj->enabled = enabled;
	}
	
private:
	bool            enabled;
};


