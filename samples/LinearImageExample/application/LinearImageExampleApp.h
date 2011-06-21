/*	Created by Miguel on 6/20/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poImage.h"
#include "poShapeBasics2D.h"
#include "poShape2D.h"
#include "poColor.h"
#include <vector>

using namespace std;

class LinearImageExampleApp : public poObject {
public:
	LinearImageExampleApp();
	virtual ~LinearImageExampleApp();
    
    virtual void draw();
    virtual void update();
    
    float mSignal;
    float mouseX, mouseY;
    int mDirection;
    bool bMousePressed;
    bool bKeyPressed;
    
        
    poImage* myImage;
    poRectShape* imageRect;
    poShape2D* mLine;

    vector<poColor> pixelWideLineColors;
    vector<poRectShape*> unitRects;
    
    void getPixelWideLine(int yPos);
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};