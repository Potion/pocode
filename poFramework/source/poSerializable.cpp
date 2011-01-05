/*
 *  poSerializable.cpp
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/28/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poSerializable.h"


poSerializable::poSerializable()
{
	serialize = false;
}

	
poXMLnode*		poSerializable::write()
{
	poXMLnode* node = registry.write( "PO_OBJECT" );
	node->addAttribute("type", getClassName() );
	
	return node;
}

void			poSerializable::read(poXMLnode* node)
{
	registry.read(node);
}