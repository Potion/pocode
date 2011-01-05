/*
 *  poSerializable.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 3/28/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */
 
 #ifndef PO_SERIALIZABLE
 #define PO_SERIALIZABLE

#include "poXMLparser.h"
#include "poVariableRegistry.h"

class poSerializable
{
public:
	poSerializable();

	virtual void			init() { };
	
	virtual const char*		getClassName() { return "poSerializable"; };
	
	virtual poXMLnode*		write();
	virtual void			read(poXMLnode* node);
	
	poVariableRegistry		registry;
	bool					serialize;
};

#endif