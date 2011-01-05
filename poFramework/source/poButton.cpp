/*
 *  poButton.cpp
 *  poCal
 *
 *  Created by Jared Schiffman on 6/8/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poButton.h"


// ====================== poButtonState ====================== //

poButtonState::poButtonState( buttonState state, float w, float h )
: poRectShape(w, h), state(state), backShape(0), textureBg(0), textBox(0), isDefault(true)
{
	this->strokeWidth = 0;
	this->enableFill = false;
	this->setOrigin(PO_ALIGN_LEFT,PO_ALIGN_TOP);
}

poButtonState::poButtonState( buttonState state, char* image, float w, float h, poHoriOrientation hAlign, poVertOrientation vAlign)
: poRectShape(w, h), state(state), backShape(0), textBox(0)
{
	this->strokeWidth = 0;
	this->setOrigin(PO_ALIGN_LEFT,PO_ALIGN_TOP);
	this->enableFill = false;

	addImage(image, w, h, hAlign, vAlign);
}


poButtonState::poButtonState( buttonState state, char* image, char* text, poFont* font, poColor fontColor, float w, float h, poHoriOrientation hAlign, poVertOrientation vAlign)
: poRectShape(w, h), state(state), backShape(0)
{
	this->strokeWidth = 0;
	this->enableFill = false;
	this->setOrigin(PO_ALIGN_LEFT,PO_ALIGN_TOP);

	
	addImage(image, w, h, hAlign, vAlign);
	addTextBox(text, font, fontColor, w, h, hAlign, vAlign);
}

poButtonState::poButtonState( buttonState state,  buttonShapeStyle shapeStyle, poColor fillcolor, float w, float h, float r, poHoriOrientation hAlign, poVertOrientation vAlign)
: poRectShape(w, h), state(state), backShape(0),  textureBg(0)
{
	this->strokeWidth = 0;
	this->enableFill = false;
	this->setOrigin(PO_ALIGN_LEFT,PO_ALIGN_TOP);	

	
	addShape(shapeStyle, fillcolor, w, h, r, hAlign, vAlign);
}


void poButtonState::addShape( buttonShapeStyle shapeStyle,  poColor fillcolor, float w, float h, float r, poHoriOrientation hAlign, poVertOrientation vAlign)
{
	//allow different sizes for diff parts of button
	float width  = (w==0) ? this->rectWidth  : w;
	float height = (h==0) ? this->rectHeight : h;
	float radius = r;
	
	bW = width;     bH = height;
	
	this->style = shapeStyle;
		
	// create back shape based upon button style
	if ( shapeStyle == PO_SHAPE_BUTTON )
		backShape = new poShape2D();
	else if ( shapeStyle == PO_RECT_BUTTON )
		backShape = new poRectShape( width, height );
	else if ( shapeStyle == PO_ROUND_RECT_BUTTON )
		backShape = new  poRoundRectShape( width, height, radius );
	else if ( shapeStyle == PO_OVAL_BUTTON )
		backShape = new poOvalShape( width, height );
		
	backShape->fillColor = fillcolor;

	reshapeBackShape( width, height, hAlign, vAlign );
}

void poButtonState::addImage( char* image, float w, float h, poHoriOrientation hAlign, poVertOrientation vAlign)
{
	//allow different sizes for diff parts of button
	float width  = (w==0) ? this->rectWidth  : w;
	float height = (h==0) ? this->rectHeight : h;

	iW = width; iH = height;
		
	textureBg = new poRectShape(width, height, poColor::white);
	textureBg->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
	
	poTexture* tex = new poTexture(image);
	tex->initWithObject(textureBg);
	textureBg->addModifier(tex);
	
	reshapeImage(width, height, hAlign, vAlign);
}

void poButtonState::addTextBox( const char* text, poFont* font, poColor fontColor, float w, float h, poHoriOrientation hAlign, poVertOrientation vAlign)
{
	//allow different sizes for diff parts of button
	float width  = (w==0) ? this->rectWidth  : w;
	float height = (h==0) ? this->rectHeight : h;
	
	tW = width; tH = height;
	
	textBox = new poTextBox( text, width, height );
	textBox->setFont( font );
	textBox->setPadding( 5 );
	textBox->setFontColor(fontColor);
	
	tHO = hAlign;
	tVO = vAlign;
	
	reshapeTextBox(width, height, hAlign, vAlign);
}


void poButtonState::addTextBox( const char* text, float w, float h, poHoriOrientation hAlign, poVertOrientation vAlign)
{
	//allow different sizes for diff parts of button
	float width  = (w==0) ? this->rectWidth  : w;
	float height = (h==0) ? this->rectHeight : h;
	
	tW = width; tH = height;
	
	textBox = new poTextBox( text, width, height );
	textBox->setFont( poTextBox::getDefaultFont() );
	textBox->setPadding( 5 );
	
	tHO = hAlign;
	tVO = vAlign;
	
	reshapeTextBox(width, height, hAlign, vAlign);
}



void poButtonState::addTextBox( poTextBox* t)
{
	tW = t->getWidth();
	tH = t->getHeight();
	
	textBox = t;
	
	tHO = PO_ALIGN_LEFT;
	tVO = PO_ALIGN_TOP;
	
	reshapeTextBox(tW, tH, PO_ALIGN_LEFT, PO_ALIGN_TOP);
}


void poButtonState::reshapeBackShape( float width,  float height, poHoriOrientation hAlign, poVertOrientation vAlign )
{
	if ( this->style == PO_SHAPE_BUTTON )
	{	// do nothing
	}
	else if ( this->style == PO_RECT_BUTTON ) {
		((poRectShape*)backShape)->reshape( width, height );
	}
	else if ( this->style == PO_ROUND_RECT_BUTTON ) {
		((poRoundRectShape*)backShape)->reshape( width, height );
	}
	else if ( this->style == PO_OVAL_BUTTON ) {
		((poOvalShape*)backShape)->reshape( width, height );
	}	
		
	backShape->setOrigin( hAlign, vAlign );
	reshape(width, height);
}

void poButtonState::reshapeImage( float width, float height, poHoriOrientation hAlign, poVertOrientation vAlign)
{
	textureBg->reshape(width, height);
	textureBg->setOrigin( hAlign, vAlign );
	this->reshape(width, height);
}

void poButtonState::reshapeTextBox( float W, float H, poHoriOrientation hAlign, poVertOrientation vAlign )
{
	textBox->reshape( W, H );
	textBox->setOrigin( hAlign, vAlign );
	textBox->doLayout();
	this->reshape(W, H);
}

void	poButtonState::reshapeWithTextbox()
{
	textBox->reshape( 1000, 1000 );
	textBox->doLayout();
	poBoundingBox box = textBox->getActualBounds();
	float w = box.right - box.left;
	float h = box.top - box.bottom;
		
	reshapeTextBox( w, h);
	if(backShape) reshapeBackShape(w,  h);
	if(textureBg) reshapeImage(w, h);
	
	reshape(w,h);
	
	poButton* p = (poButton*)getParent();
	if(p) {
		p->reshape(w,h);
		p->setHitState(w,h);
	}
}

void poButtonState::reshapeAll(float w, float h)
{
	if(backShape) reshapeBackShape(w,  h);
	if(textureBg) reshapeImage(w, h);
	if(textBox)   reshapeTextBox(w,h);

	reshape(w,h);
	
	poButton* p = (poButton*)getParent();
	if(p) {
		p->reshape(w,h);
		p->setHitState(w,h);
	}
}


void poButtonState::organize()
{
	if(backShape) {
		addChild(backShape);
	}
	if(textureBg) {
		addChild(textureBg);
	}
	if(textBox) {
		addChild(textBox);
	}
	
	//image alignments within button
	if(textureBg) {
		if(textureBg->hOrigin == PO_ALIGN_HORI_CENTER)  textureBg->position.x =+ this->rectWidth/2;	
		if(textureBg->hOrigin == PO_ALIGN_RIGHT)		textureBg->position.x =+ this->rectHeight; 
		if(textureBg->vOrigin == PO_ALIGN_VERT_CENTER)  textureBg->position.y =- this->rectHeight/2;
		if(textureBg->vOrigin == PO_ALIGN_BOTTOM)		textureBg->position.y =- this->rectHeight;
	}
	
	//text box alignments within button
	if(textBox) {
		if( tHO == PO_ALIGN_HORI_CENTER)	textBox->position.x =- (tW - this->rectWidth/2);
		if( tHO == PO_ALIGN_RIGHT)			textBox->position.x =- tW + (this->rectWidth - tW);
		if( tVO == PO_ALIGN_VERT_CENTER)	textBox->position.y =- this->rectHeight/2;
		if( tVO == PO_ALIGN_BOTTOM)			textBox->position.y =- this->rectHeight;
		
		textBox->doLayout();
	}	
}


void poButtonState::update()
{
	poButton* p = (poButton*)getParent();
	if( p->activeStateExists() ) {
		this->visible = (this->state == p->activeButtonState);
	}
	else this->visible = this->isDefault;
}

// ====================== poButton ====================== //

poButton::poButton()
: activeButtonState(PO_BUTTON_UP), poRectShape(0,0), needsReshape(false)
{
	init();
}

void poButton::init()
{
	this->strokeWidth = 0;
	this->setOrigin(PO_ALIGN_LEFT,PO_ALIGN_TOP);	
	this->position.set(0,0);
	this->enableFill = false;
	
	//events
	this->addEvent( PO_MOUSE_PRESS_EVENT, this, "clicked" );
	this->addEvent( PO_MOUSE_RELEASE_EVENT, this, "release" ); 
	this->addEvent( PO_MOUSE_ENTER_EVENT, this, "entered" ); 
	this->addEvent( PO_MOUSE_LEAVE_EVENT, this, "exited" ); 
}

poButton::poButton(float w, float h)
: activeButtonState(PO_BUTTON_UP), poRectShape(w,h)
{
	init();
}


// { PO_BUTTON_UP=0, PO_BUTTON_OVER, PO_BUTTON_DOWN, PO_BUTTON_ALL };
void poButton::addButtonState( poButtonState* button )
{
	if(button->state == PO_BUTTON_ALL) 
	{
		button->state = PO_BUTTON_UP;
		button->isDefault = true; 
	}
	
	if(button->state == PO_BUTTON_UP) button->isDefault = true;
	
	button->organize();
	addChild(button);
	
	existingButtonState.push_back(button->state);
	
	if((this->rectWidth == 0) && (this->rectHeight == 0)) setHitState(button->getWidth(), button->getHeight());
}

void poButton::addButtonStates( poButtonState* up, poButtonState* down, poButtonState* over)
{
	up->organize(); down->organize(); over->organize();
	
	up->state   = PO_BUTTON_UP;
	down->state = PO_BUTTON_DOWN;
	over->state = PO_BUTTON_OVER;
	
	up->isDefault = true;
	
	existingButtonState.push_back(PO_BUTTON_UP);
	existingButtonState.push_back(PO_BUTTON_OVER);
	existingButtonState.push_back(PO_BUTTON_DOWN);

	addChild(up);
	addChild(down);
	addChild(over);
	
	if((this->rectWidth == 0) && (this->rectHeight == 0)) setHitState(up->getWidth(), up->getHeight());
}


void poButton::update()
{
	poButtonState* c;
	for(int i = 0; i < numChildren(); i++)
	{
		c = (poButtonState*)getChild(i);
		if(getWidth() > c->getWidth() )     c->position.x = this->rectWidth/2  -  c->getWidth()/2;
		if(getHeight() > c->getHeight())	c->position.y = this->rectHeight/2 -  c->getHeight()/2;
		c->update();
	}
}


void poButton::setHitState(float width, float height)
{
	this->reshape(width, height);
}


void poButton::setButtonState(buttonState state)
{
	activeButtonState = state;
}

bool  poButton::activeStateExists()
{
	return (find(existingButtonState.begin(), existingButtonState.end(), activeButtonState) != existingButtonState.end() );
}


bool	poButton::eventHandler(poEvent* event)
{
	// set this to be the active text box and determine cursor position
	if ( event->type == PO_MOUSE_PRESS_EVENT)
	{
		activeButtonState = PO_BUTTON_DOWN;
		return true;
	}
	if ( event->type == PO_MOUSE_ENTER_EVENT)
	{
		activeButtonState = PO_BUTTON_OVER;
		return true;
	}
	if ( event->type == PO_MOUSE_RELEASE_EVENT)
	{
		activeButtonState = PO_BUTTON_UP;
		return true;
	}
	if ( event->type == PO_MOUSE_LEAVE_EVENT)
	{
		activeButtonState = PO_BUTTON_UP;
		return true;
	}
	
	activeButtonState = PO_BUTTON_UP; //defualt image when nothing is around
	return true;
}


/*
poButton::poButton()
{
	construct( PO_SHAPE_BUTTON, 0, 0, 0 );
}

poButton::poButton( buttonShapeStyle style, float width, float height, float radius )
{
	construct( style, width, height, radius );
}

void poButton::construct( buttonShapeStyle style, float width, float height, float radius )
{
	this->shapeStyle = style;
	this->width = width;
	this->height = height;
	this->cornerRadius = radius;
		
	// create back shape based upon button style
	if ( shapeStyle == PO_SHAPE_BUTTON )
		backShape = new poShape2D();
	else if ( shapeStyle == PO_RECT_BUTTON )
		backShape = new poRectShape( width, height );
	else if ( shapeStyle == PO_ROUND_RECT_BUTTON )
		backShape = new  poRoundRectShape( width, height, radius );
	else if ( shapeStyle == PO_OVAL_BUTTON )
		backShape = new poOvalShape( width, height );
	addChild( backShape );
	
	reshape( width, height );
	
	//registry.addSeparator( getClassName() );
	//registry.add( "shapeStyle", (int*) &this->shapeStyle );
	//registry.add( "width", &this->width );
	//registry.add( "height", &this->height );
	//registry.add( "cornerRadius", &this->cornerRadius );
}

//poObject* poButton::duplicate( bool copyChildren, poObject* copyIntoObject )
//{
//	poButton* obj = (poButton*) copyIntoObject;
//	if ( obj==NULL )
//		obj = new poButton( shapeStyle, width, height, cornerRadius );
//	
//	obj->reshape( width, height );
//	
//	poObject::duplicate( copyChildren, obj );
//	return obj;
//}

//void poButton::init()
//{
//	reshape( width, height );	
//	poObject::init();
//}


void	poButton::reshape( float width,  float height )
{
	if ( shapeStyle == PO_SHAPE_BUTTON )
	{	// do nothing
	}
	else if ( shapeStyle == PO_RECT_BUTTON )
		((poRectShape*)backShape)->reshape( width, height );
	else if ( shapeStyle == PO_ROUND_RECT_BUTTON )
		((poRoundRectShape*)backShape)->reshape( width, height );
	else if ( shapeStyle == PO_OVAL_BUTTON )
		((poOvalShape*)backShape)->reshape( width, height );
		
	backShape->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
}


// ====================== poTextButton ====================== //

poTextButton::poTextButton() : poButton( PO_RECT_BUTTON, 100, 50, 0 )
{
	construct( "button", poTextBox::getDefaultFont() );
}

poTextButton::poTextButton( char *text, poFont* font, buttonShapeStyle style, float width, float height, float radius ) : 
	poButton( style, width, height, radius )
{
	construct( text, font );
}

void	poTextButton::construct( char *text, poFont* font )
{
	textBox = new poTextBox( text, width, height );
	textBox->setFont( font );
	textBox->setPadding( 5 );
	addChild( textBox );
}

void	poTextButton::reshape( float W, float H )
{
	poButton::reshape( W, H );
	textBox->reshape( W, H );
	textBox->doLayout();
		
	textBox->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
}

void	poTextButton::reshapeWithTextbox()
{
	textBox->reshape( 1000, 1000 );
	textBox->doLayout();
	poBoundingBox box = textBox->getActualBounds();
	printf("%f %f %f %f\n", box.left, box.bottom, box.right, box.top );
	poButton::reshape( box.right - box.left, box.top - box.bottom );
	textBox->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
}


// ====================== poImageButton ====================== //

poImageButton::poImageButton() : poButton( PO_RECT_BUTTON, 100, 50, 0 ), state(0)
{
	construct(0); 
}


poImageButton::poImageButton( char* imageFile, buttonShapeStyle style, float width, float height, float radius)
: poButton( style, width, height, radius ), state(0)
{
	//initial image is set as backShape texture
	constructBackShape(imageFile);
	
	this->addEvent( PO_MOUSE_PRESS_EVENT, this, "clicked" );
	this->addEvent( PO_MOUSE_RELEASE_EVENT, this, "release" ); 
	this->addEvent( PO_MOUSE_ENTER_EVENT, this, "entered" ); 
	this->addEvent( PO_MOUSE_LEAVE_EVENT, this, "exited" );
}


void poImageButton::construct( char* imageFile )
{
	
	poRectShape* childShape = new poRectShape(width, height,poColor::white);
	childShape->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
	
	poTexture* tex = new poTexture(imageFile);
	tex->initWithObject(childShape);
	childShape->addModifier(tex);

	addChild(childShape);

	reshape(width, height);
}


void poImageButton::constructBackShape(char* imageFile)
{
	backShape->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );
	
	poTexture* tex = new poTexture(imageFile);
	tex->initWithObject(backShape);
	backShape->addModifier(tex);

	reshape(width, height);
}


void poImageButton::addImage(  char* imageFile )
{
	construct(imageFile);
}
	
void poImageButton::reshape( float W, float H )
{
	for(int i = 0; i < numChildren(); i++)
	{
		poShape2D* childShape = (poShape2D*)getChild(i);
	
		if ( shapeStyle == PO_SHAPE_BUTTON )
		{	// do nothing
		}
		else if ( shapeStyle == PO_RECT_BUTTON )
			((poRectShape*)childShape)->reshape( width, height );
		else if ( shapeStyle == PO_ROUND_RECT_BUTTON )
			((poRoundRectShape*)childShape)->reshape( width, height );
		else if ( shapeStyle == PO_OVAL_BUTTON )
			((poOvalShape*)childShape)->reshape( width, height );
			
		childShape->setOrigin( PO_ALIGN_LEFT, PO_ALIGN_TOP );

     }
}
	
void poImageButton::update()
{
	for(int i = 0; i < numChildren(); i++)
	{
		if(state == i) getChild(i)->visible = true;
		else getChild(i)->visible = false;
	}
}


bool	poImageButton::eventHandler(poEvent* event)
{
	msg = event->message;
	// set this to be the active text box and determine cursor position
	if ( event->type == PO_MOUSE_PRESS_EVENT && numChildren() >= 2)
	{
		state = 1; //when pressed down
		return true;
	}
	if ( event->type == PO_MOUSE_ENTER_EVENT)
	{
		state = 2; //hover
		return true;
	}
	if ( event->type == PO_MOUSE_RELEASE_EVENT && numChildren() >= 4)
	{
		state = 3;  //when released
		return true;
	}
	if ( event->type == PO_MOUSE_LEAVE_EVENT)
	{
		state = 0;
		return true;
	}
	
	state = 0; //defualt image when nothing is around
	return true;
}
*/

