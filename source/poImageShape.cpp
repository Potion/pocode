#include "poImageShape.h"
#include "poSimpleDrawing.h"

poImageShape::poImageShape()
:	img(NULL)
,	tex(NULL)
,	alphaTest(false)
{}

poImageShape::poImageShape(poImage *i)
:	img(i)
,	tex(new poTexture(i))
,	alphaTest(i->hasAlpha())
{
	setSize(img->getWidth(), img->getHeight());
}

poImageShape::poImageShape(const std::string &str) 
:	img(new poImage(str))
,	tex(NULL)
,	alphaTest(false)
{
	if(!img->isValid()) {
		delete img;
		img = NULL;
		return;
	}
	
	tex = new poTexture(img);
	alphaTest = img->hasAlpha();
	setSize(img->getWidth(), img->getHeight());
}

poImageShape::~poImageShape() {
	delete tex;
}

void poImageShape::draw() {
	if(img) {
		po::drawTexturedRect(tex);
	}
}

bool poImageShape::doesAlphaTest() const {
	return alphaTest;
}

void poImageShape::setAlphaTest(bool b) {
	alphaTest = b;
}

poImage *poImageShape::getImage() const {
	return img;
}

void poImageShape::setImage(poImage* i) {
	if(img) {
		delete tex;
		
		img = i;
		tex = new poTexture(img);
	}
}

bool poImageShape::pointInside(poPoint p, bool localize) {
	if(!img)
		return false;
	
	if(localize) 
		p = globalToLocal(p);
	
	if(alphaTest) {
		poColor pix = img->getPixel(p);
		return pix.A > 0.f;
	}
	
	poRect r(0,0,img->getWidth(),img->getHeight());
	return r.contains(p.x, p.y);
}



