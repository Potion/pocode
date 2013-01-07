/*	Created by Steve Varga on 4/27/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "poObject.h"

#include "poShapeBasics2D.h"
#include "poTextBox.h"

class RetinaGraphicsApp : public po::Object {
public:
	RetinaGraphicsApp();
	virtual ~RetinaGraphicsApp();
    virtual void update();
    virtual void draw();
	
    po::RectShape *bg1, *bg2, *redSquare;
    
    po::TextBox *text;
    
    virtual void eventHandler(po::Event *event);
    virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
    
    
};

