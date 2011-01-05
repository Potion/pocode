/*
 *  poTextBox.h
 *  potionApp
 *
 *  Created by Jared Schiffman on 9/14/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poObject.h"
#include "poFont.h"

#ifndef _PO_TEXTBOX_H_
#define _PO_TEXTBOX_H_

class poFont;

struct poTBLetter
{
	char c;
	poPoint position;

	poFont* font;
	poColor color;
	float scale;
	float tracking;
	
	poPoint offset;
};

typedef std::vector<poTBLetter> poTBLetterVector;
typedef std::vector<char> charVector;

class poTextBox : public poObject
{
public:
	poTextBox();
	poTextBox(int width, int height);
	poTextBox(const char* text, int width, int height);
	poTextBox(const char* text, int width, int height, const poColor& fontColor );
	poTextBox(const char* text, int width, int height, const poColor& fontColor, const poColor& fillColor, int strokeWidth, const poColor& strokeColor );
	virtual	~poTextBox();

	virtual poObject*	duplicate(bool copyChildren=true, poObject* copyInto=NULL);
	virtual const char*	getClassName() { return "poTextBox"; }
	virtual void		init();

	virtual void draw();
	virtual void update();

	virtual void		calculateBounds();
	virtual	void		calculateRelativeBounds( poObject* relativeObj, poBoundingBox& bbox );
	poBoundingBox		getActualBounds() { return actualTextBounds; };
	

	void		setText(const char* txt);
	void		appendText( const char* text );
	const char*	getText() { return text.c_str(); };

	void	doLayout();
	
	void	resetFont();
	void	setFont(poFont* font, int startIdx=-1, int endIdx=-1);
	void	setFontColor(const poColor& color, int startIdx=-1, int endIdx=-1);
	void	setFontTracking(float track, int startIdx=-1, int endIdx=-1);
	void	setFontScale(float scale, int startIdx=-1, int endIdx=-1);
	
	void	reshape( float w, float h );
	void	setPadding(float p);
	void	setPadding(float h, float v);
	void	setPadding(float l, float r, float t, float b);
	
	void	setAlign(poHoriOrientation hAlign, poVertOrientation vAlign);
	virtual void setOrigin(poHoriOrientation ho, poVertOrientation vo);
	
	void	makeStaticTexture( bool buildImage = false );
	void	updateStaticTexture();
	
	virtual bool pointInside(float x, float y);
	
	static poFont* getDefaultFont();
	poFont* getFont() { return ((!font)? getDefaultFont() : font); }
	
	void	setItalic( poFont* f ){ italicFont = f; };
	void	setBold( poFont* f ){ boldFont = f; };
	void	postFormat();
	bool	detectAndRemoveSequence( const char* sequence, int N );
	
	int		fontLeading;

	bool	enableFill;
	int		strokeWidth;
	poColor fillColor;
	poColor strokeColor;
	
	poFont* italicFont;
	poFont* boldFont;

	poTexture* staticTexture;
	poImage* staticImage;
	
//protected:	
	void	construct(const char* text, int width, int height);
	virtual void debugDraw();
	virtual void drawFrame();
	
	char*	cleanInput(char* txt);
	
	std::string	text;
	char*	cleanedText;
	
	float width, height;
	poBoundingBox padding;
	poBoundingBox actualTextBounds;
	
	poFont* font;
	poFontInfo fontInfo;
	
	
	
	poTBLetterVector letterList;
	
	poHoriOrientation hAlign;
	poVertOrientation vAlign;
	poHoriOrientation hOrigin;
	poVertOrientation vOrigin;
	
	poColor fontColor;
	float	fontTracking;
	float	fontScale;
};

typedef std::vector<poTextBox*> poTextBoxVector;
 
#endif

