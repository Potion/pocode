#include "testApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

#include "poResourceLoader.h"
#include "SimpleDrawing.h"

poObject *createObjectForID(uint uid) {
	return new  testApp();
}

void setupApplication()
{
	lookUpAndSetPath("resources");
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "MainWindow", 100, 100, 1023, 768);
}

void cleanupApplication() 
{
}

 testApp:: testApp() 
{
    poPerspectiveCamera* cam = new poPerspectiveCamera( 60, 0.01, 400.0 );
    cam->cameraPosition( poPoint(0,0,-150) );
    addModifier( cam );
	
//    poRectShape* R = new poRectShape( 10,10 );
//    R->fillColor = poColor::blue;
//    R->alignment( PO_ALIGN_CENTER_CENTER );
//    addChild( R );
    
    myShape3D = new poSphere3D(30,60,40);
    myShape3D->useVertexNormals = true;
    
    //myShape3D = new poCube3D( 300,300,300 );
    addChild( myShape3D );
    
	addEvent(PO_KEY_DOWN_EVENT, this);
}

 testApp::~ testApp()
{
}

void  testApp::update()
{
    myShape3D->rotationAxis.set( 1,0,0 );
    myShape3D->rotation += 1.0;
}

void  testApp::eventHandler(poEvent *event) 
{
	if(event->type == PO_KEY_DOWN_EVENT) 
    {
		for(int i=0; i<100; i++) 
        {
    
		}
	}
}




