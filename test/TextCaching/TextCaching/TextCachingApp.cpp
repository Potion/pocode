#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "TextCachingApp.h"
#include "poShapeBasics2D.h"

namespace po {
	
	po::Object *createObjectForID(uint uid) {
		return new TextCachingApp();
	}

	void setupApplication() {
		applicationCreateWindow(0, po::WINDOW_TYPE_NORMAL, "TextCaching", 100, 100, 600, 600);
	}

	void cleanupApplication() {
	}

}

TextCachingApp::TextCachingApp() {
	addModifier(new po::Camera2D());
	
	std::stringstream ss;
	for(int i=0; i<10; i++) 
		ss << "I will not, ever again. ";
	
	po::TextBox *tb = (po::TextBox*)addChild(new po::TextBox(500,300));
	tb->setFont(po::getFont("Helvetica","Regular"));
	tb->position.set(50,200,0);
	tb->textColor = po::Color::white;
	tb->setText(ss.str());
	tb->setTextSize(20);
	tb->doLayout();

	tb = (po::TextBox*)addChild((po::TextBox*)tb->copy());
	tb->setCacheToTexture(true);
	tb->doLayout();
	tb->position.y += tb->getTextBounds().height + 10;
}

TextCachingApp::~TextCachingApp() {}
void TextCachingApp::eventHandler(po::Event *event) {}
void TextCachingApp::messageHandler(const std::string &msg, const po::Dictionary& dict) {}
