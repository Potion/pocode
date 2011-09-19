#include "poSqlite3.h"

poSqlite3::poSqlite3(bool bVerbose) {
    this->bVerbose = bVerbose;
}

poSqlite3::~poSqlite3() {
    close();
}


bool poSqlite3::loadFile(std::string url) {
    int error = sqlite3_open(url.c_str(), &db);
    
    if(error) {
        if(bVerbose) std::cout << sqlite3_errmsg(db) << std::endl;
        return false; 
    }
    
    bLoaded = true;
    return true;
}

void poSqlite3::setVerbose(bool isVerbose) {
    this->bVerbose = isVerbose;
}


void poSqlite3::save() {
    //save(fileLoc);
}


void poSqlite3::save(const char* url) {
    sqlite3 * toFile;
}


void poSqlite3::close() {
    if(bLoaded) {
        sqlite3_close(db);
        bLoaded = false;
    }
}


poSqlite3Result poSqlite3::query(std::string query) {
    poSqlite3Result results;
    
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
                            dict.setInt(colName, sqlite3_column_int(statement, col));
                            break;
                        case SQLITE_FLOAT:
                            dict.setFloat(colName, (float)sqlite3_column_double(statement, col));
                            break;
                        case SQLITE_TEXT:
                            dict.setString(colName, (char*)sqlite3_column_text(statement, col));
                            break;
                    }
                }
                
                results.rows.push_back(dict);
                
                //Don't need to keep checking column names
                bColumnsSet = true;
            } else {
                //If no rows returned, we're done
                break;
            }
        }
        
        sqlite3_finalize(statement);
    } else {
        //If statement is wack, tell why
        results.errorMessage = "Problem with query '" + (std::string)query + "'\n " + (std::string)sqlite3_errmsg(db);
        if(bVerbose) std::cout << results.errorMessage << std::endl;
    }
    
    //Return the dictionary object with the query results in it
    return results;
}






//-------------------------------------------------------
//Result Class
poSqlite3Result::poSqlite3Result() {
    
}

poSqlite3Result::~poSqlite3Result() {
    
}

poDictionary * poSqlite3Result::getRow(int rowNum) {
    return &rows[rowNum];
}

std::string poSqlite3Result::getColumnNames() {
    std::string columnNames;
    
    for(int i=0; i <columns.size(); i++) {
        columnNames += columns[i].name;
        if(i != columns.size() - 1) columnNames += " | ";
    }
    
    return columnNames;
}

