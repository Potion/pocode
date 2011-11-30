/*	Created by filippo on 11/29/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poTextBox.h"

class TextureAlignmentApp : public poObject {
public:
	TextureAlignmentApp();
	virtual ~TextureAlignmentApp();
	
	virtual void draw();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poTexture* portrait;
	poTexture* landscape;
	
	int currentFit;
	int currentAlign;
	
	poTextBox* TB;
};