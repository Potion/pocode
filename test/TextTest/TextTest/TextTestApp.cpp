#include "TextTestApp.h"

#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "SimpleDrawing.h"
#include "poResourceLoader.h"

poObject *createObjectForID(uint uid) {
	return new TextTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextTest", 100, 100, 800, 800);
}

void cleanupApplication() {
}

poTextBox *tb;
poFont *reg, *italic, *bold, *bold_italic, *con_bold, *huge;

TextTestApp::TextTestApp() {
	addModifier(new poCamera2D());
	
	reg = getFont("Helvetica Neue", "Regular");
	italic = getFont("Helvetica Neue", "Italic");
	bold = getFont("Helvetica Neue", "Bold");
	bold_italic = getFont("Helvetica Neue", "Bold Italic");
	con_bold = getFont("Helvetica Neue", "Condensed Bold");
	huge = getFont("Helvetica Neue", "a");
		
	std::string text =	"<b>ABCD</b>EFGHIJKLM\n"
						"NOPQR<i color='#00f0f0'>STUV</i>WXYZ\n"
						"<a color='#ff0000'>aBp</a><span tracking='1.5'>cdefghi</span>jklm\n"
						"nopqr<bi color='#f0f000'>stuvwxyz</bi>\n"
						"<cb>1234567890</cb> Hello";

//	std::string text = "hello world, lets make sure the line doesn't break in the middle of the pepper";
	
	tb = addChild(new poTextBox(300));
	tb->richText(true);
	tb->alignment(PO_ALIGN_BOTTOM_LEFT);
	tb->textAlignment(PO_ALIGN_LEFT);
	tb->position = getWindowCenter();
	tb->font(reg, PO_TEXT_REGULAR);
	tb->font(italic, PO_TEXT_ITALIC);
	tb->font(bold, PO_TEXT_BOLD);
	tb->font(bold_italic, PO_TEXT_BOLD_ITALIC);
	tb->font(con_bold, "cb");
	tb->font(huge, "a");
	tb->leading(.85f);
	tb->text(text);
	tb->textSize(30);
	tb->layout();
	
//	tb->drawBounds = PO_TEXT_BOX_STROKE_LINE;

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
