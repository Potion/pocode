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


// CLASS NOTES
//
// poXMLDocument is used for reading and writing XML files.
//
// After reading in the XML, get the root node, using the rootNode method.
//

class poXMLDocument {
public:
	poXMLDocument();
	poXMLDocument(const fs::path &p);
    
    // GET ROOT NODE FROM XML DOCUMENT
	poXMLNode       rootNode();
	
    operator        bool() const;
    
    // READ and WRITE XML FILE
	bool            read(const fs::path &p);
	bool            readStr(const std::string &str);
	bool            write(const fs::path &p);
	
    // PRINT
	void            print();
	
private:
	boost::shared_ptr< pugi::xml_document > doc;
};


// CLASS NOTES
//
// poXMLNode is the essential unit of the XML system.
//
// A poXMLnode may have a unique name and may an inner value in one of the three variable types.
// A poXMLnode may also children, as well as XML attributes.
//

class poXMLNode {
	friend class poXPathResult;
	friend class poXMLDocument;
	
public:
	poXMLNode();
	
	operator        bool() const;	

    // XML NODE NAME
	std::string     name() const;
	void            setName(const std::string &str);

    // GET XML VALUE
	int             innerInt() const;
	float           innerFloat() const;
	std::string     innerString() const;

    // SET XML VALUE
	void            setInnerInt(int i);
	void            setInnerFloat(float f);
	void            setInnerString(const std::string &str);
	
    // XML ATTRIBUTES
	uint            numAttributes() const;
	bool            hasAttribute(const std::string &name) const;
	std::vector<std::string> attributeNames() const;

    // GET ATTRIBUTE VALUE
	int             intAttribute(const std::string &name) const;
	float           floatAttribute(const std::string &name) const;
	std::string     stringAttribute(const std::string &name) const;

    // SET ATTRIBUTE VALUE
	void            setAttribute(const std::string &name, int value);
	void            setAttribute(const std::string &name, float value);
	void            setAttribute(const std::string &name, const std::string &value);
	
    // ADD and REMOVE ATTRIBUTES
	void            addAttribute(const std::string &name, int i);
	void            addAttribute(const std::string &name, float f);
	void            addAttribute(const std::string &name, const std::string &value);
	void            removeAttribute(const std::string &name);
    
    // ADD and REMOVE CHILDREN
	uint            numChildren() const;
	poXMLNode       addChild(const std::string &name);
	void            removeChild(const std::string &name);	
    
    // GET CHILDREN
	poXMLNode       getChild(uint idx);
	poXMLNode       getChild(const std::string &name);
	std::vector<poXMLNode> getChildren();
	std::vector<poXMLNode> getChildren(const std::string &name);

    // GET CHILDREN IN ORDER
	poXMLNode       firstChild();
	poXMLNode       nextSibling();
	poXMLNode       nextSibling(const std::string &name);
	
	// should be a valid xpath instruction
	// http://www.w3schools.com/xpath/default.asp for an xpath tutorial
	// http://tinyxpath.sourceforge.net/out.htm supported operations
	poXPathResult find(const std::string &xpath);
	
	pugi::xml_node handle();
	
private:
	poXMLNode(boost::shared_ptr<pugi::xml_document> doc, pugi::xml_node node);

	pugi::xml_node  node;
	boost::shared_ptr<pugi::xml_document> doc;
};


// CLASS NOTES
//
// poXPathResult is a special class for finding data in an XML document.
// Use the find() method in poXMLNode to search using poXPathResults.
//

// can be a couple of different things
// either a single int, double or string
// or a set of ints, doubles, strings or nodes
class poXPathResult {
	friend class poXMLNode;
    
public:
	uint            numMatches();
	
    // GET RESULT NODES
	poXMLNode       getNode(uint idx=0);
    
    // GET RESULT VALUES
	std::string     getString(uint idx=0);
	int             getInt(uint idx=0);
	float           getFloat(uint idx=0);
    
private:
	poXPathResult(boost::shared_ptr<pugi::xml_document> doc, pugi::xpath_node_set nodes);
    
	boost::shared_ptr<pugi::xml_document> doc;
	pugi::xpath_node_set nodes;
};
