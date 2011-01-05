#ifndef _PO_FONT_H_
#define _PO_FONT_H_

#include "poCommon.h"

class poObject;

struct poFontInfo
{
	char* fontName;
	int fontSize;
	int fontIndex;
	int charSet;
	
	poFontInfo() : fontName(NULL), fontSize(0), fontIndex(0), charSet(0) {}
	poFontInfo(const poFontInfo& rhs) : fontName(NULL), fontSize(0), fontIndex(0), charSet(0)
	{
		fontName = (char*)malloc(strlen(rhs.fontName)+1);
		strcpy(fontName, rhs.fontName);
		fontSize = rhs.fontSize;
		fontIndex = rhs.fontIndex;
		charSet = rhs.charSet;
	}
	poFontInfo& operator=(const poFontInfo& rhs)
	{
		fontName = (char*)malloc(strlen(rhs.fontName)+1);
		strcpy(fontName, rhs.fontName);
		fontSize = rhs.fontSize;
		fontIndex = rhs.fontIndex;
		charSet = rhs.charSet;
		return *this;
	}
};

class poKerningPair
{
public:
	poKerningPair( char first, char second, float kernAmount )
	{
		A = first;
		B = second;
		kern = kernAmount;
	}
	char A, B;
	float kern;
};

class poFont
{
public:
	virtual void drawChar(char c, float x, float y, float scale=1.0) = 0;
	virtual void drawString(const char* str, float x, float y, float scale=1.0) = 0;
	
	const char* getName() {return info.fontName;}
	int getSize() {return info.fontSize;}
	int getFaceIndex() {return info.fontIndex;}
	int getCharset() {return info.charSet;}
	
	virtual int getCharWidth(char c) = 0;
	virtual int getSpacing(char left, char right)
	{
		for( int i=0; i<kerningPairList.size(); i++ )
		{
			if ( kerningPairList[i].A == left && kerningPairList[i].B == right )
				return kerningPairList[i].kern;
		}
		return 0;
	};
	virtual int getLeading() = 0;
	virtual int getAscender() = 0;
	virtual int getDescender() = 0;
	
	virtual void construct() = 0;
	virtual void layoutFont(float x, float y) = 0;
	
	virtual void selectCharMap(int mapIdx) {}
	virtual void selectFace(int faceIdx) {}
	virtual void addAFMfile(const char* afmfile) {}
	virtual void remapChar(int from, int to){}
	virtual poObject* getCharMapObj() = 0;
	
	void addKerningPair( char first, char second, float kernAmount ) 
	{ 
		kerningPairList.push_back(poKerningPair(first,second,kernAmount)); 
	};
	
	poFontInfo info;
	
	std::vector<poKerningPair>	kerningPairList;
};

#endif