/*	Created by Joshua Fisher on 8/9/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

#include "TextLayout.h"

class SplitTextApp : public poObject {
public:
	SplitTextApp();

	void draw();
	void eventHandler(poEvent *event);
	
	void restartDynamicEntrance();
	
	TextBoxLayout layout;

	std::vector< float > offsets;
	std::vector< poTween<float> > tweens;
};