#include "TextTestApp.h"

#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "SimpleDrawing.h"
#include "poResourceStore.h"

poObject *createObjectForID(uint uid) {
	return new TextTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextTest", 100, 100, 800, 800);
}

void cleanupApplication() {
}

TextTestApp::TextTestApp() {
	addModifier(new poCamera2D());
	
	poFont *reg = getFont("Georgia", 30, "Regular");
	poFont *italic = getFont("Helvetica Neue", 30, "Italic");
	poFont *bold = getFont("Helvetica Neue", 30, "Bold");
	poFont *bold_italic = getFont("Helvetica Neue", 30, "Bold Italic");
	poFont *con_bold = getFont("Helvetica Neue", 30, "Condensed Bold");

	std::string text =	"<b>ABCDEFGHIJKLM</b>\n"
						"<i>NOPQRSTUVWXYZ</i>\n"
						"<span>abcdefghijklm</span>\n"
						"<bi>nopqrstuvwxyz</bi>\n"
						"<cb>1234567890</cb>Hello";
	
	poTextBox *tb;
	tb = addChild(new poTextBox(getWindowWidth(), getWindowHeight()));
	tb->alignment(PO_ALIGN_CENTER_CENTER);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->position = getWindowCenter();
	tb->font(reg, "span");
	tb->font(italic, "i");
	tb->font(bold, "b");
	tb->font(bold_italic, "bi");
	tb->font(con_bold, "cb");
	tb->text(text);
	tb->layout();
	tb->drawBounds = true;
}

TextTestApp::~TextTestApp() {
}

void TextTestApp::draw() {
	drawString("Hello people", getFont("Georgia", 30, "Regular"), poPoint(0,0));
}

void TextTestApp::eventHandler(poEvent *event) {
	
}

void TextTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
