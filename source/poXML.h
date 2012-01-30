/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  File.h
//  pocode
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once 

#include "common.h"

#include <string>
#include <vector>
#include <pugixml.hpp>
#include <boost/shared_ptr.hpp>

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
	// load from a file
	poXMLDocument(const std::string &url);
	
	bool			isValid() const;
    
    // GET ROOT NODE FROM XML DOCUMENT
	poXMLNode       getRootNode() const;
	poXMLNode		resetRootNode();
	
    // READ and WRITE XML FILE
	bool            read(const std::string &url);
	bool            readStr(const std::string &str);
	bool            write(const std::string &url);
	
    // PRINT
	void            print() const;
	pugi::xml_document &getHandle() const;
	
	poXMLDocument	copy();
	
private:
	boost::shared_ptr<pugi::xml_document> document;
};

// CLASS NOTES
//
// poXMLNode is the essential unit of the XML system.
//
// A poXMLnode may have a unique name and may an inner value in one of the three variable types.
// A poXMLnode may also children, as well as XML attributes.
//

class poXMLNode {
	friend class poXMLDocument;
	
public:
	poXMLNode();
	poXMLNode(pugi::xml_node node);

	bool			isValid() const;
	
    // XML NODE NAME
	std::string     getName() const;
	poXMLNode&		setName(const std::string &str);

    // GET XML VALUE
	int             getInnerInt() const;
	float           getInnerFloat() const;
	std::string     getInnerString() const;

    // SET XML VALUE
	poXMLNode&		setInnerInt(int i);
	poXMLNode&		setInnerFloat(float f);
	poXMLNode&		setInnerString(const std::string &str);
	
    // XML ATTRIBUTES
	uint            getNumAttributes() const;
	bool            hasAttribute(const std::string &name) const;
	std::vector<std::string> getAttributeNames() const;

    // GET ATTRIBUTE VALUE
	int             getIntAttribute(const std::string &name) const;
	float           getFloatAttribute(const std::string &name) const;
	std::string     getStringAttribute(const std::string &name) const;

    // SET ATTRIBUTE VALUE
	poXMLNode&		setAttribute(const std::string &name, int value);
	poXMLNode&		setAttribute(const std::string &name, float value);
	poXMLNode&		setAttribute(const std::string &name, const std::string &value);
	poXMLNode&		removeAttribute(const std::string &name);
    
    // ADD and REMOVE CHILDREN
	uint			getNumChildren() const;
	poXMLNode		addChild(const std::string &name);
	poXMLNode&		removeChild(const std::string &name);	
    
    // GET CHILDREN
	poXMLNode       getChild(uint idx);
	poXMLNode       getChild(const std::string &name);
	std::vector<poXMLNode> getChildren();
	std::vector<poXMLNode> getChildren(const std::string &name);

    // GET CHILDREN IN ORDER
	poXMLNode       getFirstChild();
	poXMLNode		getLastChild();
	poXMLNode       getNextSibling();
	poXMLNode       getNextSibling(const std::string &name);
	
	// should be a valid xpath instruction
	// http://www.w3schools.com/xpath/default.asp for an xpath tutorial
	// http://tinyxpath.sourceforge.net/out.htm supported operations
	poXPathResult find(const std::string &xpath);
	
	pugi::xml_node getHandle() const;
	
private:
	pugi::xml_node  node;
	poXMLDocument doc;
};

bool operator==(poXMLNode const& n1, poXMLNode const &n2);

// CLASS NOTES
//
// poXPathResult is a special class for finding data in an XML document.
// Use the find() method in poXMLNode to search using poXPathResults.
//

// can be a couple of different things
// either a single int, float or string
// or a set of ints, floats, strings or nodes
class poXPathResult {
	friend class poXMLNode;
    
public:
	uint            getNumMatches();
	
    // GET RESULT NODES
	poXMLNode       getNode(uint idx=0);
    
    // GET RESULT VALUES
	std::string     getString(uint idx=0);
	int             getInt(uint idx=0);
	float           getFloat(uint idx=0);
    
private:
	poXPathResult(pugi::xpath_node_set nodes);
	pugi::xpath_node_set nodes;
};
