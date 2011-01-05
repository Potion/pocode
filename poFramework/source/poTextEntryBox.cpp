/*
 *  poTextEntryBox.cpp
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/25/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poTextEntryBox.h"

poTextEntryBox*	poTextEntryBox::activeTextEntryBox = NULL;

poTextEntryBox::poTextEntryBox()
{
	construct();
}

poTextEntryBox::poTextEntryBox(const char* text, int width, int height) : poTextBox(text,width,height)
{
	construct();
}

poTextEntryBox::poTextEntryBox(const char* text, int width, int height, const poColor& fontColor ) : poTextBox(text,width,height,fontColor)
{
	construct();
}

poTextEntryBox::poTextEntryBox(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor )
: poTextBox(text,width,height,fontColor,fillColor,strokeWidth,strokeColor)
{
	construct();
}
	
poObject* poTextEntryBox::duplicate(bool copyChildren, poObject* copyInto)
{
	poTextEntryBox* obj = (poTextEntryBox*)copyInto;
	if(obj == NULL)
		obj = new poTextEntryBox();
		
	
	poTextBox::duplicate(copyChildren, obj);
	return obj;
}


void poTextEntryBox::construct()
{
	setPadding(6.0);
	
	this->addEvent( PO_MOUSE_PRESS_EVENT, this, "clicked" );
	this->addEvent( PO_MOUSE_RELEASE_EVENT, this, "release" );
	// this->addEvent( PO_KEY_DOWN_EVENT, this, "key" );
	
	cursorPos = cursorPos2 = 0;
	ignoreRelease = false;
	alertOnEnterObject = NULL;
}

void poTextEntryBox::activateCallback()
{
	strokeWidth = 2;
}

void poTextEntryBox::deactivateCallback()
{
	strokeWidth = 0;
}


void	poTextEntryBox::draw()
{	
	if ( this == activeTextEntryBox )
		activateCallback();
	else
		deactivateCallback();
	
	poTextBox::draw();
	
		
	if ( this != activeTextEntryBox )
		return;
		
	// limit cursorPos
	if ( cursorPos < 0 ) cursorPos = 0;
	if ( cursorPos >= letterList.size() ) cursorPos = letterList.size();
	
	// establish cursorPosPoint (location)
	poPoint cursorPosPoint;
	if ( letterList.size() == 0 )
		cursorPosPoint.set( boundingBox.left+padding.left, boundingBox.top-fontLeading );
	else if ( cursorPos < letterList.size() )
		cursorPosPoint = letterList[cursorPos].position;
	else
	{
		cursorPosPoint = letterList[cursorPos-1].position;
		cursorPosPoint.x += font->getCharWidth( letterList[cursorPos-1].c ) * letterList[cursorPos-1].scale;
	}

		
	// draw cursor
	if ( cursorPos == cursorPos2 || cursorPos2==-1 )
	{
		glPushMatrix();
		glTranslatef( cursorPosPoint.x, cursorPosPoint.y, 0 );
		fontColor.apply();
		glRectf( 0, fontLeading*-0.2, 1, fontLeading*0.8 );
		glPopMatrix();
	}
	
	// limit cursorPos2
	if ( cursorPos2 != -1 && cursorPos2 != cursorPos )
	{
		if ( cursorPos2 < 0 ) cursorPos2 = 0;
		if ( cursorPos2 >= letterList.size() ) cursorPos2 = letterList.size();
		
		// establish cursorPosPoint2 (location)
		/*if ( letterList.size() == 0 )
			cursorPosPoint.set( boundingBox.left+padding.left, boundingBox.top-fontLeading );
		else if ( cursorPos2 < letterList.size() )
			cursorPosPoint = letterList[cursorPos2].position;
		else
		{
			cursorPosPoint = letterList[cursorPos2-1].position;
			cursorPosPoint.x += font->getCharWidth( letterList[cursorPos2-1].c ) * letterList[cursorPos2-1].scale;
		}*/
		
		// draw cursor2
		/*glPushMatrix();
		glTranslatef( cursorPosPoint.x, cursorPosPoint.y, 0 );
		fontColor.apply();
		glRectf( 0, fontLeading*-0.2, 2, fontLeading*0.8 );
		glPopMatrix();*/
	}
	
	// draw selection
	if ( cursorPos != cursorPos2 )
	{
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		poPoint P, P2, D;
		glColor4f( 0,1,0,0.5 );
		for( int i=cursorPos; i<cursorPos2; i++ )
		{
			int next = (i==letterList.size()-1) ? letterList.size()-1 : i+1;
			
			P = letterList[i].position;
			P2 = letterList[next].position;
			if ( P2.x < P.x || i==next )
			{
				P2 = letterList[i].position;
				P2.x += font->getCharWidth( letterList[cursorPos2-1].c ) * letterList[cursorPos2-1].scale;
			}
			glRectf( P.x, P.y-fontLeading*0.2, P2.x, P.y+fontLeading*0.8 );
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}


bool	poTextEntryBox::eventHandler(poEvent* event)
{
	// set this to be the active text box and determine cursor position
	if ( event->type == PO_MOUSE_PRESS_EVENT )
	{
		activeTextEntryBox = this;
		
		int prevCursorPos = cursorPos;
		cursorPos = 0;
		if ( letterList.size()==0 )
			return true;
		
		// determine cursor position
		for( int i=0; i<letterList.size(); i++ )
		{
			//int next = (i==letterList.size()-1) ? letterList.size()-1 : i+1;
			
			if ( event->localY > letterList[i].position.y-fontLeading*0.25 && event->localY < letterList[i].position.y + fontLeading*0.855 &&
				 event->localX > letterList[i].position.x + font->getCharWidth( letterList[i].c )/2.0 )
			{
				cursorPos = i+1;
			}
		}
		if ( event->localY < letterList[ letterList.size()-1 ].position.y-fontLeading*0.25 )
			cursorPos = letterList.size();

		// do word selection on double click
		if ( cursorPos == prevCursorPos )
		{
			for( int i=cursorPos-1; i>=0; i-- )
			{
				if ( isspace(letterList[i].c) || i==0 ) 
				{
					cursorPos2 = cursorPos;
					cursorPos = i+1;
					if ( i==0 ) cursorPos = 0;
					break;
				}
			}
			
			for( int i=cursorPos2; i<letterList.size(); i++ )
			{
				if ( isspace(letterList[i].c) || i==letterList.size()-1 ) 
				{
					cursorPos2 = i;
					if ( i==letterList.size()-1 )
						cursorPos2 = i+1;
					break;
				}
			}
			
			ignoreRelease = true;
		}

		return true;
	}
	
	// set this to be the active text box and determine cursor position
	if ( event->type == PO_MOUSE_RELEASE_EVENT )
	{
		if ( ignoreRelease )
		{
			ignoreRelease = false;
			return true;
		}
			
		activeTextEntryBox = this;
		cursorPos2 = 0;
		if ( letterList.size()==0 )
			return true;
			
		for( int i=0; i<letterList.size(); i++ )
		{
			//int next = (i==letterList.size()-1) ? letterList.size()-1 : i+1;
			
			if ( event->localY > letterList[i].position.y-fontLeading*0.25 && event->localY < letterList[i].position.y + fontLeading*0.855 &&
				 event->localX > letterList[i].position.x + font->getCharWidth( letterList[i].c )/2.0 )
			{
				cursorPos2 = i+1;
			}
		}
		
		if ( event->localY < letterList[ letterList.size()-1 ].position.y-fontLeading*0.25 )
			cursorPos2 = letterList.size();
			
		if ( cursorPos2 == cursorPos )
			cursorPos2 = -1;
		else if ( cursorPos2 < cursorPos )
		{
			int swap = cursorPos;
			cursorPos = cursorPos2;
			cursorPos2 = swap;
		}

		return true;
	}
	
	// handle key entry
	if ( event->type == PO_KEY_DOWN_EVENT )
	{
		if ( this != activeTextEntryBox )
			return false;
				
		if ( alertOnEnterObject != NULL && (event->key==13) )		// note: 3 is overloaded with right arrow key
		{
			alertOnEnterObject->messageHandler( "text_update" );
			activeTextEntryBox = NULL;
			return true;
		}
		
		// handle arrow keys
		if ( event->key < 4 )
		{
			if ( event->key == 0  )
			{
				cursorPos -= ( boundingBox.right - boundingBox.left ) / font->info.fontSize * 2.0;
				if ( cursorPos < 0 ) 
					cursorPos = 0;
			}
			if ( event->key == 1 )
			{
				cursorPos += ( boundingBox.right - boundingBox.left ) / font->info.fontSize * 2.0;
				if ( cursorPos >= letterList.size() ) 
					cursorPos = letterList.size();
			}
			if ( event->key == 2 && cursorPos > 0 )
				cursorPos--;
			if ( event->key==3 && cursorPos <= letterList.size() )
				cursorPos++;
			return true;
		}
		
		// handle delete key
		if ( event->key == 127 )
		{
			// delete selection
			if ( cursorPos2 != -1 && cursorPos2 > cursorPos )
			{
				text.erase( cursorPos, cursorPos2-cursorPos );
				setText( text.c_str() );
				cursorPos2 = -1;
				doLayout();
			}
			// delete individual character
			else if ( letterList.size() > 0 && cursorPos>0 )
			{
				text.erase( cursorPos-1, 1 );
				setText( text.c_str() );
				cursorPos--;
				doLayout();
			}
			return true;
		}
		
		// handle all other keys
		if ( isalnum(event->key) || ispunct(event->key) || isspace(event->key) )
		{
			// delete selection if it exists
			if ( cursorPos2 != -1 && cursorPos2 > cursorPos )
			{
				text.erase( cursorPos, cursorPos2-cursorPos );
				cursorPos2 = -1;
			}
			
			char str[8];
			sprintf( str, "%c", event->key );
			
			text.insert( cursorPos, str );
			setText( text.c_str() );
			doLayout();
			cursorPos++;
		}
		return true;
	}
	
	return false;
}




