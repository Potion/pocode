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

	f = new poFont("courier", 20);
	
	tb = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb->position(PADDING,PADDING);
	tb->textAlignment(PO_ALIGN_TOP_CENTER);
	tb->font("courier",f);
	tb->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.");
	tb->layout();
	addChild(tb);
	
	tb2 = new poTextBox(getWindowWidth()-2*PADDING, getWindowHeight()/2-2*PADDING);
	tb2->position(PADDING,getWindowHeight()/2+PADDING);
	tb2->textAlignment(PO_ALIGN_TOP_CENTER);
	tb2->font("courier",f);
	tb2->text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent massa ante, malesuada a laoreet imperdiet, fringilla ut tellus. In sem est, imperdiet in lobortis vel, varius lobortis velit.");
	tb2->layout();
	addChild(tb2);
	
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

TextBoxApp::~TextBoxApp() {
}

void TextBoxApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT){
		switch (event->keyChar){
			case 'b': 
				tb->drawBounds(!tb->drawBounds()); 
				break;
			case '+': 
				f = new poFont("courier",f->pointSize()+1);
				tb->font("courier",f);
				tb->layout();
				break;
			case '-':
				f = new poFont("courier",f->pointSize()-1);
				tb->font("courier",f);
				tb->layout();
				break;
			default: break;
		}
	}
	
}

void TextBoxApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
