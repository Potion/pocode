/*
 *  poObjectModifier.cpp
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/28/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poObjectModifier.h"


poObjectModifierVector	poObjectModifier::registeredModifierSet;

void	poObjectModifier::init()
{
}

poXMLnode* poObjectModifier::write() 
{
	poXMLnode* node = registry.write( "PO_OBJECT_MODIFIER" );
	node->addAttribute("type", getClassName() );
	
	return node;
}

void poObjectModifier::read(poXMLnode* node)
{	
	registry.read(node);
}

poObjectModifier*	poObjectModifier::generateModifier( char* className )
{
	for( int i=0; i<registeredModifierSet.size(); i++ )
		if ( strcmp( registeredModifierSet[i]->getClassName(), className ) == 0 )
			return registeredModifierSet[i]->duplicate();
			
	return NULL;
}
