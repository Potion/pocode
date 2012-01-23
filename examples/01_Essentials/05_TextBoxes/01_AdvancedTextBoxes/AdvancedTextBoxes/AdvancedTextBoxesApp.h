/////////////////////////////////////////
//
// pocode : Advanced TextBoxes
//
/////////////////////////////////////////

#include "poObject.h"

class AdvancedTextBoxesApp : public poObject {
public:
	AdvancedTextBoxesApp();
	virtual ~AdvancedTextBoxesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
};