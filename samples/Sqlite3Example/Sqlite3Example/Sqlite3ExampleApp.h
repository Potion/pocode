/*	Created by Stephen Varga on 9/16/11.
 *	Copyright 2011 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"

#include "poSqlite3.h"

class Sqlite3ExampleApp : public poObject {
public:
	Sqlite3ExampleApp();
	virtual ~Sqlite3ExampleApp();
	
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poSqlite3 *db;
};