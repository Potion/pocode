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
    poObjectModifier() { };
    
    virtual void setUp( poObject* obj ) { };
    virtual void setDown( poObject* obj ) { };
};

