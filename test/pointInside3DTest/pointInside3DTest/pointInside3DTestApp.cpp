#include "pointInside3DTestApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new pointInside3DTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pointInside3DTest", 100, 100, 1024, 768);
}

void cleanupApplication() { 
}

pointInside3DTestApp::pointInside3DTestApp()
{
    poPerspectiveCamera* cam = new poPerspectiveCamera( 60, 0.01, 10000.0 );
    cam->setBackgroundColor( poColor::black );
    cam->setupFor2DOnZPlane();  
    addModifier( cam ); 
    
    myShape = new poStarShape(100,5,150); // 50,50 );
    myShape->fillColor = poColor::white;   
    myShape->rotationAxis.set( 0,1,0 ); 
    myShape->rotation = 45;   
    myShape->position.set( 40,30,20 );  
    myShape->setAlignment( PO_ALIGN_CENTER_CENTER ); 
    myShape->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "star_click" ); 
    addChild( myShape );  

    poRectShape* R = new poRectShape( 100,100 );
    R->position.set( -100,-100, 0 );
    R->setAlignment( PO_ALIGN_CENTER_CENTER );
    R->fillColor = poColor::white; 
    addChild(R);  
    
    imageShape = new poImageShape( "apple.jpg" );  
    imageShape->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "image_click" ); 
    imageShape->rotationAxis.set( 1,0,0 );
    imageShape->rotation = 40; 
    imageShape->position.set( 100,100,50 );
    addChild( imageShape );
    
    shape3D = new poSphere3D( 15,30,100 ); //poCube3D( 100,100,100 );
    shape3D->addEvent( PO_MOUSE_DOWN_INSIDE_EVENT, this, "shape3D_click" ); 
    shape3D->position.set( -200,200,0 );
    shape3D->fillColor = poColor::orange;
    shape3D->strokeWidth = 2;
    shape3D->pointSize = 3; 
    addChild( shape3D );
} 
 
pointInside3DTestApp::~pointInside3DTestApp() 
{ 
} 

void pointInside3DTestApp::update()
{
    myShape->rotationAxis.set( 1,1,0 ); 
    myShape->rotation += 1.0;
    
    shape3D->rotationAxis.set( 0,1,1 );
    shape3D->rotation += 1.0;   
}

void pointInside3DTestApp::eventHandler(poEvent *event) 
{
    if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT && event->message == "star_click" ) 
    {
        myShape->fillColor = poColor::random();
    }
    
    if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT && event->message == "image_click" )
    { 
        imageShape->rotation += 10; 
    }
    
    if ( event->type == PO_MOUSE_DOWN_INSIDE_EVENT && event->message == "shape3D_click" )
    { 
        shape3D->fillColor = poColor::random();
    }
    
	if(event->type == PO_KEY_DOWN_EVENT) 
    {
	}	
}

void pointInside3DTestApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
