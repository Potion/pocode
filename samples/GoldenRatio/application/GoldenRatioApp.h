/*	Created by Jonathan Bobrow on 6/2/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

using namespace std;

class GoldenRatioApp : public poObject {
public:
	GoldenRatioApp();
	virtual ~GoldenRatioApp();
    virtual void update();
    virtual void preDraw();
	virtual void draw();
    virtual void postDraw();
	
	virtual void eventHandler(poEvent *event);
    
    vector<poOvalShape*> seeds;
    poResourceStore resources;
};