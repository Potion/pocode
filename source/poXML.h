//
//  File.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once 

#include <pugixml.hpp>

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
	
	poXMLNode getNode(uint idx=0);

	std::string getString(uint idx=0);
	int getInt(uint idx=0);
	float getFloat(uint idx=0);

private:
	poXPathResult(boost::shared_ptr<pugi::xml_document> doc, pugi::xpath_node_set nodes);

	boost::shared_ptr<pugi::xml_document> doc;
	pugi::xpath_node_set nodes;
};

class poXMLNode {
	friend class poXPathResult;
	friend class poXMLDocument;
	
public:
	poXMLNode();
	
	operator bool() const;	

	std::string name() const;
	void		setName(const std::string &str);

	int			innerInt() const;
	float		innerFloat() const;
	std::string innerString() const;

	void		setInnerInt(int i);
	void		setInnerFloat(float f);
	void		setInnerString(const std::string &str);
	
	uint		numAttributes() const;
	bool		hasAttribute(const std::string &name) const;
	std::vector<std::string> attributeNames() const;

	int			intAttribute(const std::string &name) const;
	float		floatAttribute(const std::string &name) const;
	std::string stringAttribute(const std::string &name) const;

	void		setAttribute(const std::string &name, int value);
	void		setAttribute(const std::string &name, float value);
	void		setAttribute(const std::string &name, const std::string &value);
	
	void		addAttribute(const std::string &name, int i);
	void		addAttribute(const std::string &name, float f);
	void		addAttribute(const std::string &name, const std::string &value);
	
	uint		numChildren() const;
	
	poXMLNode	addChild(const std::string &name);
	
	poXMLNode	getChild(uint idx);
	poXMLNode	getChild(const std::string &name);

	std::vector<poXMLNode> getChildren();
	std::vector<poXMLNode> getChildren(const std::string &name);

	poXMLNode	nextSibling();
	poXMLNode	nextSibling(const std::string &name);
	
	// should be a valid xpath instruction
	// http://www.w3schools.com/xpath/default.asp for an xpath tutorial
	// http://tinyxpath.sourceforge.net/out.htm supported operations
	poXPathResult find(const std::string &xpath);
	
	pugi::xml_node handle();
	
private:
	poXMLNode(boost::shared_ptr<pugi::xml_document> doc, pugi::xml_node node);

	pugi::xml_node node;
	boost::shared_ptr<pugi::xml_document> doc;
};

class poXMLDocument {
public:
	poXMLDocument();
	poXMLDocument(const fs::path &p);
	poXMLNode rootNode();
	
	bool read(const fs::path &p);
	bool readStr(const std::string &str);
	bool write(const fs::path &p);
	bool isValid();
	
	void dump();
	
private:
	boost::shared_ptr< pugi::xml_document > doc;
};
