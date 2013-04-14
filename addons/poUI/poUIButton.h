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
            static const std::string PRESSED_MESSAGE;
            static const std::string RELEASED_MESSAGE;
            static const std::string TOGGLED_OFF_MESSAGE;
            static const std::string TOGGLED_ON_MESSAGE;
            
            Button();
            virtual ~Button();
            
            virtual void press(bool withMessage = false);
            virtual void release(bool withMessage = false);
            virtual bool isPressed();
            
            virtual Button& setToggleMode(bool shouldToggle);
            virtual Button& toggle();
            virtual Button& toggleOn();
            virtual Button& toggleOff();
            virtual bool    isToggled();
            
            //Label
            virtual Button& setUseLabelSize(bool useLableSize);
            virtual Button& setLabelText(std::string labelText);
            virtual Button& setLabelFont(po::Font *font);
            virtual Button& setLabelTextSize(float textSize);
            virtual Button& setLabelTextAlignment(po::Alignment alignment);
            virtual Button& setLabelTextColor(po::Color color);
            virtual Button& setLabelTextOnColor(po::Color color);
            
            //BG
            virtual Button& setColor(po::Color color);
            virtual Button& setOnColor(po::Color color);
            
            //Image
            virtual Button& setImage(po::Object *image);
            virtual Button& setOnImage(po::Object *onImage);
            virtual void removeImages();
            virtual void removeOnImage();
            
            //Settings
            virtual Button& setWidth(float width);
            virtual Button& setHeight(float height);
            virtual Button& setSize(float width, float height);
            virtual Button& setCornerRadius(float radius);
            
            //Padding
            virtual Button& setPadding(float padding);
            virtual Button& setPadding(float horizontalPadding, float verticalPadding);
            virtual Button& setPadding(float top, float right, float bottom, float left);
            virtual Button& setPaddingTop(float padding);
            virtual Button& setPaddingRight(float padding);
            virtual Button& setPaddingBottom(float padding);
            virtual Button& setPaddingLeft(float padding);
            
            virtual float getPaddingHorizontal();
            virtual float getPaddingVertical();
            
            //Dictionary
            po::Dictionary dict;
            
            virtual void eventHandler(po::Event *event);
            virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary());
            
        protected:
            po::RectShape *bg;
            po::Color offColor, onColor, labelOffColor, labelOnColor;
            po::Object *offImage, *onImage;
            po::TextBox *label;
            
            void doLabelLayout();
            void setSizeFromLabel();
            bool bSetSizeFromLabel;
            
            virtual void send_PressedMessage();
            virtual void sendReleasedMessage();
            virtual void sendToggledMessage();
            
            virtual void setPressedState();
            
            float paddingTop, paddingLeft, paddingBottom, paddingRight;
            
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
			
			virtual RadioButtonSet& setPadding(int p);
            virtual float           getPadding();
            
			virtual RadioButtonSet& setVertical(bool b);
            
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