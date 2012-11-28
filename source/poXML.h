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

#include <string>
#include <vector>
#include <pugixml.hpp>
#include <boost/shared_ptr.hpp>

namespace po {
    class XMLNode;
    class XpathResult;
    class XMLDocument;

    // CLASS NOTES
    //
    // XMLDocument is used for reading and writing XML files.
    //
    // After reading in the XML, get the root node, using the rootNode method.
    //

    class XMLDocument {
    public:
        XMLDocument();
        // load from a file
        XMLDocument(const std::string &url);
        
        bool			isValid() const;
        
        // GET ROOT NODE FROM XML DOCUMENT
        XMLNode     getRootNode() const;
        XMLNode		resetRootNode();
        
        // READ and WRITE XML FILE
        bool            read(const std::string &url);
        bool            readStr(const std::string &str);
        bool            write(const std::string &url);
        
        // PRINT
        void            print() const;
        pugi::xml_document &getHandle() const;
        
        XMLDocument	copy();
        
    private:
        boost::shared_ptr<pugi::xml_document> document;
    };

    // CLASS NOTES
    //
    // XMLNode is the essential unit of the XML system.
    //
    // A XMLnode may have a unique name and may an inner value in one of the three variable types.
    // A XMLnode may also children, as well as XML attributes.
    //

    class XMLNode {
        friend class XMLDocument;
        
    public:
        XMLNode();
        XMLNode(pugi::xml_node node);

        bool			isValid() const;
        
        // XML NODE NAME
        std::string     getName() const;
        XMLNode&		setName(const std::string &str);

        // GET XML VALUE
        int             getInnerInt() const;
        float           getInnerFloat() const;
        std::string     getInnerString() const;

        // SET XML VALUE
        XMLNode&		setInnerInt(int i);
        XMLNode&		setInnerFloat(float f);
        XMLNode&		setInnerString(const std::string &str);
        
        // XML ATTRIBUTES
        uint            getNumAttributes() const;
        bool            hasAttribute(const std::string &name) const;
        std::vector<std::string> getAttributeNames() const;

        // GET ATTRIBUTE VALUE
        int             getIntAttribute(const std::string &name) const;
        float           getFloatAttribute(const std::string &name) const;
        std::string     getStringAttribute(const std::string &name) const;

        // SET ATTRIBUTE VALUE
        XMLNode&		setAttribute(const std::string &name, int value);
        XMLNode&		setAttribute(const std::string &name, float value);
        XMLNode&		setAttribute(const std::string &name, const std::string &value);
        XMLNode&		removeAttribute(const std::string &name);
        
        // ADD and REMOVE CHILDREN
        uint			getNumChildren() const;
        XMLNode         addChild(const std::string &name);
        XMLNode&		addChild(XMLNode const &node);	
        XMLNode&		removeChild(const std::string &name);
        XMLNode&		removeChild(XMLNode const &node);	
        
        // GET CHILDREN
        XMLNode         getChild(uint idx);
        XMLNode         getChild(const std::string &name);
        std::vector<XMLNode> getChildren();
        std::vector<XMLNode> getChildren(const std::string &name);

        // GET CHILDREN IN ORDER
        XMLNode         getFirstChild();
        XMLNode         getLastChild();
        XMLNode         getNextSibling();
        XMLNode         getNextSibling(const std::string &name);
        
        // should be a valid xpath instruction
        // http://www.w3schools.com/xpath/default.asp for an xpath tutorial
        // http://tinyxpath.sourceforge.net/out.htm supported operations
        XpathResult find(const std::string &xpath);
        
        pugi::xml_node getHandle() const;
        
    private:
        pugi::xml_node  node;
        XMLDocument doc;
    };

    bool operator==(XMLNode const& n1, XMLNode const &n2);

    // CLASS NOTES
    //
    // XpathResult is a special class for finding data in an XML document.
    // Use the find() method in XMLNode to search using XpathResults.
    //

    // can be a couple of different things
    // either a single int, float or string
    // or a set of ints, floats, strings or nodes
    class XpathResult {
        friend class XMLNode;
        
    public:
        uint            getNumMatches();
        
        // GET RESULT NODES
        XMLNode       getNode(uint idx=0);
        
        // GET RESULT VALUES
        std::string     getString(uint idx=0);
        int             getInt(uint idx=0);
        float           getFloat(uint idx=0);
        
    private:
        XpathResult(pugi::xpath_node_set nodes);
        pugi::xpath_node_set nodes;
    };
} /* End po Namespace */