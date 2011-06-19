#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -500, 400, 850, 850);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	addEvent(PO_KEY_DOWN_EVENT, this);
	
	poFont font("Helvetica", 15);
	
	poTextBox *tb = new poTextBox(300,300);
	tb->drawBounds(true);
	tb->text("But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness. No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful.");
	tb->font(PO_FONT_REGULAR, &font);
	tb->layout();
	addChild(tb);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyCode == PO_DOWN_ARROW) {
			poTextBox *tb = getChildAs<poTextBox>(event->source,0);
			poAlignment align = (poAlignment)(tb->textAlignment() + 1);
			if(align == PO_ALIGN_NUM_OPTIONS)
				align = PO_ALIGN_TOP_LEFT;
			tb->textAlignment(align);
			tb->layout();
		}
	}
}


