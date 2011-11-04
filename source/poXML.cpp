//
//  File.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poXML.h"
#include "poHelpers.h"

using namespace pugi;

#define LCAST(type,val) boost::lexical_cast<type>(val)

#pragma mark - poXPathResult -
poXPathResult::poXPathResult(xpath_node_set nodes) 
:	nodes(nodes) 
{}

uint poXPathResult::numMatches() {
	return nodes.size();
}

poXMLNode poXPathResult::getNode(uint idx) {
	return poXMLNode(nodes[idx].node());
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

#pragma mark - poXMLNode -
poXMLNode::poXMLNode() : node() {}

poXMLNode::poXMLNode(xml_node node)
:	node(node) 
{}

bool poXMLNode::isValid() const {
	return node;
}

std::string poXMLNode::getName() const {
	return node.name();
}

void poXMLNode::setName(const std::string &str) {
	node.set_name(str.c_str());
}

int poXMLNode::getInnerInt() const {
	return LCAST(int, node.child_value());
}

float poXMLNode::getInnerFloat() const {
	return LCAST(float, node.child_value());
}

std::string poXMLNode::getInnerString() const {
	return node.child_value();
}

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

uint poXMLNode::getNumAttributes() const {
	return std::distance(node.attributes_end(), node.attributes_begin());
}

bool poXMLNode::hasAttribute(const std::string &name) const {
	return node.attribute(name.c_str());
}

std::vector<std::string> poXMLNode::getAttributeNames() const {
	std::vector<std::string> response;

	for(xml_attribute_iterator i=node.attributes_begin(); i != node.attributes_end(); ++i) {
		response.push_back(i->name());
	}
	
	return response;
}

int poXMLNode::getIntAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).as_int();
}

float poXMLNode::getFloatAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).as_float();
}

std::string poXMLNode::getStringAttribute(const std::string &name) const {
	return node.attribute(name.c_str()).value();
}

void poXMLNode::setAttribute(const std::string &name, int value) {
	node.attribute(name.c_str()) = value;
}

void poXMLNode::setAttribute(const std::string &name, float value) {
	node.attribute(name.c_str()) = value;
}

void poXMLNode::setAttribute(const std::string &name, const std::string &value) {
	node.attribute(name.c_str()) = value.c_str();
}

void poXMLNode::addAttribute(const std::string &name, int value) {
	node.append_attribute(name.c_str()) = value;
}

void poXMLNode::addAttribute(const std::string &name, float value) {
	node.append_attribute(name.c_str()) = value;
}

void poXMLNode::addAttribute(const std::string &name, const std::string &value) {
	node.append_attribute(name.c_str()) = value.c_str();
}

uint poXMLNode::getNumChildren() const {
	int size = 0;
	xml_node::iterator n = node.begin();
	while(n != node.end()) {
		size++;
		++n;
	};
	return size;
}

poXMLNode poXMLNode::addChild(const std::string &name) {
	return poXMLNode(node.append_child(name.c_str()));
}

void poXMLNode::removeChild(const std::string &name) {
	node.remove_child(name.c_str());
}

void poXMLNode::removeAttribute(const std::string &name) {
	node.remove_attribute(name.c_str());
}

poXMLNode poXMLNode::getChild(uint idx) {
	if(idx > getNumChildren())
		return poXMLNode();
	
	std::vector<poXMLNode> kids = getChildren();
	return kids[idx];
}


poXMLNode poXMLNode::getChild(const std::string &name) {
	return poXMLNode(node.child(name.c_str()));
}

std::vector<poXMLNode> poXMLNode::getChildren() {
	std::vector<poXMLNode> response;
	xml_node n = node.first_child();
	while(n) {
		response.push_back(poXMLNode(n));
		n = n.next_sibling();
	}
	return response;
}

std::vector<poXMLNode> poXMLNode::getChildren(const std::string &name) {
	std::vector<poXMLNode> response;
	xml_node n = node.child(name.c_str());
	while(n) {
		response.push_back(poXMLNode(n));
		n = n.next_sibling(name.c_str());
	}
	return response;
}

poXMLNode poXMLNode::getFirstChild() {
	return poXMLNode(node.first_child());
}

poXMLNode poXMLNode::getLastChild() {
	return poXMLNode(node.last_child());
}

poXMLNode poXMLNode::getNextSibling() {
	return poXMLNode(node.next_sibling());
}

poXMLNode poXMLNode::getNextSibling(const std::string &name) {
	return poXMLNode(node.next_sibling(name.c_str()));
}

poXPathResult poXMLNode::find(const std::string &xpath) {
	return poXPathResult(node.select_nodes(xpath.c_str()));
}

pugi::xml_node poXMLNode::handle() {
	return node;
}


#pragma mark - poXMLDocument -

poXMLDocument::poXMLDocument() {
	document.reset(new pugi::xml_document);
	document->append_child("root");
}

poXMLDocument::poXMLDocument(const std::string &url) {
	document.reset(new pugi::xml_document);
	read(url);
}

bool poXMLDocument::isValid() const {
	return document->empty();
}

poXMLNode poXMLDocument::getRootNode() const {
	return poXMLNode(document->first_child());
}

void poXMLDocument::setRootNode(poXMLNode node) {
	document.reset(new pugi::xml_document);
	node.node = document->append_copy(node.handle());
}

bool poXMLDocument::read(const std::string &url) {
	xml_parse_result result = document->load_file(url.c_str(), parse_default, encoding_utf8);
	if(!result) {
		log("poXML: parse error (file: %s) (error: %s)", url.c_str(), result.description());
		document.reset();
		return false;
	}
	return true;
}

bool poXMLDocument::readStr(const std::string &str) {
	xml_parse_result result = document->load(str.c_str());
	if(!result) {
		log("poXML: parse error", result.description());
		document.reset();
		return false;
	}
	return true;
}

bool poXMLDocument::write(const std::string &url) {
	document->save_file(url.c_str());
}

void poXMLDocument::print() {
	document->print(std::cout);
}




