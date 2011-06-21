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
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -1680, 0, 800, 1200);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	addEvent(PO_WINDOW_RESIZED_EVENT, this);

	poObject *holder = new poObject();
	holder->position(100,100);
	addChild(holder);
	
	for(int col=0; col<35; col++) {
		for(int row=0; row<25; row++) {
			poShape2D *rect = new poRectShape(20,20);
			rect->alignment(PO_ALIGN_CENTER_CENTER);
			rect->fillColor(hashPointerForColor(rect));
			rect->position(row*25,col*25);
			rect->rotation_tween.set(360).setTweenFunction(linearFunc).setDuration(10).setRepeat(PO_TWEEN_REPEAT_REGULAR).setDelay(row*col).start();
			rect->scale_tween.set(poPoint(.2f,.2f)).setTweenFunction(linearFunc).setDuration(4).setRepeat(PO_TWEEN_REPEAT_PINGPONG).setDelay(row*col).start();
			holder->addChild(rect);
		}
	}
	
	poFont font("Helvetica", 20);
	poTextBox *tb = new poTextBox(200,50);
	tb->font(PO_FONT_REGULAR, &font);
	addChild(tb);
	
//	poShape2D *img = new poRectShape("images/testimg.png");
//	img->rotation_tween.set(360).setTweenFunction(linearFunc).setDuration(10).setRepeat(PO_TWEEN_REPEAT_REGULAR).start();
//	img->alignment(PO_ALIGN_CENTER_CENTER);
//	img->addEvent(PO_MOUSE_PRESS_EVENT, this, "", poDictionary().setBool("scaled",false));
//	addChild(img);
}

void TestObj::draw() {
}

void TestObj::update() {
	poTextBox *tb = getChildAs<poTextBox>(this, 1);
	tb->text((boost::format("% 8.2f")%getWindowFramerate()).str());
	tb->layout();
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_WINDOW_RESIZED_EVENT:
//			event->source->getChild(1)->position(getWindowWidth()/2, getWindowHeight()/2);
			break;
			
		case PO_MOUSE_PRESS_EVENT:
		{
			poDictionary &dict = event->dict;
			if(dict.getBool("scaled")) {
				event->source->scale_tween.set(poPoint(1,1,1)).setDuration(5).start();
				dict.setBool("scaled", false);
			}
			else {
				event->source->scale_tween.set(poPoint(.5,.5,1)).setDuration(5).start();
				dict.setBool("scaled", true);
			}
			break;
		}
	}
}


