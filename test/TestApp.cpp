#include "TestApp.h"

#include "poGUI.h"
#include "Helpers.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poApplication.h"
#include "poShapeBasics2D.h"
#include "poMaskModifier.h"
#include "poXML.h"

using namespace std;
using namespace boost;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	fs::path path;
	pathToFolder("xcode", &path);
	setCurrentPath(path/"test/resources");
	
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 650, 500, 700, 700);
}

void cleanupApplication() {
}

void print(poXMLNode node, int level=0) {
	if(!node.isValid()) {
		printf("not valid");
		return;
	}
	
	printf("%*s%s (kids:%d attribs:%d)\n", level*2, "", node.name().c_str(), node.numChildren(), node.numAttributes());
	for(int i=0; i<node.numChildren(); i++) {
		poXMLNode n = node.getChild(i);
		print(n,level+1);
	}
}

TestObj::TestObj() {
	addModifier(new poCamera2D());
	
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
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}

void TestObj::messageHandler(const std::string &msg, const poDictionary &dict) {
	
}



