#include "Sqlite3ExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"

poObject *createObjectForID(uint uid) {
	return new Sqlite3ExampleApp();
}

void setupApplication() {
    //Set Local Path
    lookUpAndSetPath("resources");
    
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Sqlite3Example", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

Sqlite3ExampleApp::Sqlite3ExampleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    //Create new poSqlite
    db = new poSqlite3(true);
    
    //Load database (creates DB if not found)
    db->loadFile("test.sqlite");
    
    //Create table if necessary
    db->query("CREATE TABLE table1 (id INTEGER PRIMARY KEY, one varchar(10), two smallint);");
    db->query("INSERT INTO table1 VALUES(NULL, 'hello!',  10);");
    db->query("INSERT INTO table1 VALUES(NULL, 'goodbye', 20);");
    
    int test = 55;
    db->query("INSERT INTO table1 VALUES(NULL, 'testing'," + poToString(test) + ");");
    
    //Get Results
    poSqlite3Result results = db->query("SELECT * FROM table1");
    
    //Show Columns
    std::cout << results.getColumnNames() << std::endl;
    
    //Loop through rows
    for(int i=0; i<results.rows.size(); i++) {
        poDictionary *thisRow = results.getRow(i);
        
        std::string rowString;
        
        rowString += poToString(thisRow->getInt("id")) + " | ";
        rowString += poToString(thisRow->getString("one")) + " | ";
        rowString += poToString(thisRow->getInt("two"));
        
        std::cout << rowString << std::endl;
    }
    
    
}

Sqlite3ExampleApp::~Sqlite3ExampleApp() {
    db->close();
}

void Sqlite3ExampleApp::eventHandler(poEvent *event) {
	
}

void Sqlite3ExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
