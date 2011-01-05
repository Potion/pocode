/*
 *  poTextEntryBox.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/25/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#ifndef PO_TEXT_ENTRY_BOX
#define PO_TEXT_ENTRY_BOX

#include "poTextBox.h"


class poTextEntryBox : public poTextBox
{
public:
	poTextEntryBox();
	poTextEntryBox(const char* text, int width, int height);
	poTextEntryBox(const char* text, int width, int height, const poColor& fontColor );
	poTextEntryBox(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor );
	
	virtual poObject*		duplicate(bool copyChildren=true, poObject* copyInto=NULL);
	virtual const char*		getClassName() { return "poTextEntryObject"; };

	virtual void			draw();
	virtual bool			eventHandler(poEvent* event);

	void					setAlertObject( poObject* obj ) { alertOnEnterObject = obj; };

	static poTextEntryBox*	activeTextEntryBox;
	
protected:
	void construct();
	
	virtual void activateCallback();
	virtual void deactivateCallback();

	int			cursorPos, cursorPos2;
	bool		ignoreRelease;
	poObject*	alertOnEnterObject;
};

#endif
