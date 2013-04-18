#pragma once

/*	Created by Ilan Schifter on 3/28/13.
 *	Copyright 2013 Potion. All rights reserved.
 */

#include "poObject.h"
#include "poShapeBasics2D.h"
#include "poTextBox.h"
#include "poMessageCenter.h"


class TextField : public poObject
{
public:
	static const std::string TEXT_EDIT_BEGAN_MESSAGE;
	static const std::string TEXT_EDIT_DONE_MESSAGE;
	static const std::string TEXT_CHANGED_MESSAGE;
	
	TextField(float w=200, float h=26);
	virtual ~TextField();
	
	virtual TextField& init();
	
	virtual TextField& setEditable(bool enableEdit);
	virtual TextField& setEditMode(bool b);
	virtual bool isEditable();
	virtual bool isBeingEdited();
	
	virtual TextField& setText(std::string s);
	virtual std::string getText();
	virtual TextField& clearText();
	virtual TextField& setDefaultText(std::string s);
	
	//BG
	virtual TextField& setImage(std::string path);
	virtual TextField& setColor(poColor color);
	virtual TextField& setSize(float w, float h);
	virtual TextField& setPadding(float p);
	virtual TextField& setCornerRadius(float radius);
	virtual TextField& setStrokeColor(poColor c);
	virtual TextField& setStrokeSize(float f);

	//TEXT
	virtual TextField& setTextColor(poColor color);
	virtual TextField& setFont(poFont* font);
	virtual TextField& setTextSize(float size);
	
	//CURSOR
	virtual TextField& setCursorColor(poColor color);
	virtual TextField& setCursorBlinkSpeed(float seconds);
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary(), const poObject *sender=NULL);

protected:
	virtual void sendDoneMessage();
	virtual void sendBeganMessage();
	virtual void sendChangedMessage();
	
	virtual bool isTextDefaultText();
	
	poRectShape *bg;
	poTextBox *text;
	poLineShape* cursor;
	
	poColor offColor, onColor;
	
	bool editable;
	bool edit_mode;
	bool hasDefaultText;
	
	std::string defaultText;
	
	float padding;
	float last_sec;
	float blink_gap_secs;
};