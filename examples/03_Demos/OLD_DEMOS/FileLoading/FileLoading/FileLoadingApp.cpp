#include "FileLoadingApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "Helpers.h"
#include "poFileLoader.h"

poObject *createObjectForID(uint uid) {
	return new FileLoadingApp();
}

void setupApplication() {
    //Set Local Path
    lookUpAndSetPath("resources");
    
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Sqlite3Example", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

FileLoadingApp::FileLoadingApp() {
	addModifier(new poCamera2D(poColor::black));    
    
    poFileLoader loader;
    //std::cout << loader.getFileAsString("http://www.vargatron.com") << std::endl;
    
    loader.getFile("http://www.vargatron.com/index.html");
}

FileLoadingApp::~FileLoadingApp() {
}

void FileLoadingApp::eventHandler(poEvent *event) {
	
}

void FileLoadingApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
