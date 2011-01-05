/*
 *  poXMLparser.h
 *  poXMLparser
 *
 *  Created by Jared Schiffman on 2/23/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __PO_XML_PARSER_H__
#define __PO_XML_PARSER_H__

class	poXMLattribute
{
public:
	poXMLattribute( const char* theName, const char* theValue );
	poXMLattribute(const poXMLattribute& rhs);
	poXMLattribute& operator=(const poXMLattribute& rhs);
	~poXMLattribute();
	
	void init(const char* n, const char* v);
	
	char*	name;
	char*	value;
};



class	poXMLnode
{
public:
	poXMLnode();
	poXMLnode( const char* name );
	poXMLnode( const char* name, const char* data );
	poXMLnode( const char* name, int data );
	poXMLnode( const char* name, float data );
	~poXMLnode();

	void			init();
	
	void			setName( const char* nodeName );
	void			addAttribute( const char* attName, const char* attValue );
	void			setData( const char* nodeData );
	void			setData( int nodeData );
	void			setData( float nodeData );
	poXMLnode*		addChild( poXMLnode* N );

	int				getNumNodes();
	int				getNumNodes( const char* nodeName );
	poXMLnode*		getNode( int N );
	poXMLnode*		getNode( const char* nodeName );
	poXMLnode*		getNode( const char* nodeName, int N );
	poXMLnode*		getNode( const char* attName, const char* attValue );

	int				getDataAsInt();
	float			getDataAsFloat();
	const char*		getDataAsString();
	
	int				getNodeAsInt( const char* nodeName );
	float			getNodeAsFloat( const char* nodeName );
	const char*		getNodeAsString( const char* nodeName );
	
	const char*		getAttribute( const char* attName );
	bool			hasAttributeEqual( const char* attName, const char* attValue );
		
	void			unparse( int tabDepth, FILE* outFile = stdout );
	

	char*			name;
	char*			data;
	
	int				numAttributes;
	poXMLattribute* attributeList;
	
	int				numChildren, numAllocChildren;
	poXMLnode**		childList;

};


class poXMLparser
{
public:

	poXMLparser();
	~poXMLparser() {}
	
	poXMLnode*	parseFile( const char* fileName );
	void		parseNameAndAttributes( poXMLnode* N, char* nodeName );
};

#endif

