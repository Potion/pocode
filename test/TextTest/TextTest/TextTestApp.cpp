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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextTest", 100, 100, 300, 300);
}

void cleanupApplication() {
}

poTextBox *tb;
poFont *reg, *italic, *bold, *bold_italic, *con_bold, *huge;

TextTestApp::TextTestApp() {
	addModifier(new poCamera2D());
	
	reg = getFont("Helvetica Neue", 30, "Regular");
	italic = getFont("Helvetica Neue", 30, "Italic");
	bold = getFont("Helvetica Neue", 30, "Bold");
	bold_italic = getFont("Helvetica Neue", 30, "Bold Italic");
	con_bold = getFont("Helvetica Neue", 30, "Condensed Bold");
	huge = getFont("Helvetica Neue", 70, "a");
		
//	std::string text =	"<b>ABCD</b>EFGHIJKLM\n"
//						"NOPQR<i color='#00f0f0'>STUV</i>WXYZ\n"
//						"<a color='#ff0000'>aB</a>cdefghijklm\n"
//						"nopqrstuv<bi color='#f0f000'>wxyz</bi>\n"
//						"<cb>1234567890</cb> Hello";
	
	std::string text = "hello world, lets make sure the line doesn't break in the middle of the word";
	
//	tb = addChild(new poTextBox(getWindowWidth(), getWindowHeight()));
//	tb->richText(true);
	tb = addChild(new poTextBox(300,300));
	tb->alignment(PO_ALIGN_CENTER_CENTER);
	tb->textAlignment(PO_ALIGN_CENTER_CENTER);
	tb->position = getWindowCenter();
	tb->font(reg, PO_TEXT_REGULAR);
	tb->font(italic, PO_TEXT_ITALIC);
	tb->font(bold, PO_TEXT_BOLD);
	tb->font(bold_italic, PO_TEXT_BOLD_ITALIC);
	tb->font(con_bold, "cb");
	tb->font(huge, "a");
	tb->text(text);
	tb->layout();

//	tb->drawBounds = true;

	addEvent(PO_KEY_DOWN_EVENT,this);
}

TextTestApp::~TextTestApp() {}
void TextTestApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			tb->richText(!tb->richText());
			tb->layout();
		}
	}
}
void TextTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
