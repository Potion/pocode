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
    cam->cameraPosition( poPoint(0,0,-250) );
    addModifier( cam );

    poRectShape* R = new poRectShape("apple.jpg"); // 50,50 );
    R->fillColor = poColor::white;
    R->alignment( PO_ALIGN_CENTER_CENTER );
    //addChild( R );
        
    //myShape3D = new poSphere3D(20,40,40);
    //myShape3D->useVertexNormals = true;
    //myShape3D = new poCube3D( 300,300,300 );
    //addChild( myShape3D );

    myMesh3D = new poMesh3D( 10,10 );
    myMesh3D->placeTexture( poTexture("apple.jpg") );
    myMesh3D->useVertexTextureCoords = true;
    addChild( myMesh3D );

    
    for( int i=0; i<myMesh3D->numRows; i++ )
    {
        for( int j=0; j<myMesh3D->numColumns; j++ )
        { 
            myMesh3D->getVertex( i,j ).position.y += sin_deg(i*36)*20.0;
        }
    }
    
    
	addEvent(PO_KEY_DOWN_EVENT, this);
}

 testApp::~ testApp()
{
}

void  testApp::update()
{
    //myShape3D->rotationAxis.set( 1,0,0 );
    //myShape3D->rotation += 1.0;
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




