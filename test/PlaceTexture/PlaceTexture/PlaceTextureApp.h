/*	Created by Joshua Fisher on 10/18/11.
 *	Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"
class poRectShape;

class PlaceTextureApp : public poObject {
public:
	PlaceTextureApp();
	virtual ~PlaceTextureApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape *shape;
};