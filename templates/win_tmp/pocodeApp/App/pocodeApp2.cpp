
#include "pocodeApp2.h"
#include "poShapeBasics2D.h"
#include "poApplication.h"
#include "poHelpers.h"
#include "poCamera.h"
#include "poResourceStore.h"

pocodeApp::pocodeApp()
{
	poCamera2D *cam = new poCamera2D( poColor::black );
    addModifier( cam );

	poShape2D* shape = new poRectShape( 100,100 );
	shape->fillColor = poColor::white;
	addChild( shape );

	printf("hello world!\n");
}

void pocodeApp::update()
{
}

void pocodeApp::draw()
{
}

void pocodeApp::eventHandler(poEvent*)
{
}

void	pocodeApp::messageHandler(const std::string &msg, const poDictionary &dict )
{
}