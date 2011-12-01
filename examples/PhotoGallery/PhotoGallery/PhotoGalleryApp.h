/*	Created by filippo on 11/29/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poImageShape.h"

class PhotoGalleryApp : public poObject {
public:
	PhotoGalleryApp();
	virtual ~PhotoGalleryApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	std::vector<poImageShape*> images;
	
	poImageShape* selectedPhoto;
	poPoint	goBackPosition;
    
    poPoint smallScale;
};