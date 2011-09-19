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

	poXMLDocument doc("test.xml");
	poXMLNode root = doc.rootNode();

	poXPathResult rez = root.find("//container");
	for(int i=0; i<rez.numMatches(); i++) {
		poXMLNode node = rez.getNode(i);
		printf("%s: %d children", node.name().c_str(), node.numChildren());

		std::vector<std::string> attribs = node.attributeNames();
		for(int i=0; i<attribs.size(); i++) {
			printf(" (%s,%s)", attribs[i].c_str(), node.stringAttribute(attribs[i]).c_str());
		}
		printf("\n");
		
		node = node.getChild("obj");
		while(node) {
			printf("\tnode: %s\n", node.name().c_str());
			node = node.nextSibling();
		}
	}
	
	
	printf("\n\n");
	for(int i=0; i<root.numChildren(); i++) {
		poXMLNode node = root.getChild(i);
		printf("%s: %d children\n", node.name().c_str(), node.numChildren());

		for(int j=0; j<node.numChildren(); j++) {
			poXMLNode child = node.getChild(j);
			printf("\tnode: %s\n", child.name().c_str());
		}
	}
	
	poXMLNode node = root.addChild("container");
	node.addAttribute("attrib1", 50);
	node.addAttribute("attrib2", 4.2f);
	node.addAttribute("attrib3", "blank");
	node.setInnerFloat(100.45f);
	
	doc.write("test2.xml");
	
	applicationQuit();
}


