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

#include "poSqlite.h"

#include <iostream>

namespace po {
    // -----------------------------------------------------------------------------------
    // ================================ Class: Sqlite Result =============================
    #pragma mark Class: Sqlite Result

    SqliteResult::SqliteResult(std::string query, bool bVerbose) {
        //Record the query for reuse, can be good to have
        this->query     = query;
        this->bVerbose  = bVerbose;
    }


    SqliteResult::~SqliteResult() {
    }


    //------------------------------------------------------------------
    int SqliteResult::getNumRows() {
        return rows.size();
    }


    //------------------------------------------------------------------
    //Get a poDictionary for a row
    poDictionary SqliteResult::getRow(int rowNum) {
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
    std::string SqliteResult::getColumnNames() {
        std::string columnNames;
        
        for(uint i=0; i <columns.size(); i++) {
            columnNames += columns[i].name;
            if(i != columns.size() - 1) columnNames += " | ";
        }
        
        return columnNames;
    }


    // -----------------------------------------------------------------------------------
    // ================================ Class: Sqlite ====================================
    #pragma mark Class: Sqlite

    Sqlite::Sqlite()
    : bVerbose(false) {
    }


    Sqlite::Sqlite(std::string url, bool bVerbose)
    : bVerbose(bVerbose) {
        openDatabase(url);
    }


    Sqlite::~Sqlite() {
        close();
    }


    //------------------------------------------------------------------
    bool Sqlite::openDatabase(std::string url, bool bOverwrite) {
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
            
            SqliteResult dropCommands = query("SELECT name FROM sqlite_master WHERE type = 'table';)");
            
            for(int i=0; i<dropCommands.getNumRows(); i++) {
                query("DROP TABLE " + dropCommands.rows[i].getString("name"));
            }
        }
        
        
        bLoaded = true;
        return true;
    }


    //------------------------------------------------------------------
    void Sqlite::setVerbose(bool isVerbose) {
        this->bVerbose = isVerbose;
    }


    //------------------------------------------------------------------
    void Sqlite::save() {
        //save(fileLoc);
    }


    //------------------------------------------------------------------
    void Sqlite::save(const char* url) {
        //sqlite3 * toFile;
    }


    //------------------------------------------------------------------
    void Sqlite::close() {
        if(bLoaded) {
            sqlite3_close(db);
            bLoaded = false;
        }
    }


    //------------------------------------------------------------------
    void Sqlite::beginTransaction() {
        sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    }


    //------------------------------------------------------------------
    void Sqlite::endTransaction() {
        sqlite3_exec(db, "END TRANSACTION", NULL, NULL, NULL);
    }


    //------------------------------------------------------------------
    //This statement runs any SQLite3 Query and returns a SQLite3 result object
    //This object is how you get any kind of results
    SqliteResult Sqlite::query(std::string query) {
        SqliteResult results(query, bVerbose);
        
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
                            results.columns.push_back(SqliteColumn());
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
    std::string Sqlite::escapeQuotes(std::string text) {
        
        int position = text.find( "'" );
        
        while ( position != std::string::npos ) {
            text.replace( position, 1, "''" );
            position = text.find( "'", position + 2 );
        } 
        
        return text;
    }
}