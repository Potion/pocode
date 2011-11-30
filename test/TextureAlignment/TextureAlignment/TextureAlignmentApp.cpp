#include "TextureAlignmentApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poSimpleDrawing.h"

poObject *createObjectForID(uint uid) {
	return new TextureAlignmentApp();
}

void setupApplication() {
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextureAlignment", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

TextureAlignmentApp::TextureAlignmentApp() {
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
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

TextureAlignmentApp::~TextureAlignmentApp() {
}

void TextureAlignmentApp::draw() {
	
	for(int i=0; i < 4; i++) {
		
		poRect rect;
		poTexture* tex;
		
		switch (i) {
			case 0:
				// top left
				rect.set(150, 100, 300, 150);
				tex = landscape;
				break;
			case 1:
				// top right
				rect.set(570, 100, 300, 150);
				tex = portrait;
				break;
			case 2:
				// bottom left
				rect.set(230, 370, 150, 300);
				tex = landscape;
				break;
			case 3:
				// bottom right
				rect.set(640, 370, 150, 300);
				tex = portrait;
				break;
			default:
				break;
		}
		
		po::setColor(poColor::yellow);
		po::drawStrokedRect(rect);
		
		std::vector<poPoint> points;
		points.push_back(poPoint(rect.x, rect.y));
		points.push_back(poPoint(rect.x + rect.width, rect.y));
		points.push_back(poPoint(rect.x + rect.width, rect.y + rect.height));
		points.push_back(poPoint(rect.x, rect.y + rect.height));
		
		po::setColor(poColor::white);
		po::drawTexturedPolygon(points, tex, poTextureFitOption(currentFit), poAlignment(currentAlign));
	}
}

void TextureAlignmentApp::eventHandler(poEvent *event) {
	
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
	
	char str[16];
	sprintf(str, "FIT %d - ALIGN %d", currentFit, currentAlign);
	
	TB->setText(str);
	TB->doLayout();
}

void TextureAlignmentApp::messageHandler(const std::string &msg, const poDictionary& dict) {
}
