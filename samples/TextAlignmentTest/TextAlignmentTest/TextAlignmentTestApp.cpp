#include "TextAlignmentTestApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"

poObject *createObjectForID(uint uid) {
	return new TextAlignmentTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TextAlignmentTest", 100, 100, 700, 700);
}

void cleanupApplication() {
}

TextAlignmentTestApp::TextAlignmentTestApp() {
	addModifier(new poCamera2D(poColor::black));
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	poTextBox *tb = new poTextBox(200,200);
	tb->text("Hello World\nGoodbye World");
	tb->layout();
	tb->drawBounds(true);
	tb->position(350,350);
	addChild(tb);
}

TextAlignmentTestApp::~TextAlignmentTestApp() {
}

void TextAlignmentTestApp::eventHandler(poEvent *event) {
	if(event->keyCode == PO_DOWN_ARROW) {
		poTextBox *tb = getChildAs<poTextBox>(this,0);
		if(event->modifiers & PO_KEY_META) {
			float t = std::min(tb->tracking() + 0.01f, 3.f);
			tb->tracking(t).layout();
		}
		else {
			poAlignment a = poAlignment(tb->alignment() + 1);
			if(a == PO_ALIGN_NUM_OPTIONS)
				a = PO_ALIGN_TOP_LEFT;
			tb->alignment(a);
		}
	}
	if(event->keyCode == PO_RIGHT_ARROW) {
		poTextBox *tb = getChildAs<poTextBox>(this,0);
		if(event->modifiers & PO_KEY_META) {
			float l = std::min(tb->leading() + 0.01f, 3.f);
			tb->leading(l).layout();
		}
		else {
			poAlignment a = poAlignment(tb->textAlignment() + 1);
			if(a == PO_ALIGN_NUM_OPTIONS)
				a = PO_ALIGN_TOP_LEFT;
			tb->textAlignment(a).layout();
		}
	}
	if(event->keyCode == PO_UP_ARROW && event->modifiers & PO_KEY_META) {
		poTextBox *tb = getChildAs<poTextBox>(this,0);
		float t = std::max(0.1f, tb->tracking() - 0.01f);
		tb->tracking(t).layout();
	}
	if(event->keyCode == PO_LEFT_ARROW && event->modifiers & PO_KEY_META) {
		poTextBox *tb = getChildAs<poTextBox>(this,0);
		float l = std::max(0.1f, tb->leading() - 0.01f);
		tb->leading(l).layout();
	}
}

void TextAlignmentTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
