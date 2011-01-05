
#include "poObject.h"
#include "poScene.h"


// INITIALIZE STATIC CLASS VARIABLES
float		poObject::masterAlphaControl = 1.f;
int			poObject::masterAlphaStackHeight = 0;
float		poObject::masterAlphaStack[256];
bool		poObject::masterDisableMatrixCapture = false;
bool		poObject::doDebugDraw = false;

poObjectVector	poObject::registeredClassSet;


// HELPER FUNCTIONS
void	localizeEvent(poEvent* eIn, poEvent* eOut);

poObject::poObject()
:	name(""), parent(NULL),
	mouseCurrentlyInside(false),
	matrixOrder(PO_TRS_MATRIX),
	visible(true), alpha(1.0f), rotation(0.0f), rotationAxis(0,0,1), scale(poPoint(1,1,1)),
	transformChildren(true)
{
	construct();
}

poObject::poObject(const char* n)
:	name(n), parent(NULL),
	mouseCurrentlyInside(false),
	matrixOrder(PO_TRS_MATRIX),
	visible(true), alpha(1.0f), rotation(0.0f), rotationAxis(0,0,1), scale(poPoint(1,1,1)),
	transformChildren(true)
{
	construct();
}

poObject::poObject(const poObject& rhs)
:	name(rhs.getName()), parent(NULL),
	mouseCurrentlyInside(false),
	matrixOrder(PO_TRS_MATRIX),
	visible(rhs.visible), alpha(rhs.alpha), rotation(rhs.rotation), 
	position(rhs.position), scale(rhs.position), rotationAxis(rhs.rotationAxis),
	transformChildren(rhs.transformChildren)
{
	construct();
}

poObject& poObject::operator=(const poObject& rhs)
{
	setName(rhs.getName());
	visible = rhs.visible;
	alpha = rhs.alpha;
	position = rhs.position;
	scale = rhs.position;
	rotation = rhs.rotation;
	rotationAxis = rhs.rotationAxis;
	
	return *this;
}

void poObject::construct()
{
	registry.addSeparator( getClassName() );
	registry.add("name", &name);
	registry.add("visible",  &(visible));
	registry.add("alpha",  &(alpha));	
	
	registry.add("position", &(position));
	registry.add("scale", &(scale));
	registry.add("rotation",  &(rotation));
	registry.add("rotationAxis", &(rotationAxis));
	registry.add("transformChildren", &(transformChildren));
	
	setName(getClassName());
}

poObject::~poObject()
{
	for(int i=0; i<children.size(); i++)
		delete children[i];
	children.clear();

	for( int i=0; i<modifiers.size(); i++ )
		delete modifiers[i];
	modifiers.clear();
			
	for(int i=0; i<events.size(); i++) 
	{
		for(int j=0; j<events[i].size(); j++)
			delete events[i][j];
		events[i].clear();
	}
	events.clear();
}

poObject* poObject::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poObject* P = copyIntoObject;
	if ( P==NULL )
		P = new poObject();
	
	P->setName( getName() );
	P->setProperties( getProperties() );
	return P;
}


void poObject::init()
{
	for( int i=0; i<modifiers.size(); i++ )
	{
		modifiers[i]->init();
		modifiers[i]->initWithObject( this );
	}
}

void	poObject::draw()
{
	if ( ! visible )
		return;

	if ( poObject::doDebugDraw )
		debugDraw();
}

void	poObject::debugDraw()
{
	if ( parent == NULL )	// don't draw for root
		return;
	
	glColor3f( 0,1,0 );
	glRectf( -5,-0.5, 5,0.5 );
	glRectf( -0.5,-5, 0.5,5 );
	
	// draw bounding box
	if ( numChildren() > 0 )
	{
		boundingBox.initFarBounds();
		calculateRelativeBounds( this, boundingBox );
		
		glColor4f( 0,1,0,0.1);
		glBegin( GL_LINE_LOOP );
		glVertex2f( boundingBox.left, boundingBox.top );
		glVertex2f( boundingBox.left, boundingBox.bottom );
		glVertex2f( boundingBox.right, boundingBox.bottom );
		glVertex2f( boundingBox.right, boundingBox.top );
		glEnd();
		
		glRectf( boundingBox.left-5, boundingBox.bottom-5, boundingBox.left+5, boundingBox.bottom+5 );
	}
	
}

void poObject::setName(const char* n)
{
	name = n;
}

poObjectProperties poObject::getProperties()
{	
	poObjectProperties P( position, alpha, scale, rotation, rotationAxis, visible );
	return P;
}

void poObject::setProperties( const poObjectProperties& P )
{
	position = P.position;
	alpha = P.alpha;
	scale = P.scale;
	rotation = P.rotation;
	rotationAxis = P.rotationAxis;
	visible = P.visible;
}

void poObject::initTree()
{
	if ( serialize == false )
		return;
		
	init();

	for(int i=0; i<children.size(); i++)
		children[i]->initTree();
}

void poObject::drawTree()
{
	if( !visible )
		return;
	
	if(transformChildren)
		pushObjectMatrix();
		
	poObjectModifierVector::iterator iter;
	for(iter=modifiers.begin(); iter!=modifiers.end(); iter++)
		if( (*iter)->enabled )
			(*iter)->setup(this);
	
	draw();
	
	for(int i=0; i<children.size(); i++)
		children[i]->drawTree();
	
	for(iter=modifiers.begin(); iter!=modifiers.end(); iter++)
		if( (*iter)->enabled )
			(*iter)->setdown(this);
		
	if(transformChildren)
		popObjectMatrix();

}

void poObject::updateTree()
{
	if(!visible)
		return;

	update();
	
	for(int i=0; i<children.size(); i++)
		children[i]->updateTree();
}

void poObject::addChild(poObject* obj) 
{
	children.push_back(obj);
	obj->parent = this;
}

bool poObject::insertChild(poObject* obj, int index)
{
	if(index < 0 || index > children.size())
	{
		printf("WARNING: poObject->insertChild: trying to insert child beyond end of childlist (%d / %d)\n", index, numChildren());
		return false;
	}

	poObjectVector::iterator pos = children.begin() + index;
	children.insert(pos, obj);
	obj->parent = this;
	return true;
}

poObject* poObject::removeChild(poObject* obj)
{
	poObjectVector::iterator found = std::find(children.begin(), children.end(), obj);
	if(found != children.end()) 
	{
		(*found)->parent = NULL;
		children.erase(found);
		return obj;
	}
	
	printf("WARNING: poObject->removeChild: tried to remove non-existant child\n");
	return NULL;
}

poObject* poObject::removeChildAt(int index) 
{
	poObject* obj = getChild(index);
	if(obj) 
		children.erase(children.begin() + index);
	obj->parent = NULL;
	return obj;
}

poObject* poObject::getChild(int index)
{
	if(index < 0 || index >= children.size()) 
	{
		printf("WARNING: poObject->getChild: index %d out of bounds\n", index);
		return NULL;
	}
	return children[index];
}

poObject* poObject::getChildByName(const char* N)
{
	for(int i=0; i<children.size(); i++)
		if( children[i]->name == N )
			return children[i];
	
	//printf("WARNING: poObject->getChildByName: no child by name of '%s'\n", name.c_str());
	return NULL;
}

int poObject::getChildIndex(poObject* obj)
{
	for(int i=0; i<children.size(); i++)
		if(children[i] == obj)
			return i;
			
	printf("WARNING: poObject->getChildIndex: trying to index non-existant child\n");
	return -1;
}

void poObject::addModifier(poObjectModifier* mod, int idx)
{
	///poObjectModifierVector::iterator iter = modifiers.begin() + idx;
	//modifiers.insert(iter, mod);
	modifiers.push_back( mod );
}

void poObject::removeModifier(int idx)
{
	poObjectModifierVector::iterator iter = modifiers.begin() + idx;
	if(iter != modifiers.end())
		modifiers.erase(iter);
}

poObjectModifier*	poObject::getModifier( char* modType )
{
	if ( modType == NULL )
		return NULL;
		
	for( int i=0; i<modifiers.size(); i++ )
		if ( strcmp( modifiers[i]->getClassName(), modType )==0 )
			return modifiers[i];
	
	return NULL;
}

void poObject::removeModifier(poObjectModifier* mod)
{
	poObjectModifierVector::iterator iter = modifiers.begin();
	for(iter; iter!=modifiers.end(); iter++)
		if(*iter == mod)
		{
			modifiers.erase(iter);
			return;
		}
}

void poObject::calculateBounds()
{
	boundingBox.left = 10000000;
	boundingBox.right = -10000000;
	boundingBox.top = -100000000;
	boundingBox.bottom = 10000000;
	
	calculateRelativeBounds( this, boundingBox );
	
	if ( numChildren()==0 )
		boundingBox = poBoundingBox();
}

void poObject::calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox )
{
	for( int i=0; i<numChildren(); i++ )
		getChild(i)->calculateRelativeBounds( relativeObj, bbox );
}

void poObject::setOrigin(poHoriOrientation ho, poVertOrientation vo) 
{}


bool poObject::pointInside(float x, float y)
{
	if( !visible || almostEqual(masterAlphaControl*alpha, 0.f, 0.01f) || matrixSet.isZero() )
		return false;

	if ( poObject::doDebugDraw )
	{
		poPoint P;
		P.set( x, y, 0 );
		globalToLocal(P.x, P.y, P.z);
		
		if ( P.x >= boundingBox.left-10 && P.x <= boundingBox.left+10 && P.y >= boundingBox.bottom-10 && P.y <= boundingBox.bottom+10 )
			return true;
	}
	
	for(int i=0; i<children.size(); i++)
		if(children[i]->pointInside(x, y))
			return true;
	
	return false;
}

void poObject::globalToLocal(poPoint& p) const { globalToLocal(p.x, p.y, p.z); }

void poObject::globalToLocal( float &mX, float &mY, float &mZ ) const
{	
	GLdouble objX, objY, objZ;						// coordinates in object space (what we're getting)
	GLdouble winX = mX, winY = mY, winZ = 0;		// coordinates in screen space (what we have)

	// printf("%f %f\n", mX, mY );

	gluUnProject( winX, winY, winZ,
				  matrixSet.modelMatrix, matrixSet.projectMatrix, matrixSet.viewport, 
				  &objX, &objY, &objZ );
	
	mX = objX;
	mY = objY;
	mZ = objZ;
}

void poObject::localToGlobal(poPoint& p) const { localToGlobal(p.x, p.y, p.z); }

void	poObject::localToGlobal( float &mX, float &mY, float &mZ ) const
{		
	GLdouble objX = mX, objY = mY, objZ = mZ;      // coordinates in object space (what we have)
	GLdouble winX, winY, winZ;					   // coordinates in screen space (what we're getting)

	gluProject( objX, objY, objZ ,
				  matrixSet.modelMatrix, matrixSet.projectMatrix, matrixSet.viewport, 
				  &winX, &winY, &winZ );
	
	mX = winX;
	mY = winY;
	mZ = winZ;
}



// EVENT HANDLING CRAP
poEvent* poObject::addEvent(int type, poObject* to)
{
	return addEvent(type, to, "");
}

poEvent* poObject::addEvent(int type, poObject* to, const char* msg, const poDictionary& dict)
{
	if(type > poEvent::NUM_EVENTS)
	{
		printf("WARNING: poObject->addEvent: trying to add event of non-existant event type %d\n", type);
		return NULL;
	}
	
	if(type >= events.size()) 
		events.resize(type+1);
	
	poEvent* event = new poEvent(type, this, to, msg, dict);
	events[type].push_back(event);
	
	return event;
}

bool poObject::hasEvents(int type)
{
	if(events.size() > type)
		return !events[type].empty();
	else if(type > poEvent::NUM_EVENTS)
		printf("WARNING: poObject->hasEvents: trying to check for non-existent event type\n");
	return false;
}

bool poObject::hasEvent(int type, poObject* to)
{
	if(type > poEvent::NUM_EVENTS)
	{
		printf("WARNING: poObject->hasEvent: trying to check for non-existent event type\n");	
		return false;
	}
		
	poEventVector::iterator iter = events[type].begin();
	while(iter!=events[type].end())
	{
		if((*iter)->receiver == to)
			return true;
		iter++;
	}
	
	return false;
}

void poObject::removeEvent(int type, poObject* to) 
{
	if(type > poEvent::NUM_EVENTS)
	{
		printf("WARNING: poEventProcessor->removeEvent: trying to remove event of non-existant event type %d\n", type);
		return;
	}

	if(!hasEvents(type)) {
		printf("WARNING: poEventProcessor->removeEvent: trying to remove unregistered event of type %d\n", type);
		return;
	}

	bool found = false;
	poEventVector::iterator iter = events[type].begin();
	while(iter!=events[type].end())
	{
		if((*iter)->receiver == to)
		{
			iter = events[type].erase(iter);
			found = true;
		} else
			iter++;
	}
	
	if(!found)
		printf("WARNING: poEventProcessor->removeEvent: no event of type %d to %s registered here\n", type, name.c_str());
}

bool poObject::processEventTree(poEvent* e)
{
	if(!visible)
		return false;

	// pass down the tree first
	for(int i=children.size()-1; i>=0; i--)
	{
		if(children[i]->processEventTree(e)) {
			// child took care of it
			return true;
		}
	}

	// notify parent whether we handled it or not
	return this->processEvent(e);
}

void	poObject::dumpEventTable()
{
	printf("DEBUG: poObject->dumpEventTable: for object %s\n", name.c_str());
	for( int i=0; i<events.size(); i++ )
		for( int j=0; j<events[i].size(); j++ )
			printf("\tevent %d, %d type: %d\n", i,j, events[i][j]->type );
}

// FILL OUT OUR LOCAL EVENT PTR BEFORE PUSHING IT TO ITS DESTINATION
void localizeEvent(poEvent* eIn, poEvent* eOut) {
	eOut->x = eIn->x;
	eOut->y = eIn->y;
	eOut->key = eIn->key;
	eOut->modifiers = eIn->modifiers;

	eOut->localX = eIn->x;
	eOut->localY = eIn->y;
	float tempZ = 0;
	eOut->receiver->globalToLocal( eOut->localX, eOut->localY, tempZ );
}
/*
	PO_EVENT=0,
	PO_TICK_EVENT,
	PO_KEY_DOWN_EVENT,
	PO_MOUSE_MOVE_EVENT,
	PO_MOUSE_DOWN_EVENT,
	PO_MOUSE_UP_EVENT,
	PO_MOUSE_DRAG_EVENT,
	// EVENTS BELOW THIS POINT REQUIRE BOUNDS CHECKING BY DEFAULT
	PO_MOUSE_PRESS_EVENT,
	PO_MOUSE_RELEASE_EVENT,
	PO_MOUSE_ENTER_EVENT,
	PO_MOUSE_LEAVE_EVENT,
	PO_MOUSE_RELEASE_OUTSIDE_EVENT */


bool poObject::processEvent(poEvent* e)
{
	// nobody wants it so lets try to handle it ourselves
	if(e->type < events.size() && !events[e->type].empty())
	{
		// if we don't have any events of this type, ignore it
		if(!hasEvents(e->type))
			return false;
		
		for(int i=0; i<events[e->type].size(); i++)
		{
			poEvent* event = events[e->type][i];
			
			// first check if its an event that must be in side us
			if ( e->checkBounds )
				mouseCurrentlyInside = pointInside(e->x, e->y);
		
			bool doProcessEvent = true;
			
			if ( event->type == PO_MOUSE_PRESS_EVENT && mouseCurrentlyInside==false )
				doProcessEvent = false;
			if ( event->type == PO_MOUSE_OVER_EVENT && mouseCurrentlyInside==false )
				doProcessEvent = false;
			if ( event->type == PO_MOUSE_RELEASE_EVENT && mouseCurrentlyInside==false )
				doProcessEvent = false;
			if ( event->type == PO_MOUSE_ENTER_EVENT && ! (event->mouseWasInside==false && mouseCurrentlyInside==true) )
				doProcessEvent = false;
			if ( event->type == PO_MOUSE_LEAVE_EVENT && ! (event->mouseWasInside==true && mouseCurrentlyInside==false) )
				doProcessEvent = false;
			if ( event->type == PO_MOUSE_RELEASE_OUTSIDE_EVENT && !(mouseCurrentlyInside==false) )
				doProcessEvent = false;

			event->mouseWasInside = mouseCurrentlyInside;
			
			if ( doProcessEvent )
			{
				localizeEvent(e, event);
				if ( event->receiver->eventHandler(event) )
					return true;
			}
			
		}
	}

	return false;
}

poXMLnode* poObject::write() 
{
	poXMLnode* node = registry.write( "PO_OBJECT" );
	node->addAttribute("type", getClassName() );
	
	poXMLnode* modifiersNode = new poXMLnode("MODIFIERS");
	for( int i=0; i<modifiers.size(); i++ )
		if ( modifiers[i]->serialize )
			modifiersNode->addChild( modifiers[i]->write() );
	node->addChild( modifiersNode );
	
	poXMLnode* kids = new poXMLnode("CHILDREN");
	for(int i=0; i<numChildren(); i++) {
		poObject* kid = getChild(i);
		if(kid->serialize)
			kids->addChild(kid->write());
	}
	node->addChild(kids);

	return node;
}

void poObject::read(poXMLnode* node)
{
	registry.read(node);
	
	poXMLnode* modifiersNode = node->getNode("MODIFIERS");
	int numMods = modifiersNode->getNumNodes();
	for(int i=0; i<numMods; i++) 
	{
		poXMLnode* modNode = modifiersNode->getNode(i);
		const char* classTypeName = modNode->getAttribute( "type" );
		poObjectModifier* theMod = poObjectModifier::generateModifier( (char*) classTypeName );
		if ( theMod != NULL )
		{
			theMod->serialize = true;
			theMod->read( modNode );
			addModifier( theMod );
		}
	}
	
	poXMLnode* kids = node->getNode("CHILDREN");
	int numKids = kids->getNumNodes();
	for(int i=0; i<numKids; i++) 
	{
		poXMLnode* kidNode = kids->getNode(i);
		const char* classTypeName = kidNode->getAttribute( "type" );
		poObject* obj = generateObject( (char*) classTypeName );
		if ( obj==NULL )
		{
			printf("ERROR: could not generate object of type: %s. Be sure to register all classes and implement getClassName() and duplicate().\n", classTypeName );
			exit(0);
			return;
		}
		obj->read( kidNode );
		obj->serialize = true;
		addChild(obj);
	}
}

poObject* poObject::generateObject( char* className )
{
	for( int i=0; i<registeredClassSet.size(); i++ )
		if ( strcmp( registeredClassSet[i]->getClassName(), className ) == 0 )
			return registeredClassSet[i]->duplicate( false );
			
	return NULL;
}

void    poObject::pushObjectMatrix()
{
    // push matrix
    glPushMatrix();
	
	// push alpha stack
	if ( masterAlphaStackHeight < 256 )
	{
		masterAlphaStack[ masterAlphaStackHeight ] = poObject::masterAlphaControl;
		masterAlphaStackHeight++;
		poObject::masterAlphaControl *= alpha;
	}
	else
		printf("WARNING: poObject->pushObjectMatrix: over stacked\n");
    
	if(matrixOrder == PO_TRS_MATRIX)
	{
		glTranslatef(position.x, position.y, position.z);
		glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glScalef(scale.x, scale.y, scale.z);
	} else if(matrixOrder == PO_RTS_MATRIX)
	{
		glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glTranslatef(position.x, position.y, position.z);
		glScalef(scale.x, scale.y, scale.z);
	} else if(matrixOrder == PO_RST_MATRIX)
	{
		glRotatef(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
		glScalef(scale.x, scale.y, scale.z);
		glTranslatef(position.x, position.y, position.z);
	}
	
	if ( masterDisableMatrixCapture == false )
	{
		glGetIntegerv (GL_VIEWPORT, (GLint*) matrixSet.viewport );          // get current viewport transformation 
		glGetDoublev (GL_MODELVIEW_MATRIX, matrixSet.modelMatrix );         // get current model view transformation
		glGetDoublev (GL_PROJECTION_MATRIX, matrixSet.projectMatrix );      // get current projection transformation
	}
}

void    poObject::popObjectMatrix()
{
	// pop alpha stack
	if ( masterAlphaStackHeight >= 1 )
	{
		masterAlphaStackHeight--;
		poObject::masterAlphaControl = masterAlphaStack[ masterAlphaStackHeight ];
	}
	else
		printf("WARNING: poObject->pushObjectMatrix: pushObjectMatrix not popped\n");
	
    // pop matrix
    glPopMatrix();
}
