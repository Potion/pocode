
#include "poApp.h"

#include "poShapePrimitives.h"
#include "poShape3DPrimitives.h"
#include "poTextEntryBox.h"
#include "poScene.h"
#include "poCamera.h"


#ifndef WIN32
#include "poPDFShape.h"
#endif

int poApp::appW = 1024;
int poApp::appH = 768;
poApp* poApp::theApp = NULL;


poApp::poApp(int w, int h)
{
	appW = w;
	appH = h;
	theApp = this;
	
	clearColor.set( 0,0,0,1 );
	initGL();
	
	this->addModifier( new poCamera2D() );

	rootNode = new poObject();
	rootNode->setName( "rootNode" );
	poObject::addChild( rootNode );
		
	serialize = true;		// so init is called
	
	
}

void poApp::addChild(poObject* child)
{
	// enforces that all app object are added to the rootNode
	rootNode->addChild( child );
}

void poApp::drawApp()
{
	cleanCanvas();
	
	rootNode->position.set( 0,0,0 );

	updateTree();
	drawTree();
}

void poApp::initGL()
{
#ifndef WIN32
    glEnable (GL_MULTISAMPLE_ARB);
    glHint (GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable( GL_COLOR_MATERIAL ); 
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);
}

void poApp::setupLighting()
{
	GLfloat ambient[] = {0.00f, 0.00f, 0.00f, 1.0f};
	GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat specular[] = {0.00f, 0.00f, 0.00f, 1.0f};
	GLfloat lightpos[] = {0.0f, 100.0f, -100.0f, 1.0f};

	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	
	int lightModel = 1;
	glLightModeliv( GL_LIGHT_MODEL_TWO_SIDE, (GLint*) &lightModel );
}

void poApp::cleanCanvas()
{
	glClearColor( clearColor.R, clearColor.G, clearColor.B, clearColor.A );
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void poApp::cleanCamera()
{
    // set viewing
    glViewport( 0, 0, appW, appH );  
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
	gluOrtho2D( 0, appW, 0, appH );
    //gluPerspective( 60.0, appW/(float)appH, 0.1, 10000.0);
    //glTranslatef( 0, 0, -dtan(60)*appW/2 );

    // set modeling
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void poApp::mouseDown(float x, float y, int modifiers)
{
	poTextEntryBox::activeTextEntryBox = NULL;
	
	poEvent e;
	e.type = PO_MOUSE_DOWN_EVENT;
	e.x = x;
	e.y = y;
	e.modifiers = modifiers;
	processEventTree(&e);
	
	e.type = PO_MOUSE_PRESS_EVENT;
	e.checkBounds = true;
	processEventTree(&e);
}

void poApp::mouseUp(float x, float y, int modifiers)
{
	poEvent e;
	e.type = PO_MOUSE_UP_EVENT;
	e.x = x;
	e.y = y;
	e.modifiers = modifiers;
	processEventTree(&e);
		
	e.type = PO_MOUSE_RELEASE_EVENT;
	e.checkBounds = true;
	processEventTree(&e);
	
	e.type = PO_MOUSE_RELEASE_OUTSIDE_EVENT;
	e.checkBounds = true;
	processEventTree(&e);
}

void poApp::mouseMove(float x, float y, int modifiers)
{
	poEvent e;
	e.type = PO_MOUSE_MOVE_EVENT;
	e.x = x;
	e.y = y;
	e.modifiers = modifiers;
	processEventTree(&e);
	
	e.type = PO_MOUSE_OVER_EVENT;
	e.checkBounds = true;
	processEventTree(&e);

	e.type = PO_MOUSE_LEAVE_EVENT;
	e.checkBounds = true;
	processEventTree(&e);

	e.type = PO_MOUSE_ENTER_EVENT;
	e.checkBounds = true;
	processEventTree(&e);
}

void poApp::mouseDrag(float x, float y, int modifiers)
{
	poEvent e;
	e.type = PO_MOUSE_DRAG_EVENT;
	e.x = x;
	e.y = y;
	e.modifiers = modifiers;
	processEventTree(&e);
}

void poApp::keyDown(char key, float x, float y, int modifiers)
{	
	poEvent e;
	e.type = PO_KEY_DOWN_EVENT;
	e.key = key;
	e.x = x;
	e.y = y;
	e.modifiers = modifiers;
	e.message = NULL;
		
	if ( poTextEntryBox::activeTextEntryBox != NULL )
		poTextEntryBox::activeTextEntryBox->eventHandler(&e);
	else
		processEventTree(&e);
}

void poApp::specialKeyDown(int key, unsigned int modifiers)
{
	poEvent e;
	e.type = PO_KEY_DOWN_EVENT;
	e.key = key;
	e.modifiers = modifiers;
	e.message = NULL;
	
	if ( poTextEntryBox::activeTextEntryBox != NULL )
		poTextEntryBox::activeTextEntryBox->eventHandler(&e);
	else
		processEventTree(&e);
}

void poApp::resize(int w, int h)
{
	appW = w;
	appH = h;
}


void poApp::readApp( const char* fileName )
{
	poXMLnode* xmlRoot = NULL;
	poXMLparser parser;
	
	xmlRoot = parser.parseFile(fileName);

	if ( xmlRoot != NULL ) // assert(xmlRoot != NULL);
		rootNode->read(xmlRoot);
	else
		printf("ERROR: poApp::readApp cold not find poAPPLICATION.xml\n");
	
	delete xmlRoot;
	
}

void poApp::writeApp( const char* fileName )
{
	poXMLnode* xmlRoot = NULL;
	xmlRoot = rootNode->write();

	FILE* xmlFile = fopen( fileName, "w");
	xmlRoot->unparse(0, xmlFile);
	fclose(xmlFile);
}

void poApp::registerClasses()
{
	poObject::registeredClassSet.push_back( new poObject() );
	poObject::registeredClassSet.push_back( new poScene() );
	poObject::registeredClassSet.push_back( new poShape2D() );
	poObject::registeredClassSet.push_back( new poRectShape() );
	poObject::registeredClassSet.push_back( new poOvalShape() );
	poObject::registeredClassSet.push_back( new poTextBox() );
	poObject::registeredClassSet.push_back( new poPlane3D() );
	poObject::registeredClassSet.push_back( new poCube3D() );
	poObject::registeredClassSet.push_back( new poSphere3D() );
	poObject::registeredClassSet.push_back( new poAppFrame() );
#ifndef WIN32
	poObject::registeredClassSet.push_back( new poPDFShape() );
#endif

	poObjectModifier::registeredModifierSet.push_back( new poTexture() );
	poObjectModifier::registeredModifierSet.push_back( new poCamera3D() );
	poObjectModifier::registeredModifierSet.push_back( new poCamera2D() );
	poObjectModifier::registeredModifierSet.push_back( new poSceneShot() );
}

void poApp::launchApp()
{
	registerClasses();
	initTree();
}

void poApp::capture( int x, int y, int w, int h )
{
	/*glReadBuffer( GL_FRONT );
	glReadPixels( x, y, w, h, GL_RGB, GL_UNSIGNED_BYTE, image );
	unsigned char* image = (unsigned char*)malloc( w*h*3 );
	long *bits = (long*)FreeImage_GetBits(memJpegInBuff);
	memcpy( bits, (unsigned char*)image, w*h*3 );
	
	FreeImage_Save( FIF_JPEG, memJpegInBuff, filename, 90 );*/
	
}

void poApp::messageHandler(const char* message, const poDictionary& data )
{
	if ( strcmp(message,"save_app")==0 )
	{
		printf("saving app\n");
		writeApp( "poAPPLICATION.xml" );
	}
}


// ====================== poAppFrame ====================== //

poAppFrame::poAppFrame()
{
	construct( 1024, 768 );
}

poAppFrame::poAppFrame( int W, int H )
{
	construct( W, H );
}

void	poAppFrame::construct( int W, int H )
{
	setName( getClassName() );
	
	frameWidth = W;
	frameHeight = H;
	
	registry.addSeparator( getClassName() );
	registry.add( "frameWidth", &frameWidth );
	registry.add( "frameWidth", &frameHeight );
}


poObject*	poAppFrame::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poAppFrame* obj = (poAppFrame*) copyIntoObject;
	if ( obj==NULL )
		obj = new poAppFrame();
	
	obj->frameWidth = frameWidth;
	obj->frameHeight = frameHeight;
	
	poObject::duplicate( copyChildren, obj );
	return obj;
}

void poAppFrame::debugDraw()
{
	// enforce zero position
	position.set(0,0,0);
	scale.set(1,1,1);

	glColor3f( 1,1,1 );
	glLineWidth(3);
	
	glBegin( GL_LINE_LOOP );
	glVertex2f( 0,0 );
	glVertex2f( frameWidth,0 );
	glVertex2f( frameWidth,frameHeight );
	glVertex2f( 0,frameHeight );
	glEnd();
	
	glLineWidth(1);
}



