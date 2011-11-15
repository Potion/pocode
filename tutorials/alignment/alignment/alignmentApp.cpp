#include "alignmentApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poResourceStore.h"


poObject *createObjectForID(uint uid) {
	return new alignmentApp();
}

void setupApplication() {
    
    // define path to the resource folder
    lookUpAndSetPath("resources");
    
    // define window settings
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "alignment", 100, 100, 800, 600);
}

void cleanupApplication() {
}

alignmentApp::alignmentApp() {
    
    // define the 2D camera and set it's color
	addModifier(new poCamera2D(poColor::black));
    
    // add a poRectshape with a background image
    poRectShape* BG = new poRectShape("bg.jpg");
    addChild( BG );
    
    poRectShape* p;
    poOvalShape* o;
    poOvalShape* d;

    
    // iterate through a 3X3 grid to create 9 rechtangles and 9 ovals
    for(int i=0; i<3; i++){
        
        // set the offset for the entire grid
        int offsetX = 200;
        int offsetY = 150;
        
        for(int j=0; j<3; j++){
            
            p = new poRectShape(getWindowWidth()/8, getWindowHeight()/8);
            p->position.set(offsetX+j*(getWindowWidth()/4),offsetY+i*(getWindowHeight()/4),1.f);
            p->generateStroke(1);
			p->strokeColor = poColor::black;
			p->fillColor = poColor::ltGrey;
			
            // switch between each allignment mode
            
            switch (i*3+j) {
                case 0: p->setAlignment(PO_ALIGN_TOP_LEFT);  break;
                case 1: p->setAlignment(PO_ALIGN_TOP_CENTER);  break;
                case 2: p->setAlignment(PO_ALIGN_TOP_RIGHT);  break;
                case 3: p->setAlignment(PO_ALIGN_CENTER_LEFT);  break;
                case 4: p->setAlignment(PO_ALIGN_CENTER_CENTER);  break;
                case 5: p->setAlignment(PO_ALIGN_CENTER_RIGHT);  break;
                case 6: p->setAlignment(PO_ALIGN_BOTTOM_LEFT);  break;
                case 7: p->setAlignment(PO_ALIGN_BOTTOM_CENTER);  break;
                case 8: p->setAlignment(PO_ALIGN_BOTTOM_RIGHT);  break;
                default: break;
            }
			
            addChild(p);
            
            //creating an oval to be a reference for the object alignment
            //notice that the position of the oval is set relative to it's respective rectShape2D parent
            
            int ovelWidth = 30;
            o = new poOvalShape( ovelWidth, ovelWidth, 20);
            //o->position.set( ovelWidth/2 ,ovelWidth/2,0 );
			//o->setAlignment(PO_ALIGN_CENTER_CENTER);
            o->fillColor = poColor::red;
            
            d = new poOvalShape( ovelWidth/5, ovelWidth/5, 20);
            d->fillColor = poColor::black;
            //d->setAlignment(PO_ALIGN_CENTER_LEFT);
            // add the oval it it's rectangle partent
            p->addChild(o);
            //o->addChild(d);
        }
    }
}

alignmentApp::~alignmentApp() {
}

void alignmentApp::eventHandler(poEvent *event) {
	
}

void alignmentApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
