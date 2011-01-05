/*
 *  poButton.h
 *  poCal
 *
 *  Created by Jared Schiffman on 6/8/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#ifndef PO_BUTTON_H
#define  PO_BUTTON_H

#include "poCommon.h"
#include "poObject.h"
#include "poShapePrimitives.h"
#include "poTextBox.h"

enum buttonShapeStyle { PO_SHAPE_BUTTON, PO_RECT_BUTTON, PO_ROUND_RECT_BUTTON, PO_OVAL_BUTTON };

enum buttonState      { PO_BUTTON_UP=0, PO_BUTTON_OVER, PO_BUTTON_DOWN, PO_BUTTON_ALL };

class poButton;

class poButtonState : public poRectShape
{
public:
	poButtonState( buttonState state, float w, float h);
	poButtonState( buttonState state, char* image, float w, float h, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	poButtonState( buttonState state,  buttonShapeStyle shapeStyle, poColor fillcolor, float w, float h, float r, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	poButtonState( buttonState state, char* image, char* text, poFont* font, poColor fontColor, float w, float h, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	
	void addShape( buttonShapeStyle shapeStyle,  poColor fillcolor, float w=0, float h=0, float r=0, poHoriOrientation hAlign= PO_ALIGN_LEFT, poVertOrientation vAlign = PO_ALIGN_TOP);
	void addImage( char* image, float W=0, float H=0, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	
	void addTextBox( const char* text, poFont* font, poColor fontColor, float W=0, float H=0, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	void addTextBox( const char* text, float w=0, float h =0, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	void addTextBox( poTextBox* t);
		
	void reshapeBackShape( float width,  float height, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	void reshapeImage( float width, float height, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	void reshapeTextBox( float width, float height, poHoriOrientation hAlign = PO_ALIGN_LEFT, poVertOrientation vAlign= PO_ALIGN_TOP);
	void reshapeWithTextbox();
	void reshapeAll(float w, float h);
	
	virtual void    update();
	
	void organize();

	buttonShapeStyle		style;
	buttonState				state;	
	poShape2D*				backShape;  
	poTexture*				tex;
	poRectShape*			textureBg;
	poTextBox*				textBox;	
	float					cornerRadius;	
	bool					isDefault;
	
protected:
	float bW, bH; //backshape info
	float iW, iH; //image info
	float tW, tH; //textbox info 
	
	poHoriOrientation tHO;
	poVertOrientation tVO;
};

//poButton has a hit area that is never disabled. By default it is the size of the button states, 
//or it can be explicitly specified to a larger size
class poButton : public poRectShape
{
public:
	poButton();		
	poButton(float w, float h);	//specifies hit area
		
	void addButtonStates( poButtonState* up, poButtonState* down, poButtonState* over);
	void addButtonState( poButtonState* button);
	
	void setButtonState(buttonState state);
	void setHitState(float width, float height);
	
	bool activeStateExists();
	void init();
	
	virtual void	update();
	virtual bool	eventHandler(poEvent* event);
		
	std::vector<int>			existingButtonState;
	buttonState					activeButtonState;
	bool						needsReshape;
};


/* class poButton : public poObject
{	
public:
	poButton();
	poButton( buttonShapeStyle style, float width, float height, float radius=0 );
	
	void construct( buttonShapeStyle style, float width, float height, float radius=0 );
	
	//virtual poObject*	duplicate( bool copyChildren, poObject* copyIntoObject=NULL );
	//virtual char*		getClassName() { return "poButton"; };
	//virtual void		init();
	
	virtual void	reshape( float W, float H );

	poShape2D*			backShape;
	buttonShapeStyle	shapeStyle;
	int					width, height, cornerRadius;
};
*//*
class poTextButton : public poButton
{
public:
	poTextButton();
	poTextButton( char *text, poFont* font, buttonShapeStyle style, float width, float height, float radius=0 );
	
	void	construct( char *text, poFont* font );
	
	virtual void	reshape( float W, float H );
	void			reshapeWithTextbox();
	
	poTextBox*		textBox;
};

*//*
class poImageButton : public poButton
{
public:
	poImageButton();
	poImageButton( char* imageFile, buttonShapeStyle style, float width, float height, float radius=0 );

	void construct( char* imageFile );
	void constructBackShape(char* imageFile);
	void addImage(  char* imageFile );
	
	virtual void	reshape( float W, float H );
	virtual void	update();
	virtual bool	eventHandler(poEvent* event);

	int state;
	std::string msg;
};
*/

#endif