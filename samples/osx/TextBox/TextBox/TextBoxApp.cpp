#include "TextBoxApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poApplication.h"
#include "poResourceLoader.h"
#include "poShapeBasics2D.h"
#include "poSimpleDrawing.h"

#define PADDING 20


poObject *createObjectForID(uint uid) {
	return new TextBoxApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextBox", 100, 600, 600, 500);
}

void cleanupApplication() {
}

TextBoxApp::TextBoxApp() {
	addModifier(new poCamera2D());
	
	tb = new poTextBox(getWindowWidth()-2*20, getWindowHeight()/2-2*20);
	tb->setCacheToTexture(true);
	tb->setFont(getFont("Helvetica", "Regular"));
	tb->setTextSize(20);
	tb->textColor = poColor::red;
	tb->position.set(20,20,0);
	tb->drawBounds = PO_TEXT_BOX_STROKE_TEXT_BOUNDS;
	tb->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.");
	tb->doLayout();
	addChild(tb);

//	poObject *obj = new poRectShape(tb->getTextBounds().width, tb->getTextBounds().height);
//	obj->position = tb->position;
//	addChild(obj);
//	moveChildBackward(obj);
//	addChild(tb);
	
//	poObject *obj = addChild(new poRectShape(getBitmapFont(getFont("Helvetica", "Regular"), 50)->textureForPage(0)));
	addEvent(PO_KEY_DOWN_EVENT, this);
}
 
static uint codepoint = 'l';
static double lastTime = 0.0;

void TextBoxApp::draw() {
//	// testing simple text drawing
//	poPoint pos(5,5);
//	
//	po::setColor(poColor::white);
//	
//	po::drawString("quick brown fox blah blah", getFont("Helvetica", "Regular"), pos, 20);
//	po::drawLine(pos+poPoint(.5,.5), poPoint(pos.x+.5,pos.y+15.5));
//	po::drawLine(pos+poPoint(.5,.5), poPoint(pos.x+100.5,pos.y+.5));
	poObject::draw();
}

void TextBoxApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT){
		switch (event->keyChar){
			case 'b': 
				tb->drawBounds = !tb->drawBounds; 
				break;
			case '+': 
				tb->setTextSize(tb->getTextSize() + 1);
				tb->doLayout();
				break;
			case '-':
			{
				tb->setTextSize(tb->getTextSize() - 1);
				tb->doLayout();
				break;
			}
			case ' ':
				tb->setCacheToTexture(!tb->getCacheToTexture());
				tb->doLayout();
				break;
		}
	}

}
