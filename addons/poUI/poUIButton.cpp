#include "poUIButton.h"
#include "poMessageCenter.h"

namespace po {
    namespace UI {
        const std::string Button::PRESSED_MESSAGE           = "UI_BUTTON_PRESSED_MESSAGE";
        const std::string Button::RELEASED_MESSAGE          = "UI_BUTTON_RELEASED_MESSAGE";
        const std::string Button::TOGGLED_ON_MESSAGE        = "UI_BUTTON_TOGGLED_ON_MESSAGE";
        const std::string Button::TOGGLED_OFF_MESSAGE       = "UI_BUTTON_TOGGLED_OFF_MESSAGE";
        
        Button::Button()
        : bIsPressed(false)
        , bToggleEnabled(false)
        , bIsToggled(false)
        , bSetSizeFromLabel(false)
        , offImage(NULL)
        , onImage(NULL)
        {
            bg = new po::RectShape(50,50,5);
            addChild(bg);
            
            label = new po::TextBox(50,50);
            label->setTextAlignment(po::ALIGN_CENTER_CENTER);
            label->textColor.set(0, 0, 0);
            addChild(label);
            
            setPadding(5); //Default padding
            
            //Default Colors
            offColor.set255(255, 255, 255);
            onColor.set255(255, 0, 0);
            
            
			addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
            addEvent(po::MOUSE_UP_EVENT, this);
            
            addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
            addEvent(po::TOUCH_ENDED_EVENT, this);
            
            setPressedState();
        }

        Button::~Button() {
        }
        
        
        
        
        //------------------------------------------------------------------
		//	Format
		//------------------------------------------------------------------
        #pragma mark Format
        
        
        //------------------------------------------------------------------
        Button& Button::setWidth(float width) {
			setSize(width, getHeight()-getPaddingVertical());
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setHeight(float height) {
			setSize(getWidth()-getPaddingHorizontal(), height);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setSize(float width, float height) {
            bg->reshape(width + getPaddingHorizontal(), height + getPaddingVertical(), 0);
            
            label->reshape(width, height);
            label->position.set(paddingLeft, paddingTop, 0);
            
            if(offImage)    offImage->position.set(paddingLeft, paddingTop, 0);
            if(onImage)     onImage->position.set(paddingLeft,  paddingTop, 0);
            
            setUseLabelSize(false);
            label->doLayout();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPadding(float padding) {
            return setPadding(padding, padding, padding, padding);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPadding(float paddingHorizontal, float paddingVertical) {
            return setPadding(paddingVertical, paddingHorizontal, paddingVertical, paddingHorizontal);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPadding(float top, float right, float bottom, float left) {
            //Find the current padding, record
            float curPaddingHorizontal  = getPaddingHorizontal();
            float curPaddingVertical    = getPaddingVertical();
            
            paddingTop      = top;
            paddingRight    = right;
            paddingBottom   = bottom;
            paddingLeft     = left;
            
            //Set size, taking out previous padding
            this->setSize(getWidth()-curPaddingHorizontal, getHeight()-curPaddingVertical);
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPaddingTop(float padding) {
            return setPadding(padding, paddingRight, paddingBottom, paddingLeft);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPaddingRight(float padding) {
            return setPadding(paddingTop, padding, paddingBottom, paddingLeft);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPaddingBottom(float padding) {
            return setPadding(paddingTop, paddingRight, padding, paddingLeft);
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setPaddingLeft(float padding) {
            return setPadding(paddingTop, paddingRight, paddingBottom, padding);
        }
        
        //------------------------------------------------------------------
        float Button::getPaddingHorizontal() {
            return this->paddingLeft + this->paddingRight;
        }
        
        
        //------------------------------------------------------------------
        float Button::getPaddingVertical() {
            return this->paddingTop + this->paddingBottom;
        }
        
        //------------------------------------------------------------------
        Button& Button::setCornerRadius(float radius) {
            bg->reshape(bg->getWidth(), bg->getHeight(), radius);
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setUseLabelSize(bool useLabelSize) {
            this->bSetSizeFromLabel = useLabelSize;
            label->useTextBoundsAsBounds(useLabelSize);
            
            if(useLabelSize) {
                label->reshape(1000, 100);
                label->setTextAlignment(po::ALIGN_CENTER_LEFT);
            } else {
                label->setTextAlignment(po::ALIGN_TOP_CENTER);
            }
            
            doLabelLayout();
            
            return *this;
        }
        
        
        
        
        //------------------------------------------------------------------
		//	Label
		//------------------------------------------------------------------
        #pragma mark Label
        
        
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
            labelOffColor   = color;
            labelOnColor    = color;
            setPressedState();
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setLabelTextOnColor(po::Color color) {
            labelOnColor = color;
            setPressedState();
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
            
            if(this->bSetSizeFromLabel) {
                setSize(label->getWidth(), label->getHeight());
            }
        }
        
        
        
        
        //------------------------------------------------------------------
		//	Off/On States
		//------------------------------------------------------------------
        #pragma mark Off/On States
        
        
        //------------------------------------------------------------------
        Button& Button::setColor(po::Color color) {
            offColor.set(color);
            onColor.set(color);
            
            setPressedState();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setOnColor(po::Color color) {
            onColor.set(color);
            setPressedState();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setImage(po::Object *image) {
            removeImages();
            
            this->offImage = image;
            setOnImage(image);
            
            this->setSize(offImage->getWidth(), offImage->getHeight());
            
            
            //Set BG Transparent by default when using images
            offColor.set(0,0,0,0);
            onColor.set(0,0,0,0);
            
            setPressedState();
            return *this;
        }
        
        
        //------------------------------------------------------------------
        Button& Button::setOnImage(po::Object *onImage) {
            removeOnImage();
            
            this->onImage = onImage;
            onImage->position.set(paddingLeft, paddingTop, 0);
            
            setPressedState();
            
            return *this;
        }
        
        
        //------------------------------------------------------------------
        void Button::removeImages() {
            if(offImage != NULL) {
                removeChild(offImage);
                offImage = NULL;
            }
            
            removeOnImage();
            setPressedState();
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
        void Button::press(bool withMessage) {
            bIsPressed = true;
            setPressedState();
            
            if(withMessage) {
                send_PressedMessage();
            }
        }
        
        
        //------------------------------------------------------------------
        void Button::release(bool withMessage) {
            bIsPressed = false;
            setPressedState();
            
            if(withMessage) {
                sendReleasedMessage();
            }
        }
        
        
        //------------------------------------------------------------------
        bool Button::isPressed() {
            return bIsPressed;
        }
        
        //------------------------------------------------------------------
        void Button::setPressedState() {
            if(isPressed() || isToggled()) {
                bg->fillColor.set(onColor);
                
                if(offImage) removeChild(offImage);
                if(onImage) {
                    addChild(onImage);
                    moveChildToBack(onImage);
                    moveChildToBack(bg);
                } else {
                    label->textColor.set(labelOnColor);
                }
            } else {
                bg->fillColor.set(offColor);
                
                if(onImage) removeChild(onImage);
                if(offImage) {
                    addChild(offImage);
                    moveChildToBack(offImage);
                    moveChildToBack(bg);
                } else {
                    label->visible  = true;
                    label->textColor.set(labelOffColor);
                }
            }
        }
        
        
        
            
        //------------------------------------------------------------------
		//	Messages
		//------------------------------------------------------------------
        #pragma mark Messages
        
        
        //------------------------------------------------------------------
        void Button::send_PressedMessage() {
            po::MessageCenter::broadcastMessage(PRESSED_MESSAGE, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::sendReleasedMessage() {
            po::MessageCenter::broadcastMessage(RELEASED_MESSAGE, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::sendToggledMessage() {
            std::string msg = isToggled() ? TOGGLED_ON_MESSAGE : TOGGLED_OFF_MESSAGE;
            po::MessageCenter::broadcastMessage(msg, this);
        }
        
        
        //------------------------------------------------------------------
        void Button::eventHandler(po::Event *event) {
            switch (event->type) {
                case po::TOUCH_BEGAN_INSIDE_EVENT:
				case po::MOUSE_DOWN_INSIDE_EVENT:
                    press(true);
                    break;
                    
                case po::TOUCH_ENDED_EVENT:
                case po::MOUSE_UP_EVENT:
                    //Send pressed message
                    if(bIsPressed) {
                        if(bToggleEnabled) {
                            toggle();
                            sendToggledMessage();
                        }
                        
                        release(true);
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
			setPressedState();
            
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
