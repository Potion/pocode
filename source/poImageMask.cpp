#include "poMask.h"
#include "SimpleDrawing.h"
#include "poResourceStore.h"

poImageMask::poImageMask(poImage *img)
:	image(img)
{}

poImageMask::poImageMask(const std::string &str)
{
	image = getImage(str, IMAGE_8);
}

bool poImageMask::pointInside(poPoint p) {
	
}

void poImageMask::doSetUp( poObject* obj ) {
	my_obj = obj;
	
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	
	glClear(GL_STENCIL_BUFFER_BIT);
	
	glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ZERO);
	drawRect(image->texture());
	
	glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
}

void poImageMask::doSetDown( poObject* obj ) {
	glPopAttrib();
}
