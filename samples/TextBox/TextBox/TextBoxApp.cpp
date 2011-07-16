#include "TextBoxApp.h"
#include "poApplication.h"
#include "poCamera.h"

#define PADDING 20

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

	tb = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb->textAlignment(PO_ALIGN_TOP_CENTER)
		.font(poFont("Courier", 20))
		.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.")
		.layout()
		.position(PADDING,PADDING);
	addChild(tb);
	
	tb2 = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb2->textAlignment(PO_ALIGN_TOP_CENTER)
		.font(poFont("Courier", 20))
		.text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.")
		.layout()
		.position(PADDING,getWindowHeight()/2+PADDING);
	addChild(tb2);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TextBoxApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT){
		switch (event->keyChar){
			case 'b': 
				tb->drawBounds(!tb->drawBounds()); 
				break;
			case '+': 
				tb->font(poFont("Courier", tb->font(PO_FONT_REGULAR)->pointSize()+1));
				tb->layout();
				break;
			case '-':
				tb->font(poFont("Courier", tb->font(PO_FONT_REGULAR)->pointSize()-1));
				tb->layout();
				break;
		}
	}
	
}
