#include "XMLApp.h"
#include "poApplication.h"
#include "poCamera.h"

#include <fstream>

#include "Helpers.h"
#include "poXML.h"
#include "poTextBox.h"

poObject *createObjectForID(uint uid) {
	return new XMLApp();
}

void setupApplication() {
	fs::path path;
	pathToFolder("XML", &path);
	setCurrentPath(path);

	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "XML", 100, 100, 700, 700);
}

void cleanupApplication() {
}

XMLApp::XMLApp() {
	addModifier(new poCamera2D(poColor::black));

	poXMLNode node = poXMLDocument("test.xml").rootNode();
	
	poXPathResult result = node.find("/root/container[@attrib1=1]/@attrib1");
	assert(result.numMatches() == 1);
	assert(result.getInt() == 1);
	
	result = node.find("//container");
	assert(result.numMatches() == 2);
	
	node = node.find("//container[1]").getNode();
	assert(node.intAttribute("attrib1") == 1);
	assert(node.realAttribute("attrib2") == 5.4);
	assert(node.attribute("attrib3") == "blank");
	assert(node.name() == "container");
	
	result = node.find("./obj");
	assert(result.numMatches() == 4);
	for(int i=0; i<4; i++) {
		printf("%s\n", result.getNode(i).getText().c_str());
	}
	
	node = node.find("//container[2]").getNode();
	result = node.find("./obj");
	for(int i=0; i<4; i++) {
		printf("%d\n", result.getNode(i).getInt());
	}
	
	std::stringstream ss;
	std::ifstream input("test.xml");
	ss << input.rdbuf();
	input.close();
	
	poTextBox *tb = new poTextBox(getWindowWidth(), getWindowHeight());
	tb->text(ss.str()).layout();
	addChild(tb);
}

XMLApp::~XMLApp() {
}

void XMLApp::eventHandler(poEvent *event) {
	
}

void XMLApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
