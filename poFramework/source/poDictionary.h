/*
 *  poDictionary.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/14/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poCommon.h"

#ifndef __poDictionary_h__
#define __poDictionary_h__

enum poDictionaryDataType { PO_INT, PO_FLOAT, PO_BOOL, PO_CHAR, PO_STRING, PO_OBJECT, PO_VOIDPTR, PO_COLOR, PO_POINT };

class poObject;

union poDictionaryData
{
	int			an_int;
	float		a_float;
	bool		a_bool;
	char		a_char;
	char*		a_string;
	poObject*	a_poObject;
	float		a_color[4];
	float		a_point[3];
	void*		a_voidPtr;
};

class poDictionaryItem {
public:

	poDictionaryItem(const std::string& n, poDictionaryDataType	t, poDictionaryData d);
	poDictionaryItem(const poDictionaryItem& rhs);
	virtual ~poDictionaryItem();

	std::string				name;
	poDictionaryDataType	type;
	poDictionaryData		data;
};


typedef std::vector<poDictionaryItem> poDictionaryItemVector;

class poDictionary {
public:
	void add(const std::string& name, poDictionaryDataType type, poDictionaryData data);
	
	poDictionary& add(const std::string& name, bool data)		{ poDictionaryData D; D.a_bool=data;  add(name, PO_BOOL, D); return *this; };
	poDictionary& add(const std::string& name, int data)		{ poDictionaryData D; D.an_int=data;  add(name, PO_INT, D);  return *this; };
	poDictionary& add(const std::string& name, float data)		{ poDictionaryData D; D.a_float=data; add(name, PO_FLOAT, D); return *this; };
	poDictionary& add(const std::string& name, char data)		{ poDictionaryData D; D.a_char=data;  add(name, PO_CHAR, D); return *this; };
	poDictionary& add(const std::string& name, poObject* data)	{ poDictionaryData D; D.a_poObject=data; add(name, PO_OBJECT, D); return *this; };
	poDictionary& add(const std::string& name, void* data)		{ poDictionaryData D; D.a_voidPtr=data; add(name, PO_VOIDPTR, D); return *this; };
	poDictionary& add(const std::string& name, char* data);
	poDictionary& add(const std::string& name, poPoint data);
	poDictionary& add(const std::string& name, poColor data);
	
	bool		contains(const std::string& name);

	bool		getBool( const std::string& name ); 
	int			getInt( const std::string& name );
	float		getFloat( const std::string& name );
	char		getChar( const std::string& name );
	poObject*	getPoObject( const std::string& name );
	poObject*	getPoObject( const std::string& name, const std::string& type );
	void*		getVoidPtr( const std::string& name );
	poColor		getColor( const std::string& name );
	poPoint		getPoint( const std::string& name );
	const char*	getString( const std::string& name );
	
private:
	poDictionaryItem* findByName( const std::string& name, poDictionaryDataType checkType );
	poDictionaryItemVector items;
};


#endif

