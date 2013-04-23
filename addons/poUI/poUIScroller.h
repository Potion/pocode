#pragma once

/*	Created by bruce on 2/25/13.
 *	Copyright 2013 Potion Design. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poGeometryMask.h"

namespace po {
	namespace UI {
		
		#pragma mark - Scroller -
		
		class Scroller : public po::Object
		{
		public:
			enum ScrollBarPosition {
				SCROLLBAR_INSIDE,
				SCROLLBAR_OUTSIDE
			};
			
			Scroller();
			virtual ~Scroller();
			
			virtual void update();
			virtual void eventHandler(po::Event *event);
			virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
			
			void setSize(int width, int height);
			void setScrollbarSize(int size);
			void setScrollbarPosition(ScrollBarPosition position);
			void setScrollingThreshold(int threshold);
			void setVScrollBarEnabled(bool bEnabled);
			void setHScrollBarEnabled(bool bEnabled);
			void setMaskingEnabled(bool bEnabled);
			void setContent(po::Object *content);
			void setTouchEnabled(bool isEnabled);
			void setBackgroundColor(po::Color color);
			
			// thumb
			void setScrollThumbBackgroundColor(po::Color color);
			void setScrollThumbImage(std::string imageURL);
			void setScrollThumbCornerRadius(float radius);
			
			// track
			void setScrollTrackVisible(bool isVisible);
			void setScrollTrackBackgroundColor(po::Color color);
			
			void setVScrollPosition(float position);
		protected:
			class ScrollBar;
			class ScrollThumb;
			class ScrollTrack;
			
			po::Object *content;
			po::RectShape *background, *contentMaskShape;
			po::GeometryMask* contentMask;
			po::Point initTouchPos, prevTouchPos, curTouchPos;
			po::Tween<float> xPosTween;
			po::Tween<float> yPosTween;
			float contentXPos, contentYPos;
			
			ScrollBar *vScrollBar, *hScrollBar;
			ScrollBarPosition scrollBarPosition;
			
			int  scrollingThreshold, scrollBarSize;
			bool bIsVScrollable, bIsHScrollable, bIsTouchEnabled, bPressed, bIsScrolling;
			bool bVScrollbarEnabled, bHScrollbarEnabled;
			
			void scroll(float xDiff, float yDiff, bool bAnimate=false);
			void showVScrollBar();
			void hideVScrollBar();
			void showHScrollBar();
			void hideHScrollBar();
			void setPosition(float xPos, float yPos, bool bAnimate=false);
			void updateDisplay();
			void updateVScrollBar();
			void updateHScrollBar();
			
			#pragma mark - ScrollBar -
			
			class ScrollBar : public po::Object
			{
			public:
				static const std::string IS_SCROLLING;
				
				enum ScrollBarType {
					VERTICAL,
					HORIZONTAL
				};
				
				ScrollBar();
				virtual ~ScrollBar();
				
				virtual void update();
				virtual void eventHandler(po::Event *event);
				virtual void messageHandler(const std::string &msg, const po::Dictionary& dict=po::Dictionary(), const po::Object *sender=NULL);
				
				void setSize(float width, float height);
				void setWidth(float width);
				void setHeight(float height);
				void setThumbHeight(float height);
				void setThumbWidth(float width);
				void setVPosition(float ypos);
				void setHPosition(float xpos);
				po::Point getScrollPosition();
				void setScrollPosition(po::Point position);
				void setType(ScrollBarType type);
				void setThumbBackgroundColor(po::Color color);
				void setThumbImage(std::string imageURL);
				void setThumbCornerRadius(float radius);
				void setTrackVisible(bool isVisible);
				void setTrackBackgrounColor(po::Color color);
				void setTrackWidth(float width);
				void setTrackHeight(float height);
			private:
				ScrollBarType type;
				float width, height, xPos, yPos;
				po::Point initPosition, prevPosition, curPosition;
				ScrollThumb *thumb;
				ScrollTrack *track;
			};

			#pragma mark - ScrollThumb -

			class ScrollThumb : public po::Object
			{
			public:
				ScrollThumb();
				virtual ~ScrollThumb();
				
				void setWidth(float width);
				void setHeight(float height);
				void setCornerRadius(float radius);
				void setBackgroundColor(po::Color color);
				void setImage(std::string imageURL);
			private:
				const po::Point DEFAULT_SIZE = po::Point(5, 5);
				const float DEFAULT_RADIUS = 2.5;
				
				po::RectShape *thumb;
				float CORNER_RADIUS;
			};
			
			#pragma mark - ScrollTrack -
			
			class ScrollTrack : public po::Object
			{
			public:
				ScrollTrack();
				virtual ~ScrollTrack();
				
				void setWidth(float width);
				void setHeight(float height);
				void setBackgroundColor(po::Color color);
			private:
				const po::Point DEFAULT_SIZE = po::Point(5, 5);
				po::RectShape *track;
			};
			

		};
	}
}