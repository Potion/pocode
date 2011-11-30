/*	Created by filippo on 11/29/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

class TextureAlignmentShape2DApp : public poObject {
public:
	TextureAlignmentShape2DApp();
	virtual ~TextureAlignmentShape2DApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	void drawTexturedPolygon(const std::vector<poPoint> &points, poTexture *tex, poTextureFitOption fit, poAlignment align);
	
	poTexture* portrait;
	poTexture* landscape;
	
	int currentFit;
	int currentAlign;
	
	poTextBox* TB;
	
	std::vector<poShape2D*> shapes;
};