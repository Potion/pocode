/*
 *  poDictionary.cpp
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/14/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poDictionary.h"

poDictionaryItem::poDictionaryItem(const std::string& n, poDictionaryDataType t, poDictionaryData d)
{
	name = n;
	type = t;
	data = d;
}

poDictionaryItem::~poDictionaryItem() 
{
	if(type == PO_STRING)
		free(data.a_string);
}

poDictionaryItem::poDictionaryItem(const poDictionaryItem& rhs)
{
	name = rhs.name;
	type = rhs.type;

	if(type == PO_STRING) {
		data.a_string = (char*)malloc(strlen(rhs.data.a_string)+1);
		strcpy(data.a_string, rhs.data.a_string);
	}
	else
		data = rhs.data;
}

void poDictionary::add(const std::string& name, poDictionaryDataType type, poDictionaryData data)
{
	poDictionaryItem item( name, type, data );
	items.push_back( item );
}


poDictionary& poDictionary::add(const std::string& name, poColor data)
{
	poDictionaryData D; 
	
	D.a_color[0] = data.R; 
	D.a_color[1] = data.G;
	D.a_color[2] = data.B;
	D.a_color[3] = data.A;
	
	add(name, PO_COLOR, D);
	return *this;
}

poDictionary& poDictionary::add(const std::string& name, poPoint data) 
{
	poDictionaryData D; 
	
	D.a_point[0] = data.x; 
	D.a_point[1] = data.y;
	D.a_point[2] = data.z;
			
	add(name, PO_POINT, D);
	return *this;
}

poDictionary& poDictionary::add(const std::string& name, char* str)
{
	char* tmp = (char*)malloc(strlen(str)+1);
	strcpy(tmp, str);
	poDictionaryData D;
	D.a_string = tmp;
	add(name, PO_STRING, D);
	return *this;
}

bool poDictionary::contains(const std::string& name) 
{
	for( int i=0; i<items.size(); i++ )
		if ( items[i].name == name )
			return true;
	return false;
}

poDictionaryItem* poDictionary::findByName( const std::string& name, poDictionaryDataType checkType )
{
	for( int i=0; i<items.size(); i++ )
	{
		if ( items[i].name == name )
		{
			if ( items[i].type != checkType )
			{
				printf("WARNING: poDictionary->findByName(): Type mismatch on item %s. Type is actually %d\n", name.c_str(), items[i].type );
				return NULL;
			}
			return &items[i];
		}
	}
	
	printf("WARNING: poDictionary->findByName(): No dictionary item named %s\n", name.c_str() );
	return NULL;
}


bool	poDictionary::getBool( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_BOOL );
	if ( item==NULL )
		return false;
	return item->data.a_bool;
}

int			poDictionary::getInt( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_INT );
	if ( item==NULL )
		return 0;
	return item->data.an_int;
}

float		poDictionary::getFloat( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_FLOAT );
	if ( item==NULL )
		return 0.0;
	return item->data.a_float;
}

char		poDictionary::getChar( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_CHAR );
	if ( item==NULL )
		return 0;
	return item->data.a_char;
}

poObject*	poDictionary::getPoObject( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_OBJECT );
	if ( item==NULL )
	{
		printf("returning NULL\n");
		return NULL;
	}
	return item->data.a_poObject;
}

poObject*	poDictionary::getPoObject( const std::string& name, const std::string& type )
{
	poDictionaryItem* item = findByName( name, PO_OBJECT );
	if ( item==NULL )
	{
		return NULL;
	}
	/*if ( strcmp( item->data.a_poObject->getClassName(), type.c_str() ) != 0 )
	{
		printf( "poDictionary::getPoObject: Type mismatch of poObject.\n");
		return NULL;
	}*/
	return item->data.a_poObject;
}

void*		poDictionary::getVoidPtr( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_VOIDPTR );
	if ( item==NULL )
		return NULL;
	return item->data.a_voidPtr;
}

poColor		poDictionary::getColor( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_COLOR );
	if ( item==NULL )
		return poColor();
	
	poColor C( item->data.a_color[0], item->data.a_color[1], item->data.a_color[2], item->data.a_color[3] );
	return C;
}

poPoint		poDictionary::getPoint( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_POINT );
	if ( item==NULL )
		return poPoint();
		
	poPoint P( item->data.a_point[0], item->data.a_point[1], item->data.a_point[2] );
	return P;
}

const char*	poDictionary::getString( const std::string& name )
{
	poDictionaryItem* item = findByName( name, PO_STRING );
	if ( item==NULL )
		return NULL;
	
	return item->data.a_string;
}



