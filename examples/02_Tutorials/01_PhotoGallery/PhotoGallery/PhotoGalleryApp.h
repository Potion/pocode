/////////////////////////////////////////
//
// pocode : Photo Gallery
//
/////////////////////////////////////////

#include "poObject.h"
#include "poImageShape.h"

class PhotoGalleryApp : public poObject {
public:
	PhotoGalleryApp();
	virtual ~PhotoGalleryApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
    poPoint smallScale;
	poObject* selectedPhoto;
	poPoint	goBackPosition;
};