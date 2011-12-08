#include "SlideShowApp.h"

#include "poCamera.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new SlideShowApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "SlideShow", 100, 100, 600, 300);
}

void cleanupApplication() {
}

SlideShowApp::SlideShowApp()
:	current(0)
{
	addModifier(new poCamera2D(poColor::black));
	
	holder = new poObject();
	addChild(holder);
	
	for(int i=0; i<10; i++) {
		poShape2D *img = new poRectShape(500,200);
		img->fillColor = hashPointerForColor(img);
		img->position.set(getWindowWidth()/2.f, getWindowHeight()/2.f, 0);
		img->setAlignment( PO_ALIGN_CENTER_CENTER );
		img->visible = (i==0?true:false);
		holder->addChild(img);
	}
	
	readout = new poTextBox(25,25);
	readout->setText( "1" );
	readout->textColor = poColor::black;
	readout->position.set( getWindowWidth()/2.f, getWindowHeight()/2.f, 0 );
//	readout->doLayout();
	addChild(readout);
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void SlideShowApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT && 
	   event->keyCode == PO_RIGHT_ARROW) 
	{
		holder->getChild(current)->visible = false;
		current = (current + 1) % holder->getNumChildren();
		holder->getChild(current)->visible = true;
		
//		readout->setText( lexical_cast<string>(current+1) );
//		readout->doLayout();
	}
}
