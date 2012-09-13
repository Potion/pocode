#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "TextCachingApp.h"
#include "poShapeBasics2D.h"

poObject *createObjectForID(uint uid) {
	return new TextCachingApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextCaching", 100, 100, 600, 600);
}

void cleanupApplication() {
}

TextCachingApp::TextCachingApp() {
	addModifier(new poCamera2D());
	
	std::stringstream ss;
	for(int i=0; i<10; i++) 
		ss << "I will not, ever again. ";
	
	poTextBox *tb = (poTextBox*)addChild(new poTextBox(500,300));
	tb->setFont(poGetFont("Helvetica","Regular"));
	tb->position.set(50,200,0);
	tb->textColor = poColor::white;
	tb->setText(ss.str());
	tb->setTextSize(20);
	tb->doLayout();

	tb = (poTextBox*)addChild((poTextBox*)tb->copy());
	tb->setCacheToTexture(true);
	tb->doLayout();
	tb->position.y += tb->getTextBounds().height + 10;
}

TextCachingApp::~TextCachingApp() {}
void TextCachingApp::eventHandler(poEvent *event) {}
void TextCachingApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
