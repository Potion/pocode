/////////////////////////////////////////
//
// pocode : Update function
//
/////////////////////////////////////////

#include "poObject.h"
#include "poTextBox.h"								// include poTextBox.h to be able to use poTextBox

class UpdateApp : public poObject {
public:
	UpdateApp();
	virtual ~UpdateApp();
	
	virtual void update();
	
	virtual void draw();
	
	virtual void eventHandler(poEvent *event);
	
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poShape2D* A;
	bool isScalingUp;
	
	poShape2D* B;
	poPoint velocity;
	
	poShape2D* C;
	
	poTextBox* D;
};