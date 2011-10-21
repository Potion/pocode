#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "TextCachingApp.h"
#include "poResourceLoader.h"

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

	poTextBox *tb = addChild(new poTextBox(500,500));
	tb->font(getFont("Helvetica","Bold"), PO_TEXT_BOLD);
	tb->font(getFont("Helvetica"));
	tb->textColor = poColor::red;
	tb->position.set(50,50,0);
	tb->cacheToTexture(false);
	tb->text(ss.str());
	tb->textSize(20);
	tb->layout();

	tb = tb->clone();
}

TextCachingApp::~TextCachingApp() {}
void TextCachingApp::eventHandler(poEvent *event) {}
void TextCachingApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
