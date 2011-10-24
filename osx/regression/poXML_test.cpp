//
//  poXML_test.cpp
//  potionCode
//
//  Created by filippo on 10/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poXML.h"

using namespace pugi;

std::vector<std::string> numeric_names(int start, int end, std::string suffix="");
std::vector<std::string> numeric_names(int start, int end, std::string suffix)
{
	std::vector<std::string> names;
	for(int i=start; i <= end; i++)
	{
		std::string str = (boost::format("%d")%i).str();
		if(!suffix.empty()) str = suffix + str;
		names.push_back(str);
	}
	return names;
}

std::vector<std::string> alphabetic_names(int start, int end, std::string suffix="");
std::vector<std::string> alphabetic_names(int start, int end, std::string suffix)
{
	std::vector<std::string> names;
	for(int i=0; i < (end - start)+1; i++)
	{
		char c[16];
		sprintf(c, "%c", start+i);
//		printf("%c",c);
		std::string str = (boost::format("%c")%c).str();
		if(!suffix.empty()) str = suffix + str;
		names.push_back(str);
	}
	return names;
}

void nestChildNode(poXMLNode parent, std::vector<std::string> names, int index);
void nestChildNode(poXMLNode parent, std::vector<std::string> names, int index)
{
	if(index < names.size())
	{
		poXMLNode child = parent.addChild(names[index]);
		index++;
		nestChildNode(child, names, index);
	}
}

BOOST_AUTO_TEST_CASE( poXMLTest ) {
	
	poXMLDocument xmlDoc = poXMLDocument();
	poXMLNode rootNode = xmlDoc.getRootNode();
	
	poXMLDocument invalidXmlDoc;
	BOOST_CHECK(invalidXmlDoc.isValid() == false);
	
	poXMLNode invalidNode;
	pugi::xml_node invalid_node = invalidNode.handle();
	BOOST_CHECK((bool)invalid_node == false);
	BOOST_CHECK(invalidNode.isValid() == false);
	
	poXMLNode noChildNode = rootNode.addChild("noChild");
	BOOST_CHECK(noChildNode.numChildren() == 0);
	
	poXMLNode withChildNode = rootNode.addChild("withChild");
	poXMLNode childNode = withChildNode.addChild("a child");
	BOOST_CHECK(withChildNode.numChildren() == 1);
	BOOST_CHECK(childNode.getName() == "a child");
	BOOST_CHECK(withChildNode.getChild(0).getName() == childNode.getName());
	
	withChildNode.setInnerString("trying to add content to a node with a child");
	BOOST_CHECK(withChildNode.getInnerString().empty() == true);
	
	poXMLNode withChildAndContentNode = rootNode.addChild("withChildAndContent");
	withChildAndContentNode.setInnerString("adding content first and then a child");
	BOOST_CHECK(withChildAndContentNode.getInnerString() == "adding content first and then a child");
	
	poXMLNode anotherChildNode = withChildAndContentNode.addChild("another child");
	BOOST_CHECK(withChildAndContentNode.getInnerString() == "adding content first and then a child");
	BOOST_CHECK(anotherChildNode.getName() == "another child");
	BOOST_CHECK(withChildAndContentNode.numChildren() == 1);
	BOOST_CHECK(withChildAndContentNode.getChild(0).getName() == anotherChildNode.getName());
		
	poXMLNode numbersNode = rootNode.addChild("numbers");
	nestChildNode(numbersNode, numeric_names(0, 10, "number"), 0);
	BOOST_CHECK(numbersNode.getChildren().size() == 1);
	
	for(int i=1; i < 10; i++)
	{
		char c[16];
		sprintf(c, "%d", i);
		numbersNode.addChild(c);
		BOOST_CHECK(numbersNode.getChild(c).getName() == numbersNode.getChild(i).getName());
		BOOST_CHECK(numbersNode.getChild(c).handle() == numbersNode.getChild(i).handle());
	}
	BOOST_CHECK(numbersNode.getChildren().size() == 10);
	
	poXMLNode lettersNode = rootNode.addChild("letters");
	nestChildNode(lettersNode, alphabetic_names('A', 'Z'), 0);
	
	poXMLNode INode = rootNode.find("//letters/A/B/C/D/E/F/G/H/I").getNode(0);
	BOOST_CHECK(INode.getName() == "I");
	
	INode.setName("1");
	BOOST_CHECK(INode.getName() == "1");
	
	INode.addAttribute("letter", "no");
	BOOST_CHECK(INode.getStringAttribute("letter") == "no");
	
	INode.addAttribute("number", 123);
	BOOST_CHECK(INode.getIntAttribute("number") == 123);
	BOOST_CHECK(INode.getStringAttribute("number") == "123");
	
	INode.setAttribute("number", 123.999f);
	BOOST_CHECK(INode.getIntAttribute("number") == 123);
	BOOST_CHECK(INode.getFloatAttribute("number") == 123.999);
	
	INode.addChild("J sibling");
	INode.setInnerFloat(123.999f);
	
	poXMLNode JNode = INode.getChild(0);
	BOOST_CHECK(JNode.getName() == "J");
	
	poXMLNode JsiblingNode = JNode.getNextSibling();
	BOOST_CHECK(JsiblingNode.getName() == "J sibling");
	
	poXMLNode sevenNode = numbersNode.getChild("7");
	sevenNode.addAttribute("value", 7);
	sevenNode.setInnerString("seven");
	sevenNode.setInnerInt(7);
	sevenNode.setInnerFloat(7.f);
	
	
//	xmlDoc.write("test.xml");
//	xmlDoc.print();
}
