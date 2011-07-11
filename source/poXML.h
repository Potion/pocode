//
//  File.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once 

#include <xpath_processor.h>

class poXMLNode;
class poXPathResult;
class poXMLDocument;

// can be a couple of different things
// either a single int, double or string
// or a set of ints, doubles, strings or nodes
class poXPathResult {
	friend class poXMLNode;

public:
	uint numMatches();
	int getInt(uint idx=0);
	double getDouble(uint idx=0);
	std::string getString(uint idx=0);
	poXMLNode getNode(uint idx=0);

private:
	poXPathResult(boost::shared_ptr<TiXmlDocument> doc,
				  const TinyXPath::expression_result &rez);
	
	TinyXPath::expression_result result;
	boost::shared_ptr<TiXmlDocument> doc;
};

class poXMLNode {
	friend class poXPathResult;
	friend class poXMLDocument;
	
public:
	poXMLNode();

	bool isValid() const;
	std::string name() const;
	
	// if this node contains a text node these will extract it
	std::string getText() const;
	// try to convert the value to a number (might not work)
	int getInt() const;
	double getReal() const;
	
	uint numAttributes() const;
	bool hasAttribute(const std::string &name) const;
	std::vector<std::string> attributeNames() const;
	std::string attribute(const std::string &name) const;
	int intAttribute(const std::string &name, int def=0) const;
	double realAttribute(const std::string &name, double def=0.0) const;
	
	uint numChildren() const;
	uint numChildren(const std::string &name) const;
	
	poXMLNode getChild(uint idx);
	poXMLNode getChild(const std::string &name);
	std::vector<poXMLNode> getChildren();
	std::vector<poXMLNode> getChildren(const std::string &name);

	// should be a valid xpath instruction
	// http://www.w3schools.com/xpath/default.asp for an xpath tutorial
	// http://tinyxpath.sourceforge.net/out.htm supported operations
	poXPathResult find(const std::string &xpath);
	
private:
	poXMLNode(boost::shared_ptr<TiXmlDocument> doc,
			  TiXmlNode *node);

	TiXmlNode *node;
	boost::shared_ptr<TiXmlDocument> doc;
};

class poXMLDocument {
public:
	poXMLDocument(const fs::path &p);
	poXMLNode rootNode();
	
private:
	boost::shared_ptr<TiXmlDocument> doc;
};
