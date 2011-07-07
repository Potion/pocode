//
//  File.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poXML.h"

using namespace TinyXPath;

#pragma mark - poXPathResult -
uint poXPathResult::numMatches() {
	switch(result.e_type) {
		case e_bool:
		case e_string:
		case e_int:
		case e_double:
			return 1;
			
		case e_node_set:
			return result.nsp_get_node_set()->u_get_nb_node_in_set();
			
		case e_invalid:
			return 0;
	}
	return 0;
}

int poXPathResult::getInt(uint idx) {
	if(result.e_type == e_node_set)
		return result.nsp_get_node_set()->i_get_value(idx);

	if(result.e_type != e_int)
		return 0;
	
	return result.i_get_int();
}

double poXPathResult::getDouble(uint idx) {
	if(result.e_type == e_node_set)
		return result.nsp_get_node_set()->d_get_value(idx);
	
	if(result.e_type != e_double)
		return 0;
	
	return result.d_get_double();
}

std::string poXPathResult::getString(uint idx) {
	if(result.e_type == e_node_set)
		return result.nsp_get_node_set()->S_get_value(idx);
	
	if(result.e_type != e_string)
		return 0;
	
	return result.S_get_string();
}

poXMLNode poXPathResult::getNode(uint idx) {
	const TiXmlNode *node =result.nsp_get_node_set()->XNp_get_node_in_set(idx);
	return poXMLNode(doc, const_cast<TiXmlNode*>(node));
}

poXPathResult::poXPathResult(boost::shared_ptr<TiXmlDocument> doc,
							 const expression_result &rez) 
:	doc(doc)
,	result(rez) 
{}


#pragma mark - poXMLNode -
poXMLNode::poXMLNode() : node(NULL) {}

bool poXMLNode::isValid() const {return node != NULL;}

std::string poXMLNode::name() const {return node->ValueStr();}

std::string poXMLNode::getText() const {
	TiXmlText *text = node->FirstChild()->ToText();
	if(text)
		return text->ValueStr();
	return "";
}

int poXMLNode::getInt() const {
	std::string txt = getText();
	try {
		return boost::lexical_cast<int>(txt);
	}
	catch(boost::bad_lexical_cast c) {}
	
	return 0;
}

double poXMLNode::getReal() const {
	std::string txt = getText();
	try {
		return boost::lexical_cast<double>(txt);
	}
	catch(boost::bad_lexical_cast c) {}
	
	return 0.0;	
}

uint poXMLNode::numAttributes() const {return attributeNames().size();}

bool poXMLNode::hasAttribute(const std::string &name) const {
	TiXmlElement *ele = node->ToElement();
	if(ele)
		return ele->Attribute(name.c_str()) != NULL;
	
	return false;
}

std::vector<std::string> poXMLNode::attributeNames() const {
	std::vector<std::string> response;
	TiXmlElement *ele = node->ToElement();
	if(ele) {
		TiXmlAttribute *attrib = ele->FirstAttribute();
		while(attrib) {
			response.push_back(attrib->Name());
			attrib = attrib->Next();
		}
	}
	return response;
}

std::string poXMLNode::attribute(const std::string &name) const {
	TiXmlElement *ele = node->ToElement();
	if(ele) {
		const char *val = ele->Attribute(name.c_str());
		if(val)
			return val;
	}
	return "";
}

int poXMLNode::intAttribute(const std::string &name, int def) const {
	TiXmlElement *ele = node->ToElement();
	if(ele) {
		int i;
		const char *val = ele->Attribute(name.c_str(), &i);
		if(val)
			return i;
	}
	return def;
}

double poXMLNode::realAttribute(const std::string &name, double def) const {
	TiXmlElement *ele = node->ToElement();
	if(ele) {
		double d;
		const char *val = ele->Attribute(name.c_str(), &d);
		if(val)
			return d;
	}
	return def;
}

uint poXMLNode::numChildren() const {
	std::vector<poXMLNode> nodes = const_cast<poXMLNode*>(this)->getChildren();
	return nodes.size();
}

uint poXMLNode::numChildren(const std::string &name) const {
	std::vector<poXMLNode> nodes = const_cast<poXMLNode*>(this)->getChildren(name);
	return nodes.size();
}

poXMLNode poXMLNode::getChild(uint idx) {
	std::vector<poXMLNode> nodes = getChildren();
	return nodes[idx];
}

poXMLNode poXMLNode::getChild(const std::string &name) {
	return poXMLNode(doc, node->FirstChild(name));
}

std::vector<poXMLNode> poXMLNode::getChildren() {
	std::vector<poXMLNode> response;
	TiXmlNode *n = node->FirstChild();
	while(n) {
		response.push_back(poXMLNode(doc,n));
		n = n->NextSibling();
	}
	return response;
}

std::vector<poXMLNode> poXMLNode::getChildren(const std::string &name) {
	std::vector<poXMLNode> response;
	TiXmlNode *n = node->FirstChild(name);
	while(n) {
		response.push_back(poXMLNode(doc,n));
		n = n->NextSibling(name);
	}
	return response;
}

poXPathResult poXMLNode::find(const std::string &xpath) {
	xpath_processor proc(node, xpath.c_str());
	return poXPathResult(doc, proc.er_compute_xpath());
}

poXMLNode::poXMLNode(boost::shared_ptr<TiXmlDocument> doc,
					 TiXmlNode *node)
:	doc(doc)
,	node(node) 
{}


#pragma mark - poXMLDocument -
poXMLDocument::poXMLDocument(const fs::path &p) {
	doc = boost::shared_ptr<TiXmlDocument>(new TiXmlDocument(p.c_str()));
	doc->LoadFile(TIXML_ENCODING_UTF8);
}

poXMLNode poXMLDocument::rootNode() {
	return poXMLNode(doc, doc->RootElement());
}
