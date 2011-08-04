#include "TextTestApp.h"

#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poResourceStore.h"

poObject *createObjectForID(uint uid) {
	return new TextTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextTest", 100, 100, 1000, 800);
}

void cleanupApplication() {
}

TextTestApp::TextTestApp() {
	addModifier(new poCamera2D());
	
	poFont *light = getFont("Helvetica Neue", 30, "Regular");
	poFont *italic = getFont("Helvetica Neue", 30, "Italic");
	poFont *bold = getFont("Helvetica Neue", 30, "Bold");
	poFont *bold_italic = getFont("Helvetica Neue", 30, "Bold Italic");

	std::string text = "ABCDEFGHIJKLM\nNOPQRSTUVWXYZ\nabcdefghijklm\nnopqrstuvwxyz\n1234567890";
	
	poTextBox *tb;
	
	tb = addChild(new poTextBox(300,300));
	tb->position.set(20,10,0);
	tb->font(light);
	tb->text(text);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->layout();
	
	tb = addChild(new poTextBox(300,300));
	tb->position.set(340,10,0);
	tb->font(italic);
	tb->text(text);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->layout();
	
	tb = addChild(new poTextBox(300,300));
	tb->position.set(20,320,0);
	tb->font(bold);
	tb->text(text);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->layout();

	tb = addChild(new poTextBox(300,300));
	tb->position.set(340,320,0);
	tb->font(bold_italic);
	tb->text(text);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->layout();
}

TextTestApp::~TextTestApp() {
}

void TextTestApp::eventHandler(poEvent *event) {
	
}

void TextTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
