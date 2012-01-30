#include "TextTestApp.h"

#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poSimpleDrawing.h"
#include "poResourceStore.h"

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
	
//	reg = poGetFont("Helvetica Neue", "Regular");
	reg = poGetFont("Courier New", "Regular");
	italic = poGetFont("Helvetica Neue", "Italic");
	bold = poGetFont("Helvetica Neue", "Bold");
	bold_italic = poGetFont("Helvetica Neue", "Bold Italic");
	con_bold = poGetFont("Helvetica Neue", "Condensed Bold");
	huge = poGetFont("Helvetica Neue", "a");
		
//	std::string text =	"<b>ABCD</b>EFGHIJKLM\n"
//						"NOPQR<i color='#00f0f0'>STUV</i>WXYZ\n"
//						"<a color='#ff0000'>aBp</a><span tracking='1.5'>cdefghi</span>jklm\n"
//						"nopqr<bi color='#f0f000'>stuvwxyz</bi>\n"
//						"<cb>1234567890</cb> Hello";

	std::string text = "hello world, lets make sure the line doesn't break in the middle of the pepper";
	
	tb = (poTextBox*)addChild(new poTextBox(300));
//	tb->setRichText(true);
	tb->setAlignment(PO_ALIGN_BOTTOM_LEFT);
	tb->setTextAlignment(PO_ALIGN_LEFT);
	tb->position = getWindowCenter();
	tb->setFont(reg, PO_TEXT_REGULAR);
	tb->setFont(italic, PO_TEXT_ITALIC);
	tb->setFont(bold, PO_TEXT_BOLD);
	tb->setFont(bold_italic, PO_TEXT_BOLD_ITALIC);
	tb->setFont(con_bold, "cb");
	tb->setFont(huge, "a");
	tb->setLeading(.85f);
	tb->setText(text);
	tb->setTextSize(12);
	tb->textColor = poColor::white;
	tb->doLayout();
	
//	tb->drawBounds = PO_TEXT_BOX_STROKE_ALL;

	addEvent(PO_KEY_DOWN_EVENT,this);
}

TextTestApp::~TextTestApp() {}
void TextTestApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			tb->setRichText(!tb->isRichText());
			tb->doLayout();
		}
	}
}
void TextTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {}
