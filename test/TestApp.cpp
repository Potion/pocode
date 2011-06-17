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
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -500, 400, 800, 800);
}

void cleanupApplication() {
}

poShape2D *test = NULL;
poTextBox *tb = NULL;
poImage *img = NULL;
poShape2D *canvas = NULL;

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
	poImage tmp("images/108_Dry Dock 5.png");
	poShape2D* shape = new poRectShape(&tmp);
	shape->name("dd5");
	shape->drawBounds(true);
	shape->alphaTestTextures(true);
	shape->addEvent(PO_MOUSE_MOVE_EVENT, this);
	shape->position(400,400);
	addChild(shape);
	
	test = new poRectShape(50,50);
	addChild(test);

	poFont font("Courier New", 24);
	tb = new poTextBox(300,100);
	tb->font(PO_FONT_REGULAR, &font);
	tb->position(75,10);
	addChild(tb);
	
	img = new poImage(shape->texture()->width(), shape->texture()->height(), IMAGE_32, NULL);
	canvas = new poRectShape(img);
	canvas->generateStroke(4);
	canvas->scale(.5,.5);
	canvas->position(0, 75);
	addChild(canvas);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	switch(event->type) {
		case PO_MOUSE_PRESS_EVENT:
			printf("clicked\n");
			break;
			
		case PO_MOUSE_ENTER_EVENT:
			printf("inside\n");
			break;
			
		case PO_MOUSE_LEAVE_EVENT:
			printf("outside\n");
			break;
			
		case PO_MOUSE_MOVE_EVENT:
		{
			poColor c = ((poShape2D*)event->source)->texture()->colorAtPoint(event->local_position);
			test->fillColor(c);
			
			tb->text((format("(%.2f, %.2f)\n%d %d %d %d")
					  % event->local_position.x
					  % event->local_position.y
					  % (c.R * 255)
					  % (c.G * 255)
					  % (c.B * 255)
					  % (c.A * 255)).str());
			tb->layout();
			
			img->setPixel(event->local_position, c, 5);
			canvas->placeImage(img);
			
			break;
		}
			
		case PO_KEY_DOWN_EVENT:
			if(event->keyCode == PO_DOWN_ARROW) {
				poAlignment align = (poAlignment)(event->source->getChild(0)->alignment() + 1);
				if(align == PO_ALIGN_NUM_OPTIONS)
					align = PO_ALIGN_TOP_LEFT;

				event->source->getChild(0)->alignment(align);

				img->clear();
				canvas->placeImage(img);
			}
			break;
	}
}
