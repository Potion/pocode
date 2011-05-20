/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#include "poObject.h"

static int master_draw_order = 1;
static std::vector<float> alpha_stack(1, 1.f);

poObject::poObject() 
:	parent_(NULL)
,	name_("")
,	alpha_(1.f)
,	scale_(1.f, 1.f, 1.f)
,	position_(0.f, 0.f, 0.f)
,	rotation_(0.f)
,	rotation_axis(0.f, 0.f, 1.f)
,	offset_(0.f, 0.f, 0.f)
,	bounds_(0.f, 0.f, 0.f, 0.f)
,	alignment_(PO_ALIGN_TOP_LEFT)
,	visible_(true)
,	events(PO_LAST_EVENT)
,	matrix_order(PO_MATRIX_ORDER_TRS)
,	draw_order(0)
,	position_tween(&position_)
,	scale_tween(&scale_)
,	alpha_tween(&alpha_)
,	rotation_tween(&rotation_)
,	true_alpha(1.f)
{}

poObject::poObject(const std::string &name)
:	parent_(NULL)
,	name_(name)
,	alpha_(1.f)
,	scale_(1.f, 1.f, 1.f)
,	position_(0.f, 0.f, 0.f)
,	rotation_(0.f)
,	rotation_axis(0.f, 0.f, 1.f)
,	offset_(0.f, 0.f, 0.f)
,	bounds_(0.f, 0.f, 0.f, 0.f)
,	alignment_(PO_ALIGN_TOP_LEFT)
,	visible_(true)
,	events(PO_LAST_EVENT)
,	matrix_order(PO_MATRIX_ORDER_TRS)
,	draw_order(0)
,	position_tween(&position_)
,	scale_tween(&scale_)
,	alpha_tween(&alpha_)
,	rotation_tween(&rotation_)
,	true_alpha(1.f)
{}

poObject::~poObject() {
	removeAllChildren(true);
}

void poObject::draw() {}
void poObject::update() {}
void poObject::eventHandler(poEvent *event) {}
void poObject::messageHandler(const std::string &msg, const poDictionary& dict) {}

int poObject::addEvent(int eventType, poObject *sink, std::string message, const poDictionary& dict) {
	if(!sink)
		sink = this;
	return poEventCenter::get()->registerForEvent(eventType, this, sink, message, dict);
}

void poObject::removeEvent(int event_id) {
	poEventCenter::get()->removeEvent(event_id);
}

void poObject::preDraw() {}
void poObject::postDraw() {}

void poObject::addChild(poObject* obj) {
	obj->parent_ = this;
	children.push_back(obj);
}

void poObject::addChild(poObject *obj, int idx) {
	obj->parent_ = this;
	children.insert(children.begin()+idx, obj);
}

bool poObject::removeChild(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	bool found = iter != children.end();
	
	if(found) {
		(*iter)->parent_ = NULL;
		children.erase(iter);
	}
	
	return found;
}

bool poObject::removeChild(int idx, bool and_delete) {
	if(idx < 0 || idx >= children.size())
		return false;

	children[idx]->parent_ = NULL;

	if(and_delete)
		delete children[idx];
	
	children.erase(children.begin()+idx);
	
	return true;
}

void poObject::removeAllChildren(bool and_delete) {
	BOOST_FOREACH(poObject* obj, children) {
		obj->parent_ = NULL;
		if(and_delete)
			delete obj;
	}
	children.clear();
}

int poObject::numChildren() const {
	return (int)children.size();
}

poObject* poObject::getChild(int idx) {
	if(idx < 0 || idx >= children.size())
		return NULL;
	return *(children.begin()+idx);
}

poObject* poObject::getChild(const std::string &name) {
	poObjectVec::iterator iter = std::find_if(children.begin(), children.end(), boost::bind(&poObject::name, _1) == name);
	if(iter != children.end())
		return *iter;
	return NULL;
}

int poObject::getChildIndex(poObject* obj) {
	poObjectVec::iterator iter = std::find(children.begin(), children.end(), obj);
	if(iter != children.end())
		return (int)std::distance(children.begin(), iter);
	return INVALID_INDEX;
}

void poObject::moveChildToFront(poObject* child) {
	removeChild(child);
	addChild(child, 0);
}

void poObject::moveChildToBack(poObject* child) {
	removeChild(child);
	addChild(child);
}

void poObject::moveChildForward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, std::max(idx-1, 0));
}

void poObject::moveChildBackward(poObject* child) {
	int idx = getChildIndex(child);
	removeChild(child);
	addChild(child, idx+1);
}


void    poObject::addModifier(poObjectModifier* mod)
{
    modifiers.push_back( mod );
}

bool poObject::removeModifier(poObjectModifier* mod) {
	poObjectModifierVec::iterator found = std::find(modifiers.begin(), modifiers.end(), mod);
	if(found != modifiers.end()) {
		modifiers.erase(found);
		return true;
	}
	return false;
}

void    poObject::removeAllModifiers(bool and_delete)
{
	if(and_delete) {
		BOOST_FOREACH(poObjectModifier* mod, modifiers) {
			delete mod;
		}
	}
    modifiers.clear();
}


bool poObject::pointInside(poPoint point, bool localize) {
	if(!visible())
		return false;
	
	if(localize)
		point = globalToLocal(point);
	
	return bounds().contains(point);
}

bool poObject::pointInside(float x, float y, float z, bool localize) {
	return pointInside(poPoint(x,y,z),localize);
}

poPoint poObject::globalToLocal(poPoint point) const {
	return matrices.globalToLocal(point);
}

poPoint poObject::localToGlobal(poPoint point) const {
	return matrices.localToGlobal(point);
}

poPoint poObject::objectToLocal(poObject* obj, poPoint point) const {
	point = obj->localToGlobal(point);
	return globalToLocal(point);
}

void poObject::setAlignment(poAlignment align) {
	alignment_ = align;
}

poRect poObject::calculateBounds(bool include_children) {
	bounds_ = poRect(0,0,0,0);
	
	if(include_children) {
		BOOST_FOREACH(poObject* obj, children) {
			obj->calculateBounds(include_children);
			poRect obj_b = obj->bounds();
			
			bounds_.include(objectToLocal(obj, obj_b.bottomLeft()));
			bounds_.include(objectToLocal(obj, obj_b.bottomRight()));
			bounds_.include(objectToLocal(obj, obj_b.topLeft()));
			bounds_.include(objectToLocal(obj, obj_b.topRight()));
		}
	}
	
	return bounds_;
}

poObject*		poObject::parent() {return parent_;}

std::string		poObject::name() const {return name_;}
poObject&		poObject::name(const std::string &str) {name_ = str; return *this;}

float			poObject::alpha() const {return alpha_;}
poObject&		poObject::alpha(float f) {alpha_ = f; return *this;}

poPoint			poObject::scale() const {return scale_;}
poObject&		poObject::scale(poPoint pt) {scale_ = pt; return *this;}
poObject&		poObject::scale(float x, float y, float z) {return scale(poPoint(x,y,z));}

poPoint			poObject::position() const {return position_;}
poObject&		poObject::position(poPoint p) {position_ = p; return *this;}
poObject&		poObject::position(float x, float y, float z) {return position(poPoint(x,y,z));}

float			poObject::rotation() const {return rotation_;}
poObject&		poObject::rotation(float f) {rotation_ = f; return *this;}

poPoint			poObject::rotationAxis() const {return rotation_axis;}
poObject&		poObject::rotationAxis(poPoint p) {rotation_axis = p; return *this;}
poObject&		poObject::rotationAxis(float x, float y, float z) {return rotationAxis(poPoint(x,y,z));}

poPoint			poObject::offset() const {return offset_;}
poObject&		poObject::offset(poPoint p) {offset_ = p; return *this;}
poObject&		poObject::offset(float x, float y, float z) {return offset(poPoint(x,y,z));}

poRect			poObject::bounds() const {return bounds_;}
poObject&		poObject::bounds(poRect r) {bounds_ = r; return *this;}

poAlignment		poObject::alignment() const {return alignment_;}

bool			poObject::visible() const {return visible_;}
poObject&		poObject::visible(bool b) {visible_ = b; return *this;}

poMatrixOrder	poObject::matrixOrder() const {return matrix_order;}
poObject&		poObject::matrixOrder(poMatrixOrder o) {matrix_order = o; return *this;}

int				poObject::drawOrder() const {return draw_order;}

void poObject::_drawTree() {
	if(!visible())
		return;
	
	pushObjectMatrix();

    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setUp( this );
    }
    
	preDraw();
	
	draw();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_drawTree();
	}

	postDraw();
    
    BOOST_FOREACH(poObjectModifier* mod, modifiers) {
        mod->setDown( this );
    }
	
	popObjectMatrix();
}

void poObject::_updateTree() {
	if(!visible())
		return;
	
	updateAllTweens();
	update();
	
	BOOST_FOREACH(poObject* obj, children) {
		obj->_updateTree();
	}
}

void poObject::_broadcastEvent(poEvent* event) {
	if(!visible())
		return;
	// make sure its even a valid event
	if(event->type < 0 || event->type >= PO_LAST_EVENT)
		return;

	// send it to all our children
	BOOST_REVERSE_FOREACH(poObject* obj, children) {
		obj->_broadcastEvent(event);
	}

	// localize the point only once
	poPoint local_point = globalToLocal(event->position);
	
	// handle every event like this we have
	BOOST_FOREACH(poEvent *e, events[event->type]) {
		localizeEvent(e, event, local_point);
		eventHandler(e);
	}
}

void poObject::updateAllTweens() {
	position_tween.update();
	scale_tween.update();
	alpha_tween.update();
	rotation_tween.update();
}

void poObject::pushObjectMatrix() {
	glPushMatrix();

	if(parent_) {
		true_alpha = parent_->true_alpha * alpha_;
		draw_order = parent_->draw_order + 1;
	}
	else {
		true_alpha = alpha_;
	}
	
	// now move depending on the matrix order
	switch(matrix_order) {
		case PO_MATRIX_ORDER_TRS:
			glTranslatef(position_.x, position_.y, position_.z);
			glRotatef(rotation_, rotation_axis.x, rotation_axis.y, rotation_axis.z);
			glScalef(scale_.x, scale_.y, scale_.z);
			break;
			
		case PO_MATRIX_ORDER_RST:
			glRotatef(rotation_, rotation_axis.x, rotation_axis.y, rotation_axis.z);
			glScalef(scale_.x, scale_.y, scale_.z);
			glTranslatef(position_.x, position_.y, position_.z);
			break;
	}
	
	// translate to the offset
	glTranslatef(offset_.x, offset_.y, offset_.z);

	matrices.capture();
}

void poObject::popObjectMatrix() {

	// and reset gl
	glPopMatrix();
}

void poObject::localizeEvent(poEvent *local_event, poEvent *global_event, poPoint localized_pt) {
	local_event->position = global_event->position;
	local_event->local_position = localized_pt;
	local_event->keyChar = global_event->keyChar;
	local_event->keyCode = global_event->keyCode;
	local_event->modifiers = global_event->modifiers;
	local_event->uid = global_event->uid;
	local_event->timestamp = global_event->timestamp;
	local_event->previous_position = global_event->previous_position;
	// don't touch the message or the dictionary
}
