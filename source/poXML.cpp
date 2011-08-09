//
//  File.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poXML.h"
#include "Helpers.h"

using namespace pugi;

#define LCAST(type,val) boost::lexical_cast<type>(val)

#pragma mark - poXPathResult -
uint poXPathResult::numMatches() {
	return nodes.size();
}

poXMLNode poXPathResult::getNode(uint idx) {
	return poXMLNode(doc, nodes[idx].node());
}

std::string poXPathResult::getString(uint idx) {
	return nodes[idx].attribute().value();
}

int poXPathResult::getInt(uint idx) {
	return nodes[idx].attribute().as_int();
}

float poXPathResult::getFloat(uint idx) {
	return nodes[idx].attribute().as_float();
}

poXPathResult::poXPathResult(boost::shared_ptr<pugi::xml_document> doc, xpath_node_set nodes) 
:	doc(doc)
,	nodes(nodes) 
{}

#pragma mark - poXMLNode -
poXMLNode::poXMLNode() : node() {}

poXMLNode::operator bool() const {return (bool)node;}

std::string poXMLNode::name() const {return node.name();}
void poXMLNode::setName(const std::string &str) {node.set_name(str.c_str());}

void poXMLNode::setInnerString(const std::string &str) {
	if(!node.first_child())
		node.append_child(node_pcdata);
	
	node.first_child().set_value(str.c_str());
}

void poXMLNode::setInnerInt(int i) {
	if(!node.first_child())
		node.append_child(node_pcdata);
	
	node.first_child().set_value(LCAST(std::string,i).c_str());
}

void poXMLNode::setInnerFloat(float f) {
	if(!node.first_child())
		node.append_child(node_pcdata);
	
	node.first_child().set_value(LCAST(std::string,f).c_str());
}

int poXMLNode::innerInt() const {return boost::lexical_cast<int>(node.child_value());}
float poXMLNode::innerFloat() const {return boost::lexical_cast<float>(node.child_value());}
std::string poXMLNode::innerString() const {return node.child_value();}

uint poXMLNode::numAttributes() const {return std::distance(node.attributes_end(), node.attributes_begin());}
bool poXMLNode::hasAttribute(const std::string &name) const {return node.attribute(name.c_str());}

std::vector<std::string> poXMLNode::attributeNames() const {
	std::vector<std::string> response;

	for(xml_attribute_iterator i=node.attributes_begin();
		i != node.attributes_end();
		++i)
	{
		response.push_back(i->name());
	}
	
	return response;
}

int poXMLNode::intAttribute(const std::string &name) const {return node.attribute(name.c_str()).as_int();}
float poXMLNode::floatAttribute(const std::string &name) const {return node.attribute(name.c_str()).as_float();}
std::string poXMLNode::stringAttribute(const std::string &name) const {return node.attribute(name.c_str()).value();}

void poXMLNode::setAttribute(const std::string &name, int value) {node.attribute(name.c_str()) = value;}
void poXMLNode::setAttribute(const std::string &name, float value) {node.attribute(name.c_str()) = value;}
void poXMLNode::setAttribute(const std::string &name, const std::string &value) {node.attribute(name.c_str()) = value.c_str();}

void poXMLNode::addAttribute(const std::string &name, int value) {node.append_attribute(name.c_str()) = value;}
void poXMLNode::addAttribute(const std::string &name, float value) {node.append_attribute(name.c_str()) = value;}
void poXMLNode::addAttribute(const std::string &name, const std::string &value) {node.append_attribute(name.c_str()) = value.c_str();}

uint poXMLNode::numChildren() const {
	int size = 0;
	xml_node::iterator n = node.begin();
	while(n != node.end()) {
		size++;
		++n;
	};
	return size;
}

poXMLNode poXMLNode::addChild(const std::string &name) {return poXMLNode(doc,node.append_child(name.c_str()));}

poXMLNode poXMLNode::getChild(uint idx) {
	std::vector<poXMLNode> kids = getChildren();
	return kids[idx];
}


poXMLNode poXMLNode::getChild(const std::string &name) {return poXMLNode(doc,node.child(name.c_str()));}

std::vector<poXMLNode> poXMLNode::getChildren() {
	std::vector<poXMLNode> response;
	xml_node n = node.first_child();
	while(n) {
		response.push_back(poXMLNode(doc,n));
		n = n.next_sibling();
	}
	return response;
}

std::vector<poXMLNode> poXMLNode::getChildren(const std::string &name) {
	std::vector<poXMLNode> response;
	xml_node n = node.child(name.c_str());
	while(n) {
		response.push_back(poXMLNode(doc,n));
		n = n.next_sibling(name.c_str());
	}
	return response;
}

poXMLNode poXMLNode::firstChild() {return poXMLNode(doc,node.first_child());}

poXMLNode poXMLNode::nextSibling() {return poXMLNode(doc,node.next_sibling());}
poXMLNode poXMLNode::nextSibling(const std::string &name) {return poXMLNode(doc,node.next_sibling(name.c_str()));}

poXPathResult poXMLNode::find(const std::string &xpath) {
	return poXPathResult(doc, node.select_nodes(xpath.c_str()));
}

pugi::xml_node poXMLNode::handle() {return node;}

poXMLNode::poXMLNode(boost::shared_ptr<pugi::xml_document> doc, xml_node node)
:	doc(doc)
,	node(node) 
{}


#pragma mark - poXMLDocument -

poXMLDocument::poXMLDocument()
{}

poXMLDocument::poXMLDocument(const fs::path &p) {
	read(p);
}

poXMLNode poXMLDocument::rootNode() {
	return poXMLNode(doc, doc->first_child());
}

bool poXMLDocument::read(const fs::path &p) {
	doc = boost::shared_ptr<xml_document>(new xml_document());
	xml_parse_result result = doc->load_file(p.c_str(), parse_default, encoding_utf8);
	if(!result) {
		log("poXML: parse error (file: %s) (error: %s)", p.c_str(), result.description());
		doc.reset();
		return false;
	}
	return true;
}

bool poXMLDocument::readStr(const std::string &str) {
	doc = boost::shared_ptr<xml_document>(new xml_document());
	xml_parse_result result = doc->load(str.c_str());
	if(!result) {
		log("poXML: parse error", result.description());
		doc.reset();
		return false;
	}
	return true;
}

bool poXMLDocument::write(const fs::path &p) {
	doc->save_file(p.c_str());
}

poXMLDocument::operator bool() const {return doc;}

void poXMLDocument::dump() {
	doc->print(std::cout);
}




