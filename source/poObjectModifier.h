//
//  poObjectModifier.h
//  poFramework4
//
//  Created by Joshua Fisher on 5/20/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

class poObject;

class poObjectModifier
{
public:
    poObjectModifier() : _enabled(true) {}
    
	bool enabled() const {return _enabled;}
	void enabled(bool b) {_enabled = b;}
	
    void setUp( poObject* obj ) {
		if(enabled())
			doSetUp(obj);
	}
    void setDown( poObject* obj ) { 
		if(enabled())
			doSetDown(obj);
	}
	
protected:
	virtual void doSetUp(poObject*) = 0;
	virtual void doSetDown(poObject*) = 0;
	
private:
	bool _enabled;
};

