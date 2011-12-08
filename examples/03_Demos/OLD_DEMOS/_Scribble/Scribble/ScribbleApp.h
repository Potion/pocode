/*	Created by Jonathan Bobrow on 8/8/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "stdlib.h"
#include "math.h"
#include "poShapeBasics2D.h"
//#include "Coordinate.h"
//#include "Dot.h"

using namespace std;

class ScribbleApp : public poObject {
public:
	ScribbleApp();
	virtual ~ScribbleApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    void setup();
    void update();
    void draw();
    void createDotLocations();
    void createDots();
    void drawScribble();
    void reset();
    
    float distance(float dx, float dy);
    int windowWidth, windowHeight;
    bool doneScribbling;
	
    vector<poPoint*> vertices, locations;
    vector<poOvalShape*> dots;
	
    float fidelity, radius, diameter;
    
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
};