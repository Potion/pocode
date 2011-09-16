#pragma once

/*	Created by Stephen Varga on 9/15/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include <sqlite3.h>

#include "poDictionary.h"


#define PO_COL_TYPE_STRING  SQLITE_TEXT
#define PO_COL_TYPE_INT     SQLITE_INTEGER
#define PO_COL_TYPE_FLOAT   SQLITE_FLOAT

typedef struct {
    std::string name;
    int type;
} poSqlite3Column;


class poSqlite3Result {
public:
    poSqlite3Result();
    ~poSqlite3Result();
    
    std::string query;
    
    int nRows;
    poDictionary * getRow(int rowNum);
    std::vector <poDictionary> rows;
    
    std::vector <poSqlite3Column> columns;
    std::string getColumnNames();

    std::string errorMessage;
};



class poSqlite3 {
public:
	poSqlite3(bool bVerbose=false);
	virtual ~poSqlite3();
    
    sqlite3 *db;
    
    bool loadFile(std::string url);
    
    poSqlite3Result query(std::string query);
     
    void setVerbose(bool isVerbose);
    
    void save();
    void save(const char* url);
    
    void close();
private:
    bool bLoaded;
    bool bVerbose;
};