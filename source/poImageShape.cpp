#include "poImageShape.h"
#include "poSimpleDrawing.h"

poImageShape::poImageShape()
:	tex(NULL)
,	alphaTest(false)
,   imageScale(1.0)
{}

poImageShape::poImageShape(poImage *i, bool keepImage)
:	tex(new poTexture(i, keepImage))
,	alphaTest(i->hasAlpha())
,   imageScale(1.0)
{
}

poImageShape::poImageShape(const std::string &str, bool keepImage) 
:	tex(NULL)
,	alphaTest(false)
,   imageScale(1.0)
{
    tex = poGetTexture( str, keepImage );
	alphaTest = (keepImage && tex->getSourceImage() && tex->getSourceImage()->hasAlpha());
}

poImageShape::~poImageShape() {
	delete tex;
}

void    poImageShape::reshapeToWidth( float W )
{
    imageScale = W / (float) tex->getWidth();
}

void    poImageShape::reshapeToHeight( float H )
{
    imageScale = H / (float) tex->getHeight();
}

void    poImageShape::reshapeToWidthAndHeight( float W, float H )
{
    imageScale = W / (float) tex->getWidth();
    if ( tex->getHeight()*imageScale > H )
        imageScale = H / (float) tex->getHeight();
}

void poImageShape::draw() {
    po::setColor(poColor::white);
    po::drawTexturedRect(tex, 0,0, tex->getWidth()*imageScale, tex->getHeight()*imageScale);
}

bool poImageShape::doesAlphaTest() const {
	return alphaTest;
}

void poImageShape::setAlphaTest(bool b) {
	alphaTest = b;
}

poImage *poImageShape::getImage() const {
	return tex->getSourceImage();
}

/*void poImageShape::setImage(poImage* i) {
	if(img) {
		delete tex;
		
		img = i;
		tex = new poTexture(img);
	}
}*/

bool poImageShape::pointInside(poPoint p, bool localize)
{	
    if(!visible)
		return false;
	
	if(localize) {
		p.y = getWindowHeight() - p.y;
		p = globalToLocal(p);
	}
	
	if(alphaTest) {
        // flip y value, since poImage y coordinates are reversed
        p.y = tex->getSourceImage()->getHeight()*imageScale - p.y;
        p /= imageScale;
		poColor pix = tex->getSourceImagePixel(p);
		return pix.A > 0.f;
	}
	
    poRect r(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
	return r.contains(p.x, p.y);
}

poRect  poImageShape::getBounds()
{
    poRect rect(0,0,tex->getWidth()*imageScale,tex->getHeight()*imageScale);
    return rect;
}

