#include "poUIScroller.h"
#include "poMath.h"
#include "poMessageCenter.h"

namespace po {
	namespace UI {
		
#pragma mark - Scroller Class -
		
		Scroller::Scroller()
		: xPosTween(&contentXPos)
		, yPosTween(&contentYPos)
		, bIsTouchEnabled(false)
		, contentXPos(0)
		, contentYPos(0)
		, scrollingThreshold(5)
		, scrollBarPosition(SCROLLBAR_INSIDE)
		, scrollBarSize(5)
		{
			background = new po::RectShape();
			background->fillColor.set(po::Color::transparent);
			addChild(background);
			
			content = new po::Object();
			contentMaskShape = new po::RectShape();
			contentMask = new po::GeometryMask(contentMaskShape, true);
			
			vScrollBar = new ScrollBar();
			vScrollBar->setType(ScrollBar::VERTICAL);
			vScrollBar->visible = false;
			addChild(vScrollBar);
			
			hScrollBar = new ScrollBar();
			hScrollBar->setType(ScrollBar::HORIZONTAL);
			hScrollBar->visible = false;
			addChild(hScrollBar);
			
			po::MessageCenter::addSubscriber(ScrollBar::IS_SCROLLING, this);
		}
		
		Scroller::~Scroller()
		{
		}
		
		#pragma mark update
		
		void Scroller::update()
		{
			xPosTween.update();
			yPosTween.update();
			
			content->position.x = contentXPos;
			contentMaskShape->position.x = -contentXPos;
			content->position.y = contentYPos;
			contentMaskShape->position.y = -contentYPos;
			
			bIsVScrollable = (content->getHeight() > contentMaskShape->getHeight());
			bIsHScrollable = (content->getWidth() > contentMaskShape->getWidth());
			
			// vertical scrolling
			if (bIsVScrollable) {
				vScrollBar->visible = true;
				updateVScrollBar();
			} else {
				vScrollBar->visible = false;
			}
			
			// horizontal scrolling
			if (bIsHScrollable && bHScrollbarEnabled) {
				hScrollBar->visible = true;
				updateHScrollBar();
			} else {
				hScrollBar->visible = false;
			}
			
		}
		
		// updates scrollthumb size, scrolltrack height and scrollthumb position - based on scrollbar position and content position
		void Scroller::updateVScrollBar()
		{
			int scrollBarThumbHeight;
			
			if (bIsHScrollable && scrollBarPosition == SCROLLBAR_INSIDE && bHScrollbarEnabled) {
				float height = contentMaskShape->getHeight() - scrollBarSize;
				scrollBarThumbHeight = (height/content->getHeight()) * contentMaskShape->getHeight();
				vScrollBar->setThumbHeight(scrollBarThumbHeight);
				vScrollBar->setTrackHeight(height);
			} else {
				scrollBarThumbHeight = (contentMaskShape->getHeight()/content->getHeight()) * contentMaskShape->getHeight();
				vScrollBar->setThumbHeight(scrollBarThumbHeight);
			}
			
			if (bIsTouchEnabled) {
				float minScrollerY = 0;
				float maxScrollerY = vScrollBar->getHeight() - scrollBarThumbHeight;
				float thumbYPos = po::Mapf(0, -content->getHeight() + contentMaskShape->getHeight(), content->position.y, 0, maxScrollerY);
				thumbYPos = po::Clamp(minScrollerY, maxScrollerY, thumbYPos);
				vScrollBar->setScrollPosition(po::Point(0, thumbYPos));
			}
		}
		
		void Scroller::updateHScrollBar()
		{
			int scrollBarThumbWidth;
			if (bIsVScrollable && scrollBarPosition == SCROLLBAR_INSIDE) {
				float width = contentMaskShape->getWidth() - scrollBarSize;
				scrollBarThumbWidth = (width/content->getWidth()) * width;
				hScrollBar->setThumbWidth(scrollBarThumbWidth);
				hScrollBar->setTrackWidth(width);
			} else {
				scrollBarThumbWidth = (contentMaskShape->getWidth()/content->getWidth()) * contentMaskShape->getWidth();
				hScrollBar->setThumbWidth(scrollBarThumbWidth);
			}
			
			if (bIsTouchEnabled) {
				float minScrollerX = 0;
				float maxScrollerX = hScrollBar->getWidth() - scrollBarThumbWidth;
				float thumbXPos = po::Mapf(0, -content->getWidth() + contentMaskShape->getWidth(), content->position.x, 0, maxScrollerX);
				thumbXPos = po::Clamp(minScrollerX, maxScrollerX, thumbXPos);
				hScrollBar->setScrollPosition(po::Point(thumbXPos, 0));
			}
		}
		
		#pragma mark eventHandler
		
		void Scroller::eventHandler(po::Event *event)
		{
			if (bIsTouchEnabled) {
				switch (event->type) {
//					case po::MOUSE_DOWN_INSIDE_EVENT:
					case po::TOUCH_BEGAN_INSIDE_EVENT:
						bPressed = true;
						bIsScrolling = false;
						initTouchPos = event->globalPosition;
						prevTouchPos = curTouchPos = initTouchPos;
						if (bVScrollbarEnabled) showVScrollBar();
						if (bHScrollbarEnabled) showHScrollBar();
						break;
//					case po::MOUSE_DRAG_INSIDE_EVENT:
					case po::TOUCH_MOVED_EVENT:
						prevTouchPos = curTouchPos;
						if (bPressed && (bIsVScrollable || bIsHScrollable)) {
							curTouchPos = event->globalPosition;
							scroll(curTouchPos.x - prevTouchPos.x, curTouchPos.y - prevTouchPos.y);
							if (!bIsScrolling) {
								if (fabs(po::Dist(initTouchPos, curTouchPos)) > scrollingThreshold) {
									bIsScrolling = true;
								}
							}
						}
						break;
//					case po::MOUSE_UP_EVENT:
					case po::TOUCH_ENDED_EVENT:
						if (bPressed && (bIsVScrollable || bIsHScrollable)) {
							scroll((curTouchPos.x - prevTouchPos.x) * 8.0f, (curTouchPos.y - prevTouchPos.y) * 8.0f, true);
							bPressed = false;
						}
						break;
					default:
						break;
				}
			}
		}
		
		#pragma mark messageHandler
		
		void Scroller::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender)
		{
			if (msg == ScrollBar::IS_SCROLLING) {
				if (sender == vScrollBar) {
					float newContentYPos = po::Mapf(0, vScrollBar->getHeight(), vScrollBar->getScrollPosition().y, 0, -content->getHeight());
//					std::cout << vScrollBar->getScrollPosition() << ", " << contentYPos << std::endl;
					contentYPos = newContentYPos;
				}
				
				if (sender == hScrollBar) {
					float newContentXPos = po::Mapf(0, hScrollBar->getWidth(), hScrollBar->getScrollPosition().x, 0, -content->getWidth());
					contentXPos = newContentXPos;
				}
			}
			
			if (bIsTouchEnabled) {
				hideVScrollBar();
				hideHScrollBar();
			}
		}
		
#pragma mark scrolling
		
		void Scroller::scroll(float xDiff, float yDiff, bool bAnimate)
		{
			float minXPos	= -content->getWidth() + contentMaskShape->getWidth();
			float maxXpos	= 0;
			float minYPos   = -content->getHeight() + contentMaskShape->getHeight();
			float maxYPos   = 0;
				
			float newXPos;
			if (!bHScrollbarEnabled) {
				newXPos = contentXPos;
			} else {
				newXPos = contentXPos + xDiff;
			}
			
			float newYPos = contentYPos + yDiff;
			
			if(bAnimate) {
				newXPos = std::min(newXPos, maxXpos);
				newXPos = std::max(newXPos, minXPos);
				newYPos = std::min(newYPos, maxYPos);
				newYPos = std::max(newYPos, minYPos);
			} else {
				if (newXPos > maxXpos || newXPos < minXPos) {
					newXPos -= (xDiff /= 2);
				}
				if(newYPos > maxYPos || newYPos < minYPos) {
					newYPos -= (yDiff /= 2);
				}
			}
			
			setPosition(newXPos, newYPos, bAnimate);
		}
		
		// show/hide scroll bars ----------------------
		
		void Scroller::showVScrollBar()
		{
			yPosTween.setNotification(NULL);
			vScrollBar->alphaTween.stop();
			vScrollBar->alpha = 0.9f;
		}
		
		void Scroller::hideVScrollBar()
		{
			vScrollBar->alphaTween
			.set(0.0f)
			.setDuration(0.5f)
			.setDelay(0.5f)
			.setTweenFunction(po::TWEEN_QUAD_OUT_FUNC)
			.start();
		}
		
		void Scroller::showHScrollBar()
		{
			xPosTween.setNotification(NULL);
			hScrollBar->alphaTween.stop();
			hScrollBar->alpha = 0.9f;
		}
		
		void Scroller::hideHScrollBar()
		{
			hScrollBar->alphaTween
			.set(0.0f)
			.setDuration(0.5f)
			.setDelay(0.5f)
			.setTweenFunction(po::TWEEN_QUAD_OUT_FUNC)
			.start();
		}
		
		// Set position ---------------------------------
		
		void Scroller::setPosition(float xposition, float yposition, bool bAnimate)
		{
			if(bAnimate) {
				xPosTween.set(xposition)
				.setDuration(1.0f)
				.setTweenFunction(po::TWEEN_QUAD_OUT_FUNC)
				.setNotification(this)
				.start();
				yPosTween.set(yposition)
				.setDuration(1.0f)
				.setTweenFunction(po::TWEEN_QUAD_OUT_FUNC)
				.setNotification(this)
				.start();
			} else {
				xPosTween.stop();
				contentXPos = xposition;
				yPosTween.stop();
				contentYPos = yposition;
			}
		}
		
		// update scrollbar positions ---------------------------------
		
		void Scroller::updateDisplay()
		{
			switch (scrollBarPosition) {
				case SCROLLBAR_INSIDE:
					vScrollBar->position.x = contentMaskShape->getWidth() - vScrollBar->getWidth();
					hScrollBar->position.y = contentMaskShape->getHeight() - hScrollBar->getHeight();
					break;
				case SCROLLBAR_OUTSIDE:
					vScrollBar->position.x = contentMaskShape->getWidth();
					hScrollBar->position.y = contentMaskShape->getHeight();
					break;
				default:
					break;
			}
		}
		
		// Setters -----------------------------------------------
		
		#pragma mark setters
		
		void Scroller::setSize(int width, int height)
		{
			background->reshape(width, height);
			contentMaskShape->reshape(width, height);
			vScrollBar->setHeight(height);
			hScrollBar->setWidth(width);
			updateDisplay();
		}
		
		void Scroller::setScrollbarSize(int size)
		{
			scrollBarSize = size;
			vScrollBar->setWidth(scrollBarSize);
			hScrollBar->setHeight(scrollBarSize);
			updateDisplay();
		}
		
		void Scroller::setScrollbarPosition(ScrollBarPosition position)
		{
			scrollBarPosition = position;
		}
		
		void Scroller::setScrollingThreshold(int threshold)
		{
			scrollingThreshold = threshold;
		}
		
		void Scroller::setVScrollBarEnabled(bool bEnabled)
		{
			bVScrollbarEnabled = bEnabled;
			vScrollBar->visible = bVScrollbarEnabled;
		}
		
		void Scroller::setHScrollBarEnabled(bool bEnabled)
		{
			bHScrollbarEnabled = bEnabled;
			hScrollBar->visible = bHScrollbarEnabled;
		}
		
		void Scroller::setMaskingEnabled(bool bEnabled)
		{
			if(bEnabled) {
				content->addModifier(contentMask);
			} else {
				content->removeModifier(contentMask);
			}
		}
		
		void Scroller::setContent(po::Object *$content)
		{
			content = $content;
			addChild(content);
			moveChildToFront(vScrollBar);
			moveChildToFront(hScrollBar);
		}
		
		void Scroller::setTouchEnabled(bool isEnabled)
		{
			bIsTouchEnabled = isEnabled;
			vScrollBar->alpha = 0.0f;
			hScrollBar->alpha = 0.0f;
//			addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
//			addEvent(po::MOUSE_DRAG_INSIDE_EVENT, this);
//			addEvent(po::MOUSE_UP_EVENT, this);
			addEvent(po::TOUCH_BEGAN_INSIDE_EVENT, this);
			addEvent(po::TOUCH_MOVED_EVENT, this);
			addEvent(po::TOUCH_ENDED_EVENT, this);
		}
		
		void Scroller::setBackgroundColor(po::Color color)
		{
			background->fillColor.set(color);
		}
		
		// thumb
		void Scroller::setScrollThumbBackgroundColor(po::Color color)
		{
			vScrollBar->setThumbBackgroundColor(color);
			hScrollBar->setThumbBackgroundColor(color);
		}
		
		void Scroller::setScrollThumbImage(std::string imageURL)
		{
			vScrollBar->setThumbImage(imageURL);
			hScrollBar->setThumbImage(imageURL);
		}
		
		void Scroller::setScrollThumbCornerRadius(float radius)
		{
			vScrollBar->setThumbCornerRadius(radius);
			hScrollBar->setThumbCornerRadius(radius);
		}
		
		// track
		void Scroller::setScrollTrackVisible(bool isVisible)
		{
			vScrollBar->setTrackVisible(isVisible);
			hScrollBar->setTrackVisible(isVisible);
		}
		
		void Scroller::setScrollTrackBackgroundColor(po::Color color)
		{
			vScrollBar->setTrackBackgrounColor(color);
			hScrollBar->setTrackBackgrounColor(color);
		}
		
		void Scroller::setVScrollPosition(float position)
		{
//			std::cout << content->position.y << ", " << vScrollBar->getScrollPosition() << std::endl;
			contentYPos = position;
			vScrollBar->setScrollPosition(po::Point(0, position));
		}

// ------------------------------------------------------------------------
// ScrollBar Class
// ------------------------------------------------------------------------
		
#pragma mark - ScrollBar -
		
		const std::string Scroller::ScrollBar::IS_SCROLLING = "IS_SCROLLING";
		
		Scroller::ScrollBar::ScrollBar()
		: xPos(0)
		, yPos(0)
		{
			track = new ScrollTrack();
			addChild(track);

			thumb = new ScrollThumb();
			thumb->addEvent(po::MOUSE_DOWN_INSIDE_EVENT, this);
			thumb->addEvent(po::MOUSE_DRAG_INSIDE_EVENT, this);
			thumb->addEvent(po::MOUSE_UP_EVENT, this);
			addChild(thumb);
		}
		
		Scroller::ScrollBar::~ScrollBar()
		{
		}
		
		void Scroller::ScrollBar::update()
		{
			thumb->position.x = xPos;
			thumb->position.y = yPos;
		}
		
		void Scroller::ScrollBar::eventHandler(po::Event *event)
		{
			switch (event->type) {
				case po::MOUSE_DOWN_INSIDE_EVENT:
					initPosition = event->globalPosition;
					prevPosition = curPosition = initPosition;
					break;
				case po::MOUSE_DRAG_INSIDE_EVENT:
				{
					prevPosition = curPosition;
					curPosition = event->globalPosition;
					if (type == VERTICAL) {
						setVPosition(curPosition.y - prevPosition.y);
					}
					if (type == HORIZONTAL) {
						setHPosition(curPosition.x - prevPosition.x);
					}
					break;
				}
				default:
					break;
			}
		}
		
		void Scroller::ScrollBar::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender)
		{
			
		}
		
		#pragma mark getters
		
		po::Point Scroller::ScrollBar::getScrollPosition()
		{
			return thumb->position;
		}
		
		#pragma mark setters
		
		void Scroller::ScrollBar::setSize(float $width, float $height)
		{
			width = $width;
			height = $height;
			track->setWidth(width);
			track->setHeight(height);
		}
		
		void Scroller::ScrollBar::setWidth(float $width)
		{
			width = $width;
			track->setWidth(width);
			thumb->setWidth(width);
		}
		
		void Scroller::ScrollBar::setHeight(float $height)
		{
			height = $height;
			track->setHeight(height);
			thumb->setHeight(height);
		}
		
		void Scroller::ScrollBar::setVPosition(float yDiff)
		{
			float minYPos = 0;
			float maxYPos = height - thumb->getHeight();
			float newYPos = yPos + yDiff;
			
			if (newYPos >= minYPos && newYPos <= maxYPos) {
				yPos = newYPos;
			}
			
			po::MessageCenter::broadcastMessage(IS_SCROLLING, this);
		}
		
		void Scroller::ScrollBar::setHPosition(float xDiff)
		{
			float minXPos = 0;
			float maxXPos = width - thumb->getWidth();
			float newXPos = xPos + xDiff;
			
			if (newXPos >= minXPos && newXPos <= maxXPos) {
				xPos = newXPos;
			}
			
			po::MessageCenter::broadcastMessage(IS_SCROLLING, this);
		}
		
		void Scroller::ScrollBar::setScrollPosition(po::Point position)
		{
			xPos = position.x;
			yPos = position.y;
		}
		
		void Scroller::ScrollBar::setType(ScrollBarType $type)
		{
			type = $type;
		}
		
		#pragma mark thumb setters
		
		void Scroller::ScrollBar::setThumbHeight(float height)
		{
			thumb->setHeight(height);
		}
		
		void Scroller::ScrollBar::setThumbWidth(float width)
		{
			thumb->setWidth(width);
		}
		
		void Scroller::ScrollBar::setThumbBackgroundColor(po::Color color)
		{
			thumb->setBackgroundColor(color);
		}
		
		void Scroller::ScrollBar::setThumbImage(std::string imageURL)
		{
			thumb->setImage(imageURL);
		}
		
		void Scroller::ScrollBar::setThumbCornerRadius(float radius)
		{
			thumb->setCornerRadius(radius);
		}
		
		#pragma mark track setters
		
		void Scroller::ScrollBar::setTrackVisible(bool isVisible)
		{
			track->visible = isVisible;
		}
		
		void Scroller::ScrollBar::setTrackBackgrounColor(po::Color color)
		{
			track->setBackgroundColor(color);
		}
		
		void Scroller::ScrollBar::setTrackWidth(float $width)
		{
			width = $width;
			track->setWidth($width);
		}
		
		void Scroller::ScrollBar::setTrackHeight(float $height)
		{
			height = $height;
			track->setHeight($height);
		}
		
// ScrollThumb Class ----------------------------------
#pragma mark - ScrollThumb -
		
		Scroller::ScrollThumb::ScrollThumb()
		: CORNER_RADIUS(DEFAULT_RADIUS)
		{
			thumb = new po::RectShape(DEFAULT_SIZE.x, DEFAULT_SIZE.y, CORNER_RADIUS);
			thumb->fillColor.set(po::Color::dkGrey);
			addChild(thumb);
		}
		
		Scroller::ScrollThumb::~ScrollThumb()
		{
		}
		
		#pragma mark setters
		
		void Scroller::ScrollThumb::setWidth(float width)
		{
			thumb->reshape(width, thumb->getHeight(), CORNER_RADIUS);
		}
		
		void Scroller::ScrollThumb::setHeight(float height)
		{
			thumb->reshape(thumb->getWidth(), height, CORNER_RADIUS);
		}
		
		void Scroller::ScrollThumb::setCornerRadius(float radius)
		{
			CORNER_RADIUS = radius;
			thumb->setCornerRadius(radius);
		}
		
		void Scroller::ScrollThumb::setBackgroundColor(po::Color color)
		{
			thumb->fillColor.set(color);
		}
		
		void Scroller::ScrollThumb::setImage(std::string imageURL)
		{
			thumb->placeTexture(new po::Texture(imageURL));
		}
		
// ScrollTrack Class ------------------------------------
		
#pragma mark - ScrollTrack -
		
		Scroller::ScrollTrack::ScrollTrack()
		{
			track = new po::RectShape(DEFAULT_SIZE.x, DEFAULT_SIZE.y);
			track->fillColor.set(po::Color::black);
			track->alpha = 0.3;
			addChild(track);
		}
		
		Scroller::ScrollTrack::~ScrollTrack()
		{
		}
		
		#pragma mark setters
		
		void Scroller::ScrollTrack::setWidth(float width)
		{
			track->reshape(width, track->getHeight());
		}
		
		void Scroller::ScrollTrack::setHeight(float height)
		{
			track->reshape(track->getWidth(), height);
		}
		
		void Scroller::ScrollTrack::setBackgroundColor(po::Color color)
		{
			track->fillColor.set(color);
		}
		
	}
}
