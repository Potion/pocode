#include "WindowApp.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poImageShape.h"
#include "poShapeBasics2D.h"

#include <iostream>

WindowApp::WindowApp() {
	addModifier(new poCamera2D());
	
	poShape2D *shp = new poRectShape(100,100);
	shp->position.set(150,150,0);
	shp->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "square");
	addChild(shp);

	poImageShape *imgShp = new poImageShape("thing2.png", true);
	imgShp->position.set(100,100,0);
	imgShp->setAlphaTest(true);
	imgShp->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this, "clover");
	addChild(imgShp);

	poFont *font = poGetFont("c:\\windows\\fonts\\arial.ttf");

	poTextBox *tb = new poTextBox(200,50);
	tb->setFont(font);
	tb->setTextSize(30);
	tb->setText("hello world");
	tb->textColor = poColor::red;
	tb->useAutoAdjustHeight(true);
	tb->doLayout();
	tb->drawBounds = 1;
	tb->position.set(0,0,0);
	tb->positionTween.set(poPoint(0,100,0)).setRepeat(PO_TWEEN_REPEAT_PINGPONG).setDuration(3).setTweenFunction(PO_TWEEN_LINEAR_FUNC).start();
	addChild(tb);

	addEvent(PO_KEY_DOWN_EVENT, this, "hello");
}

void WindowApp::eventHandler(poEvent* e) {
	if(e->type == PO_KEY_DOWN_EVENT)
		printf("%c %c\n", e->keyChar, e->keyCode);
	else
		printf("%s\n", e->message.c_str());
}

void WindowApp::messageHandler(const std::string &msg, const poDictionary &dict) {
}
