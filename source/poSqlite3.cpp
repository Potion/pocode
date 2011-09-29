#include "poSqlite3.h"

#pragma mark - poSqlite3 -

poSqlite3::poSqlite3(bool bVerbose) {
    this->bVerbose = bVerbose;
}


poSqlite3::poSqlite3(std::string url, bool bVerbose) {
    this->bVerbose = bVerbose;
    
    openDatabase(url);
}


poSqlite3::~poSqlite3() {
    close();
}


bool poSqlite3::openDatabase(std::string url, bool bOverwrite) {
    int error = sqlite3_open(url.c_str(), &db);
    
    if(error) {
        if(bVerbose) std::cout << sqlite3_errmsg(db) << std::endl;
        return false; 
    }
    
    //If We're overwriting this whole db, just drop all tables
    if(bOverwrite) {
        poSqlite3Result dropCommands = query("SELECT name FROM sqlite_master WHERE type = 'table';)");
        
        for(int i=0; i<dropCommands.getNumRows(); i++) {
            query("DROP TABLE " + dropCommands.rows[i].getString("name"));
        }
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


std::string poSqlite3::escapeQuotes(std::string text){
	
	int position = text.find( "'" );
	
	while ( position != std::string::npos ) 
	{
		text.replace( position, 1, "''" );
		position = text.find( "'", position + 2 );
	} 
	
	return text;
}





#pragma mark - poSqlite3Result -

poSqlite3Result::poSqlite3Result(std::string query, bool bVerbose) {
    //Record the query for reuse, can be good to have
    this->query     = query;
    this->bVerbose  = bVerbose;
}

poSqlite3Result::~poSqlite3Result() {
    
}


int poSqlite3Result::getNumRows() { return rows.size(); }


//Get a poDictionary for a row
poDictionary poSqlite3Result::getRow(int rowNum) {
    if(rowNum < rows.size()) {
        return rows[rowNum];
    } else {
        if(bVerbose) std::cout << "Row " << rowNum << " is out of range, only " << rows.size() << " rows in result";
        return rows[rows.size()-1];
    }
}

//Get column names prints out the column names to the console
//Should probably print their type as well!
std::string poSqlite3Result::getColumnNames() {
    std::string columnNames;
    
    for(int i=0; i <columns.size(); i++) {
        columnNames += columns[i].name;
        if(i != columns.size() - 1) columnNames += " | ";
    }
    
    return columnNames;
}

