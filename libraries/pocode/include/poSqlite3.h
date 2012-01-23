#pragma once


/*	Created by Stephen Varga on 9/15/11.
 *	Copyright 2011 Potion. All rights reserved.
 */

#include "poDictionary.h"

#include <string>
#include <vector>
#include <sqlite3.h>

#define PO_COL_TYPE_STRING  SQLITE_TEXT
#define PO_COL_TYPE_INT     SQLITE_INTEGER
#define PO_COL_TYPE_FLOAT   SQLITE_FLOAT

//------------------------------------------------------------
//poSqlite3Column
typedef struct {
    std::string name;
    int type;
} poSqlite3Column;

//------------------------------------------------------------
//poSqlite3Result
class poSqlite3Result {
public:
    poSqlite3Result(std::string query, bool bVerbose=false);
    ~poSqlite3Result();
    
    std::string query;
    
    std::vector <poSqlite3Column> columns;
    std::string getColumnNames();
    
    int getNumRows();
    poDictionary getRow(int rowNum);
    
    std::vector <poDictionary> rows;

    std::string errorMessage;
private:
    bool bVerbose;
    int nRows;
};

class poSqlite3 {
public:
    poSqlite3(bool bVerbose=false);
	poSqlite3(std::string url, bool bVerbose=false);
	virtual ~poSqlite3();
    
    void setVerbose(bool isVerbose);
    bool openDatabase(std::string url, bool bOverwrite = false);
    
    poSqlite3Result query(std::string query);
    
    std::string escapeQuotes(std::string text);
    
    void save();
    void save(const char* url);
    
    void close();
    
    sqlite3 *db;
private:
    bool bLoaded;
    bool bVerbose;
};

