//
//  poXML_test.cpp
//  potionCode
//
//  Created by filippo on 10/19/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include <boost/test/unit_test.hpp>
#include "poXML.h"

BOOST_AUTO_TEST_CASE( poXML_validity ) {
	// nodes always have to come from a document
	poXMLNode node;
	BOOST_CHECK(!node.isValid());
	
	// has a root node and is ready to write to
	poXMLDocument doc;
	BOOST_CHECK(doc.isValid());
	
	BOOST_CHECK(!doc.read("this file doesn't exist"));
	BOOST_CHECK(!doc.isValid());
	
	doc.resetRootNode();
	BOOST_CHECK(doc.isValid());
}

BOOST_AUTO_TEST_CASE( poXML_modify ) {
	// you always have to start with a document
	poXMLDocument doc;
	
	poXMLNode node = doc.getRootNode();
	node.setName("root");

	poXMLNode child1 = node.addChild("c1");
	child1.setAttribute("intAttrib", 1);
	BOOST_CHECK(child1.getIntAttribute("intAttrib") == 1);
	child1.setAttribute("intAttrib", 2);
	BOOST_CHECK(child1.getIntAttribute("intAttrib") == 2);
	
	poXMLNode child2 = node.addChild("child");
	BOOST_CHECK(child2.getName() == "child");
	child2.setName("c2");
	BOOST_CHECK(child2.getName() == "c2");
	
	// make a text node and add it as a child
	child2.setInnerFloat(10.5f);
	BOOST_CHECK_CLOSE(child2.getInnerFloat(), 10.5f, 0.00001f);
	// make a regular node and add it as a sibling of the previous text node
	// will look like <two>10.5 <twoChild /></two>
	poXMLNode child2A = child2.addChild("c2A");
	BOOST_CHECK(child2.getNumChildren() == 2);
	// make a text node and replace the contents of child2
	child2.setInnerString("hello");
	BOOST_CHECK(child2.getNumChildren() == 1);
	
	BOOST_CHECK(node.getNumChildren() == 2);
	
	BOOST_CHECK(!doc.getRootNode().getChild("doesn't exist").isValid());
}

BOOST_AUTO_TEST_CASE( poXML_find ) {
	const char *xml =
	"<library>"
	"	<book isbn='0030951593'>"
	"		<author>Mark Twain</author>"
	"		<title>The Adventures of Huckleberry Finn</title>"
	"	</book>"
	"	<book isbn='006106498X'>"
	"		<author>Mark Twain</author>"
	"		<title>The Adventures of Tom Sawyer</title>"
	"	</book>"
	"	<book isbn='0137030274'>"
	"		<author>Mark Twain</author>"
	"		<title>The Prince and the Pauper</title>"
	"	</book>"
	"</library>";
	
	poXMLDocument doc;
	BOOST_CHECK(doc.readStr(xml));
	
	poXMLNode root = doc.getRootNode();
	
	poXPathResult rez = root.find("//book");
	BOOST_CHECK(rez.getNumMatches() == 3);
	
	rez = root.find("//book[@isbn = 0137030274]");
	BOOST_CHECK(rez.getNumMatches() == 1);
	BOOST_CHECK(rez.getNode(0).getChild("author").getInnerString() == "Mark Twain");
	BOOST_CHECK(rez.getNode(0).getChild("title").getInnerString() == "The Prince and the Pauper");
	
	rez = root.find("//book[title = 'The Prince and the Pauper']");
	BOOST_CHECK(rez.getNumMatches() == 1);
	BOOST_CHECK(rez.getNode(0).getStringAttribute("isbn") == "0137030274");
}

BOOST_AUTO_TEST_CASE( poXML_readWrite ) {
	poXMLDocument doc;
	poXMLNode root = doc.getRootNode();
	root.addChild("one").setAttribute("intAttrib",10).setAttribute("stringAttrib","10");
	root.addChild("two").setAttribute("floatAttrib",30.f).addChild("twoA");
	doc.write("test.xml");
	BOOST_CHECK(fs::exists("test.xml"));
	
	poXMLDocument doc2("test.xml");
	BOOST_CHECK(doc2.getRootNode().getNumChildren() == 2);
	BOOST_CHECK(doc2.getRootNode().getFirstChild().getIntAttribute("intAttrib") == 10);
	
	fs::remove("test.xml");
}
