/*	Created by Steve Varga on 4/27/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poObject.h"

#include "poShapeBasics2D.h"
#include "poTextBox.h"

class RetinaGraphicsApp : public poObject {
public:
	RetinaGraphicsApp();
	virtual ~RetinaGraphicsApp();
    virtual void update();
    virtual void draw();
	
    poRectShape *bg1, *bg2, *redSquare;
    
    poTextBox *text;
    
    virtual void eventHandler(poEvent *event);
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    
};

