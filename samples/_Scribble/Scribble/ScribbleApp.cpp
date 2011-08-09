
#include "ScribbleApp.h"
#include "poApplication.h"
#include "poCamera.h"

poObject *createObjectForID(uint uid) {
	return new ScribbleApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Scribble", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

ScribbleApp::ScribbleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    fidelity = 0.1;
	radius = 5.0;
	diameter = 2*radius;
	doneScribbling = false;
}

ScribbleApp::~ScribbleApp() {
}

void ScribbleApp::eventHandler(poEvent *event) {
	
}

void ScribbleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}


//--------------------------------------------------------------
void ScribbleApp::update(){
	if(doneScribbling){
		for(int i=0; i<dots.size(); i++){
			dots[i]->update(locations[i]->x, locations[i]->y);
		}
	}
}

//--------------------------------------------------------------
void ScribbleApp::draw(){
	
//	ofBackground(204, 204, 204);
	
	drawScribble();    // draw the scribble to screen
	
	if(doneScribbling){
		for(int i=0; i<dots.size(); i++){
			dots[i]->display();
		}   
	}
	
    
}

//-------------------- record the points if mouse being dragged ------------------------
void ScribbleApp::mouseDragged(int x, int y, int button){
	
	if(doneScribbling){
		reset();
		doneScribbling = false;
	}
	vertices.push_back(new poPoint(x, y));
    
}

//-------------------- record the points if mouse is clicked -----------------------
void ScribbleApp::mousePressed(int x, int y, int button){
	
	if(doneScribbling){
		reset();
		doneScribbling = false;
	}
	vertices.push_back(new poPoint(x, y));
}

//-------------------- populate the line with circles when done drawing --------------------
void ScribbleApp::mouseReleased(int x, int y, int button){
	createDotLocations();
	createDots();
	doneScribbling = true;
}

//-------------------- if the window is maximized, adjust height and width for dot start locations ------------------
void ScribbleApp::windowResized(int x, int y){
	windowWidth = x;
	windowHeight = y;
}

//-------------- Resets the vectors so they can make a new drawing --------------
void ScribbleApp::reset(){
	for(int i=0; i<vertices.size(); i++){
		delete vertices[i];
	}
	for(int i=0; i<locations.size(); i++){
		delete locations[i];
	}
	for(int i=0; i<dots.size(); i++){ 
		delete dots[i];
	}
	
	vertices.clear();
	locations.clear();
	dots.clear();
}

//--------------- Draw the scribble to the screen -----------------
void ScribbleApp::drawScribble(){
	
	if(vertices.size() > 1){
		for(int i=0; i<vertices.size()-1; i++){
			
			poPoint* start = vertices[i];
			poPoint* end = vertices[i+1];
			
//			ofSetColor(0, 0, 0);
//			ofSetLineWidth(3.0);
			// draw a line from prev vertex to next vertex
			poLineShape( start, end );
		}
	}
}

//------------- determines locations on the line that allow circles to be placed edge to edge ------------
void ScribbleApp::createDotLocations(){
	
	// add the beginning of the line as our first location
	poPoint* startVert = vertices[0];
	locations.push_back(new poPoint(startVert->x, startVert->y));
	
	float prevLocX = startVert->x;
	float prevLocY = startVert->y;
	float currentLocX = startVert->x;
	float currentLocY = startVert->y;
	
	
	for(int i=0; i<vertices.size()-1; i++){
		
		// recall a start and end of a line segment
		Coordinate* start= vertices[i];
		Coordinate* end = vertices[i+1];
		
		// measure the change in position on the current line segment
		float dx = end->x - start->x;
		float dy = end->y - start->y;
		
		// measure the change in position from the last added location
		float loc_dx = end->x - prevLocX;
		float loc_dy = end->y - prevLocY;
		
		// start searching from the closest point on this segment and move towards the end
		currentLocX = start->x;
		currentLocY = start->y;
		
		while(distance(loc_dx,loc_dy) >= diameter){
			// while the current segment of line contains room for a circle,
			// find the point to draw the circle and add it as a location
			
			loc_dx = prevLocX - currentLocX;  // change in x from last added location
			loc_dy = prevLocY - currentLocY;  // change in y from last added location
			
			
            while(distance(loc_dx,loc_dy) < diameter){
				// since we know this line segment contains room for another circle
				// lets fit as many in this line segment as we can
				
				if(dx!=0){ // first deal with a defined slope
					
					if(dx>0){  // move along the line in the positive x direction
						currentLocX += fidelity;
						currentLocY += dy/dx*fidelity;
					}
					else{  // move along the line in the negative x direction
						currentLocX -= fidelity;
						currentLocY -= dy/dx*fidelity;
					}
					
					// update the change in location to see if it is far enough away 
					// to add another location
					loc_dx = prevLocX - currentLocX;
					loc_dy = prevLocY - currentLocY;
				}
				else{     // now we deal with the undefined slope
					
					if(dy>0)  // move along the line in the positive y direction
						currentLocY += fidelity;
					
					else if(dy<0)  // move along the line in the negative y direction
						currentLocY -= fidelity;
					
					// update the change in location to see if it is far enough away 
					// to add another location
					loc_dy = prevLocY - currentLocY;           
				}
            }  // end while -- found a location to fit a circle in this segment
			
            locations.push_back(new Coordinate(currentLocX, currentLocY));  // add the new location because we are far enough away
            
            // now update the previous location to be the one we just added
            prevLocX = currentLocX;
            prevLocY = currentLocY;
            
            // and update the change in location from the end of the segment to the new location
            loc_dx = end->x - prevLocX;
            loc_dy = end->y - prevLocY;
            
		}  // end while -- no more room for circles in this segment
		
	}  // end for loop -- no more segments
	
	//TODO: insert catch for the end of the segment
	// add the last location if there is a segment showing
	// this is due to the segment being further than the radius
	// but less than a diameter away
	//locations.add(new Coordinate(currentLocX, currentLocY));
	
	
}


// -------- simple distance function using the change in x and change in y instead of coordinates --------
float ScribbleApp::distance(float dx, float dy){
	return( sqrt(dx*dx+dy*dy) );
}

// -------- create instances of the dot and initialize them around the canvas --------
void ScribbleApp::createDots(){
	
	float xloc, yloc;
	
	for(int i=0; i<locations.size(); i++){
		
		// place the dots around the boarder of the canvas to start
		switch(rand()%4){
			case 0: xloc = windowWidth;
				yloc = rand()%windowHeight;
				break;     
			case 1: xloc = rand()%windowWidth;
				yloc = 0;
				break;
			case 2: xloc = rand()%windowWidth;
				yloc = windowHeight;
				break;
			case 3: xloc = 0;
				yloc = rand()%windowHeight;
				break;
			default: xloc = 0;
                yloc = 0;      
		}
		
		// for each location, add a new dot
		dots.push_back(new Dot(i, radius, xloc, yloc));  
	}
}