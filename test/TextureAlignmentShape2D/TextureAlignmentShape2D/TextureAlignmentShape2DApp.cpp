#include "TextureAlignmentShape2DApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new TextureAlignmentShape2DApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextureAlignmentShape2D", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

TextureAlignmentShape2DApp::TextureAlignmentShape2DApp() {
	addModifier(new poCamera2D(poColor::black));
	
	portrait = new poTexture("portrait.jpg");
	landscape = new poTexture("landscape.jpg");
	
	currentFit = 0;
	currentAlign = 0;
	
	TB = new poTextBox(200, 100);
	TB->setFont(new poFont("Lucida Grande"));
	TB->setText("FIT 0 - ALIGN 0");
	TB->setTextSize(20);
	TB->textColor = poColor::white;
	TB->doLayout();
	TB->position.set(getWindowWidth() - 200, getWindowHeight() - 50, 0);
	addChild(TB);
	
	for(int i=0; i < 4; i++) {
		poRect rect;
		switch (i) {
			case 0:
				// top left
				rect.set(150, 100, 300, 150);
				break;
			case 1:
				// top right
				rect.set(570, 100, 300, 150);
				break;
			case 2:
				// bottom left
				rect.set(230, 370, 150, 300);
				break;
			case 3:
				// bottom right
				rect.set(640, 370, 150, 300);
				break;
			default:
				break;
		}
		
		poRectShape* frame = new poRectShape(rect.width, rect.height);
		frame->generateStroke(2);
		frame->strokeColor = poColor::red;
		frame->position.set(rect.x, rect.y, 0);
		addChild(frame);
		
		poShape2D* S = new poShape2D();
		S->position.set(rect.x, rect.y, 0);
		S->addPoint(0,0);
		S->addPoint(rect.width, 0);
		S->addPoint(rect.width, rect.height);
		S->addPoint(0, rect.height);
		addChild(S);
		
		shapes.push_back(S);
	}
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

TextureAlignmentShape2DApp::~TextureAlignmentShape2DApp() {
}

void TextureAlignmentShape2DApp::eventHandler(poEvent *event) {
	
	if(event->keyCode == PO_LEFT_ARROW) {
		
		currentFit -=1;
		if(currentFit < 0) currentFit = 4;
	}
	else if(event->keyCode == PO_RIGHT_ARROW) {
		
		currentFit +=1;
		if(currentFit > 4) currentFit = 0;
	}
	else if(event->keyCode == PO_UP_ARROW) {
		
		currentAlign +=1;
		if(currentAlign > 8) currentAlign = 0;
	}
	else if(event->keyCode == PO_DOWN_ARROW) {
		
		currentAlign -=1;
		if(currentAlign < 0) currentAlign = 8;
	}
	
	for(int i=0; i < shapes.size(); i++) {
		
		poTexture* tex;
		switch (i) {
			case 0:
				// top left
				tex = landscape;
				break;
			case 1:
				// top right
				tex = portrait;
				break;
			case 2:
				// bottom left
				tex = landscape;
				break;
			case 3:
				// bottom right
				tex = portrait;
				break;
			default:
				break;
		}
		shapes[i]->placeTexture(tex, poTextureFitOption(currentFit), poAlignment(currentAlign));
	}
	
	char str[16];
	sprintf(str, "FIT %d - ALIGN %d", currentFit, currentAlign);
	
	TB->setText(str);
	TB->doLayout();
}

void TextureAlignmentShape2DApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
