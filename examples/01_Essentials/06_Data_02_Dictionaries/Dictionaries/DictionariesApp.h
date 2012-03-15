/////////////////////////////////////////
//
// pocode : Dictionaries
//
/////////////////////////////////////////

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"

class DictionariesApp : public poObject {
public:
	DictionariesApp();
	virtual ~DictionariesApp();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
	
	poRectShape* bigRect;
	poTextBox* rectTextBox;
};