
#include <boost/format.hpp>

#include <cinder/app/AppBasic.h>
#include <cinder/gl/Texture.h>
#include <cinder/ImageIo.h>

#include "poObject.h"
#include "poShape2D.h"

using namespace ci;
using namespace ci::app;

// thanks chipmunk!
static poColor colorFromPointer(void *ptr) {
	unsigned long val = (long)ptr;
	
	// hash the pointer up nicely
	val = (val+0x7ed55d16) + (val<<12);
	val = (val^0xc761c23c) ^ (val>>19);
	val = (val+0x165667b1) + (val<<5);
	val = (val+0xd3a2646c) ^ (val<<9);
	val = (val+0xfd7046c5) + (val<<3);
	val = (val^0xb55a4f09) ^ (val>>16);
	
	GLfloat r = (((val>>0) & 0xFF) * .5f + 67.f) / 255.f;
	GLfloat g = (((val>>8) & 0xFF) * .5f + 67.f) / 255.f;
	GLfloat b = (((val>>16) & 0xFF) * .5f + 67.f) / 255.f;
	
	return poColor(r,g,b);
}

class TestObj : public poShape2D {
public:
	virtual bool eventHandler(poEvent* event) {
		if(event->type == PO_MOUSE_PRESS_EVENT) {
			printf("%s: press\n", name.c_str());
			return true;
		}
		else if(event->type == PO_MOUSE_RELEASE_EVENT) {
			printf("%s: release\n", name.c_str());
			return true;
		}
		else if(event->type == PO_MOUSE_DOWN_EVENT) {
			printf("%s: down\n", name.c_str());
		}
		else if(event->type == PO_MOUSE_UP_EVENT) {
			printf("%s: up\n", name.c_str());
		}
		else if(event->type == PO_MOUSE_MOVE_EVENT) {
			printf("%s: move\n", name.c_str());
		}
		else if(event->type == PO_MOUSE_DRAG_EVENT) {
			printf("%s: drag\n", name.c_str());
			return true;
		}
		else if(event->type == PO_KEY_RELEASE_EVENT) {
			printf("%s: key release\n", name.c_str());
			return true;
		}
		else if(event->type == PO_KEY_PRESS_EVENT) {
			printf("%s: key press\n", name.c_str());
			return true;
		}
		else if(event->type == PO_MOUSE_ENTER_EVENT) {
			printf("%s: mouse enter\n", name.c_str());
			return true;
		}
		else if(event->type == PO_MOUSE_LEAVE_EVENT) {
			printf("%s: mouse leave\n", name.c_str());
			return true;
		}
		return false;
	}
	virtual void messageHandler(const std::string &msg, const poDictionary &dict=poDictionary()) {
		printf("%s\n", msg.c_str());
	}
};

class poFramework4App
:	public AppBasic 
{
public:
	void prepareSettings(Settings *settings);
	
	void setup();
	void shutdown();
	
	void mouseUp(MouseEvent e);
	void mouseDown(MouseEvent e);
	void mouseMove(MouseEvent e);
	void mouseDrag(MouseEvent e);
	
	void keyUp(KeyEvent e);
	void keyDown(KeyEvent e);
	
	void touchesBegan(TouchEvent e);
	void touchesMoved(TouchEvent e);
	void touchesEnded(TouchEvent e);
	
	void resizeEvent(ResizeEvent e);

	void update();
	void draw();
	
	void tweenDone();
	
	// return how long since the previous frame
	double getElapsedTimeSinceLastFrame();
	
	double last_frame_time;
	poObject *root;
	
	std::vector<poTween> tweens;
};

void poFramework4App::prepareSettings(Settings *settings) {
	settings->setWindowSize(1024, 768);
	settings->setFrameRate(60.f);
	settings->setTitle("poFramework4");
}

void poFramework4App::setup() {
	gl::enableAlphaBlending();
	last_frame_time = 0.0;
	
	poRandSeed();
	
	root = new poObject();
	boost::format format("obj %d");
	
	for(int j=0; j<5; j++) {
		poShape2D *shape = new TestObj();
		shape->name = (format % j).str();

//		float rad = poRand(30,60);
//		for(int i=0; i<100; i++) {
//			float angle = i / 99.f * M_2PI;
//			shape->lineTo(poPoint(cosf(angle)*rad, sinf(angle)*rad));
//		}
		shape->moveTo(poPoint(0,0));
		shape->lineTo(poPoint(100,0));
		shape->lineTo(poPoint(100,100));
		shape->lineTo(poPoint(0,100));
		shape->close();
		
		float xpos = 110 * j + 100;
		float ypos = 100;
		
		shape->fillColor = colorFromPointer(shape);
		shape->position.set(xpos, ypos, 0);
		shape->setAlignment(PO_ALIGN_TOP_LEFT);
		
		shape->addEvent(PO_MOUSE_PRESS_EVENT);
		shape->addEvent(PO_MOUSE_RELEASE_EVENT);
		shape->addEvent(PO_MOUSE_DRAG_EVENT);
		shape->addEvent(PO_KEY_PRESS_EVENT);
		shape->addEvent(PO_KEY_RELEASE_EVENT);
		shape->addEvent(PO_MOUSE_ENTER_EVENT);
		shape->addEvent(PO_MOUSE_LEAVE_EVENT);
		shape->calculateBounds();
		
		shape->tween(PO_OBJECT_PROP_POSITION, PO_TWEEN_LINEAR, PO_TWEEN_EASE_IN, 
					 poPoint(xpos,ypos+300), 5, j, PO_TWEEN_REPEAT_PINGPONG, 1);
		shape->tween(PO_OBJECT_PROP_ALPHA, PO_TWEEN_LINEAR, PO_TWEEN_EASE_IN,
					 0.f, 2.5f, j, PO_TWEEN_REPEAT_PINGPONG);
		shape->tween(PO_OBJECT_PROP_ROTATION, PO_TWEEN_LINEAR, PO_TWEEN_EASE_IN,
					 360.f, 5.f, j, PO_TWEEN_REPEAT_PINGPONG);
		root->addChild(shape);
	}
}

void poFramework4App::shutdown() {
	delete root;
}

void poFramework4App::mouseUp(MouseEvent e) {
	poEvent event;
	event.x = e.getX();
	event.y = getWindowHeight() - e.getY();
	event.modifiers = e.getModifiers();

	event.type = PO_MOUSE_UP_EVENT;
	root->_broadcastEvent(&event);
	
	if(poEvent::mouse_receiver != NULL) {
		event.type = PO_MOUSE_RELEASE_EVENT;
		poEvent::mouse_receiver->_processEvent(&event);
		poEvent::mouse_receiver = NULL;
	}
	
	poShape2D *shape = static_cast<poShape2D*>(root->getChild(0));
	poAlignment align = shape->align;
	align = poAlignment((int)align+1);
	if(align == PO_ALIGN_NUM_OPTIONS)
		align = PO_ALIGN_TOP_LEFT;
	shape->setAlignment(align);
	static_cast<poShape2D*>(root->getChild(1))->setAlignment(align);
}

void poFramework4App::mouseDown(MouseEvent e) {
	poEvent event;
	event.x = e.getX();
	event.y = getWindowHeight() - e.getY();
	event.modifiers = e.getModifiers();
	
	event.type = PO_MOUSE_DOWN_EVENT;
	root->_broadcastEvent(&event);
	
	// reset the key receiver
	poEvent::key_receiver = NULL;
	// figure out if anyone is there
	poObject *obj = root->objectUnderMouse(event.x, event.y);
	if(obj) {
		event.type = PO_MOUSE_PRESS_EVENT;
		// send the event directy to the object
		if(obj->_processEvent(&event)) {
			poEvent::mouse_receiver = obj;
			// and set the key handler if it takes key events
			if(obj->hasEvents(PO_KEY_PRESS_EVENT) ||
			   obj->hasEvents(PO_KEY_RELEASE_EVENT))
			{
				poEvent::key_receiver = obj;
			}
		}
	}
}

void poFramework4App::mouseMove(MouseEvent e) {
	poEvent event;
	event.x = e.getX();
	event.y = getWindowHeight() - e.getY();
	event.modifiers = e.getModifiers();
	
	event.type = PO_MOUSE_MOVE_EVENT;
	root->_broadcastEvent(&event);
	
	poObject *obj = root->objectUnderMouse(event.x, event.y);
	if(obj) {
		// we're changing hover targets
		if(obj != poEvent::mouse_hover) {
			// if there was a previous hover target
			if(poEvent::mouse_hover != NULL) {
				// let them know the mouse is leaving them
				event.type = PO_MOUSE_LEAVE_EVENT;
				obj->_processEvent(&event);
			}
			// and tell the new hover target
			event.type = PO_MOUSE_ENTER_EVENT;
			if(obj->_processEvent(&event))
			   poEvent::mouse_hover = obj;
		}
	}
	else if(poEvent::mouse_hover != NULL) {
		// there's nothing under the mouse but we still have a hover target
		event.type = PO_MOUSE_LEAVE_EVENT;
		poEvent::mouse_hover->_processEvent(&event);
		poEvent::mouse_hover = NULL;
	}
}

void poFramework4App::mouseDrag(MouseEvent e) {
	poEvent event;
	event.x = e.getX();
	event.y = getWindowHeight() - e.getY();
	event.modifiers = e.getModifiers();

	if(poEvent::mouse_receiver != NULL) {
		event.type = PO_MOUSE_DRAG_EVENT;
		poEvent::mouse_receiver->_processEvent(&event);
	}
}

void poFramework4App::keyUp(KeyEvent e) {
	poEvent event;
	event.keyCode = e.getCode();
	event.keyChar = e.getChar();
	event.modifiers = e.getModifiers();

	event.type = PO_KEY_UP_EVENT;
	root->_broadcastEvent(&event);
	
	if(poEvent::key_receiver != NULL) {
		event.type = PO_KEY_RELEASE_EVENT;
		poEvent::key_receiver->_processEvent(&event);
	}
}

void poFramework4App::keyDown(KeyEvent e) {
	poEvent event;
	event.keyCode = e.getCode();
	event.keyChar = e.getChar();
	event.modifiers = e.getModifiers();
	
	event.type = PO_KEY_DOWN_EVENT;
	root->_broadcastEvent(&event);
	
	if(poEvent::key_receiver != NULL) {
		event.type = PO_KEY_PRESS_EVENT;
		poEvent::key_receiver->_processEvent(&event);
	}
}

void poFramework4App::touchesBegan(TouchEvent e) {}
void poFramework4App::touchesMoved(TouchEvent e) {}
void poFramework4App::touchesEnded(TouchEvent e) {}
void poFramework4App::resizeEvent(ResizeEvent e) {}

void poFramework4App::update(){
	root->_updateTree();
	last_frame_time = getElapsedSeconds();
}

void poFramework4App::draw(){
	gl::clear(ColorA(0,0,0,0));
	gl::setMatricesWindow(getWindowSize());
	
	root->_drawTree();
}

double poFramework4App::getElapsedTimeSinceLastFrame() {
	return getElapsedSeconds() - last_frame_time;
}

CINDER_APP_BASIC( poFramework4App, RendererGl )
