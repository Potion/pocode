#include "poUIButton.h"
#include "poMessageCenter.h"

namespace po {
    namespace UI {
        const std::string Button::DOWN_MESSAGE          = "UI_BUTTON_DOWN_MESSAGE";
        const std::string Button::PRESSED_MESSAGE       = "UI_BUTTON_PRESSED_MESSAGE";
        const std::string Button::TOGGLED_ON_MESSAGE    = "UI_BUTTON_TOGGLED_ON_MESSAGE";
        const std::string Button::TOGGLED_OFF_MESSAGE   = "UI_BUTTON_TOGGLED_OFF_MESSAGE";
        
        Button::Button()
        : bIsPressed(false)
        , bToggleEnabled(false)
        , bIsToggled(false)
        , bSetSizeFromLabel(false)
        , offImage(NULL)
        , onImage(NULL)
        , padding(5)
        {
            bg = new po::RectShape(50,50,5);
            addChild(bg);
            
            label = new po::TextBox(50,50);
            label->setTextAlignment(po::ALIGN_CENTER_CENTER);
            label->textColor.set(0, 0, 0);
            addChild(label);
            
            setPadding(padding);
            
            //Default Colors
            offColor.set255(255, 255, 255);
            onColor.set255(255, 0, 0);
            
			//bd: replaced mouse down with mouse down inside
            //addEvent(po::MOUSE_DOWN_EVENT, this);
			addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
            addEvent(po::MOUSE_UP_EVENT, this);
            
            setPressedState();
        }

        Button::~Button() {
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setWidth(float width) {
			setSize(width, getHeight());
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setHeight(float height) {
			setSize(getWidth(), height);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setSize(float width, float height) {
            bg->reshape(width, height, 5);
            label->reshape(width-padding*2, height-padding*2);
            
            setUseLabelSize(false);
            label->doLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPadding(float padding) {
            this->padding = padding;
            label->position.set(padding, padding, 0);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setCornerRadius(float radius) {
            bg->reshape(bg->getWidth(), bg->getHeight(), radius);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        void Button::setSizeFromLabel() {
            bg->reshape(label->getWidth() + padding * 2, label->getHeight() + padding*2);
            std::cout << padding << std::endl;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setUseLabelSize(bool useLabelSize) {
            this->bSetSizeFromLabel = useLabelSize;
            label->useTextBoundsAsBounds(useLabelSize);
            
            if(useLabelSize) {
                label->reshape(1000, 100);
                label->setTextAlignment(po::ALIGN_TOP_LEFT);
            } else {
                label->setTextAlignment(po::ALIGN_CENTER_CENTER);
            }
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelText(std::string text) {
            label->setText(text);
            doLabelLayout();
           
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelFont(po::Font *font) {
            label->setFont(font);
            doLabelLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelTextSize(float textSize) {
            label->setTextSize(textSize);
            doLabelLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelTextColor(po::Color color) {
            label->textColor.set(color);
            doLabelLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelTextAlignment(po::Alignment alignment) {
            label->setTextAlignment(alignment);
            doLabelLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        void Button::doLabelLayout() {
            label->doLayout();
            
            if(this->bSetSizeFromLabel) setSizeFromLabel();
        }
        
        //------------------------------------------------------------------
        Button& Button::setOffColor(po::Color color) {
            offColor.set(color);
            if(!isPressed()) bg->fillColor.set(offColor);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setOnColor(po::Color color) {
            onColor.set(color);
            if(isPressed()) bg->fillColor.set(onColor);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setOffImage(po::Object *offImage, bool andUseSize) {
            removeOffImage();
            
            this->offImage = offImage;
            addChild(offImage);
            moveChildToBack(offImage);
            
            setPressedState();
            
            if(andUseSize) {
                this->setSize(offImage->getWidth(), offImage->getHeight());
            }
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        void Button::removeOffImage() {
            if(offImage != NULL) {
                removeChild(offImage);
                offImage = NULL;
            }
            
            setPressedState();
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setOnImage(po::Object *onImage, bool andUseSize) {
            removeOnImage();
            
            this->onImage = onImage;
            addChild(onImage);
            moveChildToBack(onImage);
            
            setPressedState();
            
            if(andUseSize) {
                this->setSize(onImage->getWidth(), onImage->getHeight());
            }
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        void Button::removeOnImage() {
            if(onImage != NULL) {
                removeChild(onImage);
                onImage = NULL;
            }
            
            setPressedState();
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setToggleMode(bool shouldToggle) {
            bToggleEnabled = shouldToggle;
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::toggle() {
            bIsToggled ? toggleOff() :  toggleOn();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::toggleOn() {
            bIsToggled = true;
            setPressedState();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::toggleOff() {
            bIsToggled = false;
            setPressedState();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        bool Button::isToggled() {
            return bIsToggled;
        }
        
        
        //------------------------------------------------------------------
        bool Button::isPressed() {
            return bIsPressed;
        }
        
        
        //------------------------------------------------------------------
        void Button::setPressedState() {
            if(isPressed() || isToggled()) {
                if(offImage) removeChild(offImage);
                if(onImage) {
                    addChild(onImage);
                    moveChildToBack(onImage);
                    bg->visible = false;
                } else {
                    bg->fillColor.set(onColor);
                }
            } else {
                if(onImage) removeChild(onImage);
                if(offImage) {
                    addChild(offImage);
                    moveChildToBack(offImage);
                    bg->visible = false;
                } else {
                    bg->visible = true;
                    bg->fillColor.set(offColor);
                }
            }
        }
        
        
        //------------------------------------------------------------------
        void Button::sendDownMessage() {
            po::MessageCenter::broadcastMessage(DOWN_MESSAGE, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::sendPressedMessage() {
            po::MessageCenter::broadcastMessage(PRESSED_MESSAGE, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::sendToggledMessage() {
            std::string msg = isToggled() ? TOGGLED_ON_MESSAGE : TOGGLED_OFF_MESSAGE;
            po::MessageCenter::broadcastMessage(msg, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::eventHandler(po::Event *event) {
            switch (event->type) {
				//bd: replaced mouse down with mouse down inside
                //case po::MOUSE_DOWN_EVENT:
				case po::MOUSE_DOWN_INSIDE_EVENT:
					bIsPressed = true;
					setPressedState();
					sendDownMessage();
                    break;
                    
                case po::MOUSE_UP_EVENT:
                    //Send pressed message
                    if(bIsPressed) {
                        sendPressedMessage();
                        bIsPressed = false;
                        
                        if(bToggleEnabled) {
                            toggle();
                            sendToggledMessage();
                        }
                        
                        setPressedState();
                    }
                    
                    break;
            }
        }
        
        
        //------------------------------------------------------------------
        void Button::messageHandler(const std::string &msg, const po::Dictionary& dict) {
        }
        
        
        
        
		//------------------------------------------------------------------
		//	Radio Buttons
		//------------------------------------------------------------------
        #pragma mark - RadioButton -
        
        const std::string RadioButton::SELECTED_MESSAGE                 = "UI_RADIO_BUTTON_SELECTED_MESSAGE";
        const std::string RadioButton::SELECTED_PRESSED_MESSAGE         = "UI_RADIO_BUTTON_SELECTED_PRESSED_MESSAGE";
		
        RadioButton::RadioButton() :
		Button() {
			setToggleMode(true);
			bSelected = false;
		}
		
		//------------------------------------------------------------------
		RadioButton::~RadioButton(){
		}
		
        
		//------------------------------------------------------------------
		RadioButton& RadioButton::setOffColor(po::Color color) {
			offColor.set(color);
			if(!bSelected) bg->fillColor.set(offColor);
            
            return *this;
		}
		
        
		//------------------------------------------------------------------
		RadioButton& RadioButton::setOnColor(po::Color color) {
			onColor.set(color);
			if(bSelected) bg->fillColor.set(onColor);
            
            return *this;
		}
		
		
        //------------------------------------------------------------------
        void RadioButton::sendSelectedMessage() {
            po::MessageCenter::broadcastMessage(SELECTED_MESSAGE, this);
        }
		
		
        //------------------------------------------------------------------
        void RadioButton::sendSelectedPressedMessage() {
            po::MessageCenter::broadcastMessage(SELECTED_PRESSED_MESSAGE, this);
        }
        
		
		//------------------------------------------------------------------
        void RadioButton::eventHandler(po::Event *event) {
            if( event->type ==  po::MOUSE_DOWN_INSIDE_EVENT ) {
				if(!bSelected) {
					bSelected = true;
					sendSelectedMessage();
				} else {
                    sendSelectedPressedMessage();
                }
            }
        }
		
        
        
        
        // -----------------------------------------------------------------------------------
        // ========================== Class: Radio Button Set ================================
		#pragma mark - RadioButtonSet -
		
		//------------------------------------------------------------------
		RadioButtonSet::RadioButtonSet(){
			// init
			bVertical	= false;
			padding		= 5;
			
			po::MessageCenter::addSubscriber(RadioButton::SELECTED_MESSAGE, this);
		}
		
		//------------------------------------------------------------------
		RadioButtonSet::~RadioButtonSet(){}

		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::addButtons(std::vector<RadioButton *> btns) {
			for(int i=0; i<btns.size(); i++){
				addButton(btns[i]);
			}
            
            doLayout();
            
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::addButton(RadioButton *button) {
			radioButtons.push_back(button);
			addChild(radioButtons.back());
            
            doLayout();
            
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::removeButton(RadioButton *button) {
			for(int i=0; i<radioButtons.size(); i++){
				if( radioButtons[i] == button ){
					removeChild(radioButtons[i]);
					break;
				}
			}
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setButtonOnColors(po::Color c) {
			for(int i=0; i<radioButtons.size(); i++){
				radioButtons[i]->setOnColor(c);
			}
            
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setButtonOffColors(po::Color c) {
			for(int i=0; i<radioButtons.size(); i++){
				radioButtons[i]->setOffColor(c);
			}
            
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setButtonWidths(float width) {
			for(int i=0; i<radioButtons.size(); i++){
				radioButtons[i]->setWidth(width);
			}
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setButtonHeights(float height) {
			for(int i=0; i<radioButtons.size(); i++){
				radioButtons[i]->setHeight(height);
			}
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setButtonSizes(float width, float height) {
			for(int i=0; i<radioButtons.size(); i++){
				radioButtons[i]->setSize(width, height);
			}
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setVertical(bool b) {
			bVertical = b;
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::setPadding(int p) {
			padding = p;
            
            doLayout();
            return *this;
		}
		
		//------------------------------------------------------------------
		RadioButtonSet& RadioButtonSet::doLayout(bool firstSelected) {
			for(int i=0; i<radioButtons.size(); i++){
				if( !bVertical )
					radioButtons[i]->position.set(padding + i * (radioButtons[0]->getWidth() + padding), padding, 0);
				else
					radioButtons[i]->position.set(padding, padding + i * (radioButtons[0]->getHeight() + padding), 0);
			}
			
			if( firstSelected ){
				// start with first one selected
				radioButtons[0]->toggleOn();
				selected = radioButtons[0];
			}
            
            return *this;
		}
		
		//------------------------------------------------------------------
		bool RadioButtonSet::isFromThisSet(RadioButton *button) {
			for (int i=0; i<radioButtons.size(); i++){
				if( button == radioButtons[i])
					return true;
			}
			
			return false;
		}
		
		//------------------------------------------------------------------
		void RadioButtonSet::eventHandler(po::Event *event) {}
		
		//------------------------------------------------------------------
		void RadioButtonSet::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender) {
			
			RadioButton* btn = (RadioButton *) sender;
			
			if(msg == RadioButton::SELECTED_MESSAGE && isFromThisSet(btn)){
				
				for(int i=0; i<radioButtons.size(); i++){
					if(btn == radioButtons[i]){
						selected = radioButtons[i];
                        radioButtons[i]->toggleOn();
					}else{
						radioButtons[i]->toggleOff();
						radioButtons[i]->bSelected = false;
					}
				}
			}

		}

		
    }/* End UI namespace */
}/* End po Namespace */
