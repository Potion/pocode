/*	Created by Jonathan Bobrow on 8/12/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"

class AdvancedTweensApp : public poObject {
public:
	AdvancedTweensApp();
	virtual ~AdvancedTweensApp();
	virtual void update();
	void animate();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poOvalShape* shape;
	float x_tween_val, y_tween_val;
	poTween<float> float_tween_y, float_tween_x;
};