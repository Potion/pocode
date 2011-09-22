#include "TextBoxApp.h"

#include "poCamera.h"
#include "poApplication.h"
#include "poResourceStore.h"

#include "SimpleDrawing.h"

#define PADDING 20

#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	return new TextBoxApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextBox", 100, 100, 600, 300);
}

void cleanupApplication() {
}

TextBoxApp::TextBoxApp() {
	addModifier(new poCamera2D(poColor::black));
	addModifier(basicProgram1());
    
    poRectShape * test = addChild(new poRectShape(500,500));
    test->fillColor.set255(255,255,255);
	
	tb = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb->textAlignment(PO_ALIGN_TOP_CENTER);
	tb->drawBounds = true;
	tb->font(getFont("Courier", 20));
	tb->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.");
	tb->layout();
	tb->position.set(PADDING,PADDING,1);
	addChild(tb);
	
     
	tb2 = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb2->drawBounds = true;
	tb2->textAlignment(PO_ALIGN_TOP_CENTER);
	tb2->font(getFont("Courier", 20));
	tb2->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.");
	tb2->layout();
	tb2->position.set(PADDING,getWindowHeight()/2+PADDING,1);
	addChild(tb2);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}
   
void TextBoxApp::draw() {
	// testing simple text drawing
	poPoint pos(5,5);
	poFont *font = getFont("Helvetica", 15);
	drawString("quick brown fox blah blah", font, pos);
	drawLine(pos+poPoint(.5,.5), poPoint(pos.x+.5,pos.y+15.5));
	drawLine(pos+poPoint(.5,.5), poPoint(pos.x+100.5,pos.y+.5));
}  

void TextBoxApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT){
		switch (event->keyChar){
			case 'b': 
				tb->drawBounds = !tb->drawBounds; 
				break;
			case '+': 
				tb->font(getFont("Courier", tb->font(PO_TEXT_REGULAR)->pointSize()+1));
				tb->layout();
				break;
			case '-':
			{
				std::string font_name = "Courier";
				int font_size = tb->font(PO_TEXT_REGULAR)->pointSize();
				
				tb->font(getFont(font_name, font_size-1));
				tb->layout();
				break;
			}
		}
	}
	
}
