#include "poCamera.h"
#include "pathTestApp.h"
#include "poApplication.h"
#include "poSimpleDrawing.h"

#include "poPath.h"

enum PointType {NoneID, PointID, ControlID};

PointType pointUnderMouse(poPath &path, poPoint mouse, poPoint *response) {
	poPathSegmentIter seg = path.begin();
	while(seg != path.end()) {
		if(seg->type == PO_PATH_SEG_QUAD_TO)
			if((mouse - seg->control1).getLength() < 5.f) {
				response = &seg->control1;
				return ControlID;
			}
			else if(seg->type == PO_PATH_SEG_CUBE_TO) {
				if((mouse - seg->control1).getLength() < 5.f) {
					response = &seg->control1;
					return ControlID;
				}
				else if((mouse - seg->control2).getLength() < 5.f) {
					response = &seg->control2;
					return ControlID;
				}
			}
		++seg;
	}
	
	seg = path.begin();
	while(seg != path.end()) {
		if((mouse - seg->point).getLength() < 5.f) {
			response = &seg->point;
			return PointID;
		}
		++seg;
	}
	
	return NoneID;
}

poObject *createObjectForID(uint uid) {
	return new pathTestApp();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "pathTest", 100, 100, 400, 400);
}

void cleanupApplication() {
}

pathTestApp::pathTestApp() {
	addModifier(new poCamera2D());
}

void pathTestApp::draw() {
	std::vector<poPoint> points = path.generatePoints();
	
	poPathSegmentIter seg = path.begin();
	while(seg != path.end()) {
		if(seg->type == PO_PATH_SEG_QUAD_TO) {
			po::drawLine(seg->point, seg->control1);
		}
		else 
		if(seg->type == PO_PATH_SEG_CUBE_TO) {
			po::drawLine(seg->point, seg->control1);
			po::drawLine(seg->point, seg->control2);
		}
		++seg;
	}
}

void pathTestApp::eventHandler(poEvent *event) {
	if(event->type == PO_MOUSE_DOWN_EVENT) {
	}
	else 
	if(event->type == PO_MOUSE_UP_EVENT) {
		
	}
	else 
	if(event->type == PO_MOUSE_DRAG_EVENT) {
		
	}
}
