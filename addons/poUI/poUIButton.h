#pragma once

/*	Created by Steve Varga on 1/11/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2d.h"
#include "poTextBox.h"

namespace po {
    namespace UI {
        class Button : public po::Object {
        public:
            static const std::string DOWN_MESSAGE;
            static const std::string PRESSED_MESSAGE;
            static const std::string TOGGLED_OFF_MESSAGE;
            static const std::string TOGGLED_ON_MESSAGE;
            
            Button();
            virtual ~Button();
            
            virtual bool isPressed();
            
            virtual Button& setToggleMode(bool shouldToggle);
            virtual Button& toggle();
            virtual Button& toggleOn();
            virtual Button& toggleOff();
            virtual bool isToggled();
            
            virtual Button& setUseLabelSize(bool useLableSize);
            virtual Button& setLabelText(std::string labelText);
            virtual Button& setLabelFont(po::Font *font);
            virtual Button& setLabelTextSize(float textSize);
            virtual Button& setLabelTextAlignment(po::Alignment alignment);
            virtual Button& setLabelTextColor(po::Color color);
            
            virtual Button& setOffColor(po::Color color);
            virtual Button& setOnColor(po::Color color);
            
            virtual Button& setWidth(float width);
            virtual Button& setHeight(float height);
            virtual Button& setSize(float width, float height);
            virtual Button& setPadding(float padding);
            virtual Button& setCornerRadius(float radius);
            
            virtual Button& setOffImage(po::Object *upImage, bool andUseSize = false);
            virtual void removeOffImage();
            virtual Button& setOnImage(po::Object *downImage, bool andUseSize = false);
            virtual void removeOnImage();
            
            virtual void eventHandler(po::Event *event);
            virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
            
        protected:
            po::RectShape *bg;
            po::Color offColor, onColor;
            po::Object *offImage, *onImage;
            po::TextBox *label;
            
            void doLabelLayout();
            void setSizeFromLabel();
            bool bSetSizeFromLabel;
            
            virtual void sendDownMessage();
            virtual void sendPressedMessage();
            virtual void sendToggledMessage();
            
            void setPressedState();
            
            float padding;
            
            bool bToggleEnabled;
            bool bIsPressed, bIsToggled;
            
        };
		
		
		class RadioButton : public Button {
		public:
            static const std::string SELECTED_MESSAGE;
            static const std::string SELECTED_PRESSED_MESSAGE;
            
			RadioButton();
			virtual ~RadioButton();
			
			virtual RadioButton& setOnColor(po::Color c);
			virtual RadioButton& setOffColor(po::Color c);
            
			virtual void eventHandler(po::Event *event);
			
        private:
			virtual void sendSelectedMessage();
            virtual void sendSelectedPressedMessage();
            
			bool bSelected;
            
            friend class RadioButtonSet;
		};
		
		
		class RadioButtonSet : public po::Object {
		public:
			RadioButtonSet();
			virtual ~RadioButtonSet();
			
			virtual RadioButtonSet& addButtons(std::vector<RadioButton *> btns);
			virtual RadioButtonSet& addButton(RadioButton *button);
			virtual RadioButtonSet& removeButton(RadioButton *button);
			
			virtual RadioButtonSet& setButtonOnColors(po::Color c);
			virtual RadioButtonSet& setButtonOffColors(po::Color c);
			
			virtual RadioButtonSet& setButtonWidths(float width);
            virtual RadioButtonSet& setButtonHeights(float height);
            virtual RadioButtonSet& setButtonSizes(float width, float height);
			
			virtual RadioButtonSet& setVertical(bool b);
			virtual RadioButtonSet& setPadding(int p);
			virtual RadioButtonSet& doLayout(bool firstSelected=false);
			
			virtual bool isFromThisSet(RadioButton *button);
						
			virtual void eventHandler(po::Event *event);
            virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
			
		protected:
			std::vector<RadioButton *> radioButtons;
			
		private:
			RadioButton	*selected;
			
			int		padding;
			bool	bVertical;
		};
    }
}