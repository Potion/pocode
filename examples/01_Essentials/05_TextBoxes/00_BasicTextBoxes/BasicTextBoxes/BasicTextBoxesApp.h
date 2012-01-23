/////////////////////////////////////////
//
// pocode : Basic TextBoxes
//
/////////////////////////////////////////

#include "poObject.h"

class BasicTextBoxesApp : public poObject {
public:
	BasicTextBoxesApp();
	virtual ~BasicTextBoxesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
}; 