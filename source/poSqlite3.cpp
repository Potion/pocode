/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "poSqlite3.h"

#include <iostream>

#pragma mark - poSqlite3 -

poSqlite3::poSqlite3()
: bVerbose(false) {
}


poSqlite3::poSqlite3(std::string url, bool bVerbose)
: bVerbose(bVerbose) {
    openDatabase(url);
}


poSqlite3::~poSqlite3() {
    close();
}


//------------------------------------------------------------------
bool poSqlite3::openDatabase(std::string url, bool bOverwrite) {
    int error = sqlite3_open(url.c_str(), &db);
    
    if(error) {
        if(bVerbose) std::cout << sqlite3_errmsg(db) << std::endl;
        bLoaded = false;
        return false; 
    } else {
        if(bVerbose) std::cout << "Successfully opened database " << url << std::endl;
    }
    
    
    
    //If We're overwriting this whole db, just drop all tables
    if(bOverwrite) {
        if(bVerbose) std::cout << "Overwriting previous DB" << std::endl;
        
        poSqlite3Result dropCommands = query("SELECT name FROM sqlite_master WHERE type = 'table';)");
        
        for(int i=0; i<dropCommands.getNumRows(); i++) {
            query("DROP TABLE " + dropCommands.rows[i].getString("name"));
        }
    }
    
    
    bLoaded = true;
    return true;
}


//------------------------------------------------------------------
void poSqlite3::setVerbose(bool isVerbose) {
    this->bVerbose = isVerbose;
}


//------------------------------------------------------------------
void poSqlite3::save() {
    //save(fileLoc);
}


//------------------------------------------------------------------
void poSqlite3::save(const char* url) {
    //sqlite3 * toFile;
}


//------------------------------------------------------------------
void poSqlite3::close() {
    if(bLoaded) {
        sqlite3_close(db);
        bLoaded = false;
    }
}


//------------------------------------------------------------------
void poSqlite3::beginTransaction() {
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
}


//------------------------------------------------------------------
void poSqlite3::endTransaction() {
    sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL);
}


//------------------------------------------------------------------
//This statement runs any SQLite3 Query and returns a SQLite3 result object
//This object is how you get any kind of results
poSqlite3Result poSqlite3::query(std::string query) {
    poSqlite3Result results(query, bVerbose);
    
    sqlite3_stmt *statement;
    
    //Check the statement, make sure it is legit
    int status = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0);
    if(status == SQLITE_OK) {
        //Get the number of columns returned
        int nCols = sqlite3_column_count(statement);
        
        //Step through result
        int result  = 0;
        bool bColumnsSet = false;
        
        //Loop through Rows until there are no more
        while(true) {
            result = sqlite3_step(statement);
            
            //If a Row is returned, add to dict
            if(result == SQLITE_ROW) {
                poDictionary dict;
                
                //Step through columns
                for(int col = 0; col < nCols; col++) {
                    std::string colName = (char*)sqlite3_column_name(statement, col);
                    int colType = sqlite3_column_type(statement, col);
                    
                    //Get column names first time through
                    if(!bColumnsSet) {
                        results.columns.push_back(poSqlite3Column());
                        results.columns.back().name = colName;
                        results.columns.back().type = colType;
                    } 
                    
                    switch(colType) {
                        case SQLITE_INTEGER:
                            dict.set(colName, (int)sqlite3_column_int(statement, col));
                            break;
                        case SQLITE_FLOAT:
                            dict.set(colName, (float)sqlite3_column_double(statement, col));
                            break;
                        case SQLITE_TEXT:
                            dict.set(colName, std::string((char*)sqlite3_column_text(statement, col)));
                            break;
                    }
                }
                
                results.rows.push_back(dict);
                
                //Don't need to keep checking column names
                bColumnsSet = true;
            }
			else {
                //If no rows returned, we're done
                break;
            }
        }
        
        sqlite3_finalize(statement);
    }
	else {
        //If statement is wack, tell why
        results.errorMessage = "Problem with query '" + (std::string)query + "'\n " + (std::string)sqlite3_errmsg(db);
        if(bVerbose) std::cout << results.errorMessage << std::endl;
    }
    
    //Return the dictionary object with the query results in it
    return results;
}


//------------------------------------------------------------------
std::string poSqlite3::escapeQuotes(std::string text) {
	
	int position = text.find( "'" );
	
	while ( position != std::string::npos ) {
		text.replace( position, 1, "''" );
		position = text.find( "'", position + 2 );
	} 
	
	return text;
}





//------------------------------------------------------------------
//------------------------------------------------------------------
//SQLite3 Result Object
#pragma mark - poSqlite3Result -

poSqlite3Result::poSqlite3Result(std::string query, bool bVerbose) {
    //Record the query for reuse, can be good to have
    this->query     = query;
    this->bVerbose  = bVerbose;
}


poSqlite3Result::~poSqlite3Result() {
    
}


//------------------------------------------------------------------
int poSqlite3Result::getNumRows() {
    return rows.size();
}


//------------------------------------------------------------------
//Get a poDictionary for a row
poDictionary poSqlite3Result::getRow(int rowNum) {
    if((uint)rowNum < rows.size()) {
        return rows[rowNum];
    }
	else {
        if(bVerbose) std::cout << "Row " << rowNum << " is out of range, only " << rows.size() << " rows in result";
        return rows[rows.size()-1];
    }
}


//------------------------------------------------------------------
//Get column names prints out the column names to the console
//Should probably print their type as well!
std::string poSqlite3Result::getColumnNames() {
    std::string columnNames;
    
    for(uint i=0; i <columns.size(); i++) {
        columnNames += columns[i].name;
        if(i != columns.size() - 1) columnNames += " | ";
    }
    
    return columnNames;
}