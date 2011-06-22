#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"

using namespace std;
using namespace boost;

class poSettingsInterface : public poObject {
public:
	typedef boost::function<void(float)> FloatCallback;
	typedef boost::function<void(poPoint)> PointCallback;

	poSettingsInterface() {
		addChild(column = new poObject());
	}
	
	void addFloat(const std::string &name, float start, FloatCallback cb, float min=0.f, float max=1.f, float step=.1f) {
		column->addChild(new Slider(name, start, cb, min, max, step));
		column->bounds(column->calculateBounds());
	}
	void addPoint(const std::string &name, poPoint start, float width, float height, PointCallback cb) {
		column->addChild(new Pointer(name, start, width, height, cb));
		column->bounds(column->calculateBounds());
	}
	
private:
	class Slider : public poObject {
	public:
		float value, min, max, step;
		FloatCallback callback;

		Slider(const std::string &n, float start, boost::function<void(float)> cb, float min, float max, float step)
		: value((start-min)/(max-min)), callback(cb), min(min), max(max), step(step)
		{ 
			name(n);
			bounds(poRect(0,0,150,20));
			addEvent(PO_MOUSE_PRESS_EVENT, this);
			addEvent(PO_MOUSE_DRAG_EVENT, this);
			
			poFont font("Helvetica", 11);

			poTextBox *tb = new poTextBox(150,20);
			tb->textAlignment(PO_ALIGN_TOP_LEFT)
				.font(PO_FONT_REGULAR, &font)
				.text(n)
				.layout();
			addChild(tb);
			
			tb = new poTextBox(150,20);
			tb->textAlignment(PO_ALIGN_BOTTOM_RIGHT)
				.font(PO_FONT_REGULAR, &font)
				.text(actualValueStr())
				.layout();
			addChild(tb);
		}
		
		float actualValue() const {
			return value * (max-min) + min;
		}
		
		std::string actualValueStr() const {
			return (boost::format("%.4f")%(actualValue())).str();
		}
		
		void draw() { 
			poRect filler = bounds();
			filler.size.x *= value;
			
			applyColor(fill);
			drawRect(filler);
			applyColor(stroke);
			drawStroke(bounds());
		}
		
		void eventHandler(poEvent *event) {
			switch(event->type) {
				case PO_MOUSE_PRESS_EVENT:
				case PO_MOUSE_DRAG_EVENT:
				{
					value = clamp(0.f, bounds().width(), event->local_position.x) / bounds().width();
					poTextBox *tb = getChildAs<poTextBox>(this, 1);
					tb->text(actualValueStr()).layout();
					callback(actualValue());
					break;
				}
			}
		}
	};
	
	class Pointer : public poObject {
	public:
		poPoint value;
		float width, height;
		PointCallback callback;
		
		Pointer(const std::string &n, poPoint start, float width, float height, PointCallback cb)
		: value(start), width(width), height(height), callback(cb)
		{
			name(n);
			bounds(poRect(0,0,150,150));
			addEvent(PO_MOUSE_PRESS_EVENT, this);
			addEvent(PO_MOUSE_DRAG_EVENT, this);

			poFont font("Helvetica", 11);
			
			poTextBox *tb = new poTextBox(150,20);
			tb->textAlignment(PO_ALIGN_TOP_LEFT)
				.font(PO_FONT_REGULAR, &font)
				.text(n)
				.layout();
			addChild(tb);
		}
		
		void draw() {
			applyColor(fill);
			drawRect(poRect(poPoint(value.x-2, value.y-2), poPoint(2,2)));
			
			applyColor(stroke);
			drawStroke(bounds());
		}
		
		void eventHandler(poEvent *event) {
			
		}
	};
	
	static const poColor stroke;
	static const poColor fill;
	
	poObject *column;
};

const poColor poSettingsInterface::stroke(.5,.5,.5);
const poColor poSettingsInterface::fill(.8,.8,.8);

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", -1680, 0, 500, 500);
}

void cleanupApplication() {
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
//	poShape2D *shape = new poRectShape(200,200);
//	shape->alignment(PO_ALIGN_CENTER_CENTER);
//	shape->position(getWindowWidth()/2.f, getWindowHeight()/2.f);
//	addChild(shape);
//	
//	poSettingsInterface *interface = new poSettingsInterface();
//	interface->addFloat("Float Variable", 0.f, 
//						boost::bind(&poObject::rotation, shape, _1),
//						0, 360, 10);
//	addChild(interface);
	
	poTextBox *rect = new poTextBox(200,200);
	rect->text("hello world").layout().drawBounds(true);
	rect->position(getWindowWidth()/2, getWindowHeight()/2);
	addChild(rect);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
	if(event->keyCode == PO_DOWN_ARROW) {
		poObject *obj = getChild(0);
		poAlignment align = poAlignment(obj->alignment() + 1);
		if(align == PO_ALIGN_NUM_OPTIONS)
			align = PO_ALIGN_TOP_LEFT;
		obj->alignment(align);
	}
	else if(event->keyCode == PO_RIGHT_ARROW) {
		poTextBox *tb = getChildAs<poTextBox>(this, 0);
		poAlignment align = poAlignment(tb->textAlignment() + 1);
		if(align == PO_ALIGN_NUM_OPTIONS)
			align = PO_ALIGN_TOP_LEFT;
		tb->textAlignment(align).layout();
	}
}


