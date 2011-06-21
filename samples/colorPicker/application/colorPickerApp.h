/*	Created by xiaoyang feng on 6/21/11.
 *	Copyright 2011 Potion Design LLC. All rights reserved.
 */

#include "poObject.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

class colorPickerApp : public poObject {
public:
	colorPickerApp();
	virtual ~colorPickerApp();
    
    poRectShape     *theRect;
    
    poImage         *theImage;
    
    poOvalShape     *indicator;
    
    poRectShape     *rgbLabel;
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};