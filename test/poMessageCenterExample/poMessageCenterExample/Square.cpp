#include "Square.h"
#include "poMessageCenter.h"

Square::Square() {
    bToggle = false;
    
    shape = new poRectShape(100, 100);
    shape->fillColor.set(1.0f, 1.0f, 1.0f);
    addChild(shape);
    
    poMessageCenter::get()->addSubscriber(ToggleSquareMessage, this);
}

Square::~Square() {
}

void Square::update() {
}

void Square::eventHandler(poEvent *event) {
}

void Square::messageHandler(const std::string &msg, const poDictionary& dict)  {
    if(msg == ToggleSquareMessage) {
        bToggle = !bToggle;
        
        if(bToggle) {
            shape->fillColor.set(1.0f, 0.0f, 0.0f);
        } else {
            shape->fillColor.set(1.0f, 1.0f, 1.0f);
        }
    }
}
