#include "Dictionary_TestApp.h"

#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"

#include <sstream>
#include <fstream>
#include <boost/assign.hpp>

poObject *createObjectForID(uint uid) {
	return new Dictionary_TestApp();
}

void setupApplication() {
	fs::path path;
	pathToFolder("Dictionary Test", &path);
	setCurrentPath(path);
	
	if(!fs::exists("dictionary.xml")) {
		poDictionary dict;
		dict.setBool("bool", false);
		dict.setInt("int", 10);
		dict.setString("string", "Hello World");
		dict.setColor("color", poColor::red);
		dict.setPoint("point", poPoint(25,-8.5,100));
		dict.setFloat("float", M_PI);
		dict.write("dictionary.xml");
	}

	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "Dictionary_Test", 100, 100, 1024, 768);
}

void cleanupApplication() {
}

Dictionary_TestApp::Dictionary_TestApp() {
	addModifier(new poCamera2D());
	
	poTextBox *tb = addChild(new poTextBox(700));
	tb->position = getWindowCenter();
	tb->textAlignment(PO_ALIGN_LEFT);
	tb->alignment(PO_ALIGN_CENTER_CENTER);
	tb->drawBounds = true;
	tb->tabWidth(14);
	
	dict.read("dictionary.xml");
	reload();
	
	addEvent(PO_KEY_DOWN_EVENT, this);
}

void Dictionary_TestApp::eventHandler(poEvent *event) {
	if(event->type == PO_KEY_DOWN_EVENT) {
		if(event->keyChar == ' ') {
			dict.read("dictionary.xml");
			reload();
		}
		else 
		if(event->keyChar == '\t') {
			using namespace std;
			
			ifstream file;
			file.open("dictionary.xml");
			
			stringstream ss;
			ss << file.rdbuf();
			
			cout << ss.str();
			
			dict.set(ss.str());

			reload();
		}
	}
}

void Dictionary_TestApp::reload() {
	std::list<std::string> keys = boost::assign::list_of("bool")("int")("string")("color")("point")("float");
	std::stringstream ss;

	BOOST_FOREACH(std::string &key, keys) {
		ss << key << "\t" << dict.getProperty(key) << "\n";
	}
	
	poTextBox *tb = getChildAs<poTextBox>(0);
	tb->text(ss.str());
	tb->layout();
}

