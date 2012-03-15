/////////////////////////////////////////
//
// pocode : Control Panel
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"
#include "poControlPanel.h"

class ControlPanelApp : public poObject {
public:
	ControlPanelApp();
	virtual ~ControlPanelApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	void makeShape(int optionID);
	
	std::vector<int> shapeOptions;
	
	poTextBox* text;
	poOvalShape* shape;
	
	poControlPanel* control;
};

