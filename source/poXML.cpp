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
//  File.cpp
//  pocode
//
//  Created by Joshua Fisher on 7/7/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poXML.h"
#include "poHelpers.h"

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace pugi;

#define LCAST(type,val) boost::lexical_cast<type>(val)

namespace po {
    // -----------------------------------------------------------------------------------
    // ================================ Class: XPathResult ===============================
    #pragma mark - XPathResult -

    XpathResult::XpathResult(xpath_node_set nodes) 
    :	nodes(nodes) 
    {}

    uint XpathResult::getNumMatches() {
        return nodes.size();
    }
    
    
    //------------------------------------------------------------------
    XMLNode XpathResult::getNode(uint idx) {
        return XMLNode(nodes[idx].node());
    }
    
    
    //------------------------------------------------------------------
    std::string XpathResult::getString(uint idx) {
        return nodes[idx].attribute().value();
    }
    
    
    //------------------------------------------------------------------
    int XpathResult::getInt(uint idx) {
        return nodes[idx].attribute().as_int();
    }
    
    
    //------------------------------------------------------------------
    float XpathResult::getFloat(uint idx) {
        return nodes[idx].attribute().as_float();
    }




    // -----------------------------------------------------------------------------------
    // ================================ Class: XMLNode ============================
    #pragma mark - XMLNode -

    XMLNode::XMLNode() : node() {}

    XMLNode::XMLNode(xml_node node)
    :	node(node) 
    {}
    
    
    //------------------------------------------------------------------
    bool XMLNode::isValid() const {
        return strlen(node.name()) > 0 || node.first_attribute() || !node.empty();
    }
    
    
    //------------------------------------------------------------------
    std::string XMLNode::getName() const {
        return node.name();
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setName(const std::string &str) {
        node.set_name(str.c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    int XMLNode::getInnerInt() const {
        return LCAST(int, node.child_value());
    }
    
    
    //------------------------------------------------------------------
    float XMLNode::getInnerFloat() const {
        return LCAST(float, node.child_value());
    }
    
    
    //------------------------------------------------------------------
    std::string XMLNode::getInnerString() const {
        return node.child_value();
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setInnerString(const std::string &str) {
        for(xml_node::iterator iter=node.begin(); iter!=node.end(); iter++) {
            node.remove_child(*iter);
        }
        
        node.append_child(node_pcdata);
        node.first_child().set_value(str.c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setInnerInt(int i) {
        if(!node.first_child())
            node.append_child(node_pcdata);
        
        node.first_child().set_value(LCAST(std::string,i).c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setInnerFloat(float f) {
        if(!node.first_child())
            node.append_child(node_pcdata);
        
        node.first_child().set_value(LCAST(std::string,f).c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    uint XMLNode::getNumAttributes() const {
        return std::distance(node.attributes_end(), node.attributes_begin());
    }
    
    
    //------------------------------------------------------------------
    bool XMLNode::hasAttribute(const std::string &name) const {
        return node.attribute(name.c_str());
    }
    
    
    //------------------------------------------------------------------
    std::vector<std::string> XMLNode::getAttributeNames() const {
        std::vector<std::string> response;

        for(xml_attribute_iterator i=node.attributes_begin(); i != node.attributes_end(); ++i) {
            response.push_back(i->name());
        }
        
        return response;
    }
    
    
    //------------------------------------------------------------------
    int XMLNode::getIntAttribute(const std::string &name) const {
        return node.attribute(name.c_str()).as_int();
    }
    
    
    //------------------------------------------------------------------
    float XMLNode::getFloatAttribute(const std::string &name) const {
        return node.attribute(name.c_str()).as_float();
    }
    
    
    //------------------------------------------------------------------
    std::string XMLNode::getStringAttribute(const std::string &name) const {
        return node.attribute(name.c_str()).value();
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setAttribute(const std::string &name, int value) {
        if(!hasAttribute(name))
            node.append_attribute(name.c_str());
        node.attribute(name.c_str()) = value;
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setAttribute(const std::string &name, float value) {
        if(!hasAttribute(name))
            node.append_attribute(name.c_str());
        node.attribute(name.c_str()) = value;
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::setAttribute(const std::string &name, const std::string &value) {
        if(!hasAttribute(name))
            node.append_attribute(name.c_str());
        node.attribute(name.c_str()) = value.c_str();
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode& XMLNode::removeAttribute(const std::string &name) {
        node.remove_attribute(name.c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    uint XMLNode::getNumChildren() const {
        int size = 0;
        xml_node::iterator n = node.begin();
        while(n != node.end()) {
            size++;
            ++n;
        };
        return size;
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::addChild(const std::string &name) {
        return XMLNode(node.append_child(name.c_str()));
    }
    
    
    //------------------------------------------------------------------
    XMLNode &XMLNode::addChild(const XMLNode &node) {
        this->node.append_copy(node.node);
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode &XMLNode::removeChild(const std::string &name) {
        node.remove_child(name.c_str());
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode &XMLNode::removeChild(const XMLNode &node) {
        this->node.remove_child(node.node);
        return *this;
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::getChild(uint idx) {
        if(idx > getNumChildren())
            return XMLNode();
        
        std::vector<XMLNode> kids = getChildren();
        return kids[idx];
    }


    XMLNode XMLNode::getChild(const std::string &name) {
        return XMLNode(node.child(name.c_str()));
    }
    
    
    //------------------------------------------------------------------
    std::vector<XMLNode> XMLNode::getChildren() {
        std::vector<XMLNode> response;
        xml_node n = node.first_child();
        while(n) {
            response.push_back(XMLNode(n));
            n = n.next_sibling();
        }
        return response;
    }
    
    
    //------------------------------------------------------------------
    std::vector<XMLNode> XMLNode::getChildren(const std::string &name) {
        std::vector<XMLNode> response;
        xml_node n = node.child(name.c_str());
        while(n) {
            response.push_back(XMLNode(n));
            n = n.next_sibling(name.c_str());
        }
        return response;
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::getFirstChild() {
        return XMLNode(node.first_child());
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::getLastChild() {
        return XMLNode(node.last_child());
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::getNextSibling() {
        return XMLNode(node.next_sibling());
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLNode::getNextSibling(const std::string &name) {
        return XMLNode(node.next_sibling(name.c_str()));
    }
    
    
    //------------------------------------------------------------------
    XpathResult XMLNode::find(const std::string &xpath) {
        return XpathResult(node.select_nodes(xpath.c_str()));
    }
    
    
    //------------------------------------------------------------------
    pugi::xml_node XMLNode::getHandle() const {
        return node;
    }
    
    
    //------------------------------------------------------------------
    bool operator==(XMLNode const& n1, XMLNode const &n2) {
        return n1.getHandle() == n2.getHandle();
    }




    // -----------------------------------------------------------------------------------
    // ================================ Class: XMLDocument ===============================
    #pragma mark - XMLDocument -

    XMLDocument::XMLDocument() {
        document.reset(new pugi::xml_document);
        document->append_child("root");
    }
    
    
    //------------------------------------------------------------------
    XMLDocument::XMLDocument(const std::string &url) {
        document.reset(new pugi::xml_document);
        read(url);
    }
    
    
    //------------------------------------------------------------------
    bool XMLDocument::isValid() const {
        return document->first_child() != NULL;
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLDocument::getRootNode() const {
        if(!isValid())
            return XMLNode();
        
        return XMLNode(document->first_child());
    }
    
    
    //------------------------------------------------------------------
    XMLNode XMLDocument::resetRootNode() {
        document.reset(new pugi::xml_document);
        return XMLNode(document->append_child());
    }
    
    
    //------------------------------------------------------------------
    bool XMLDocument::read(const std::string &url) {
        document.reset(new pugi::xml_document);
        xml_parse_result result = document->load_file(url.c_str(), parse_default, encoding_utf8);
        if(result.status == status_ok) {
            return true;
        }
        
        po::log("XML: parse error (file: %s) (error: %s)", url.c_str(), result.description());
        return false;
    }
    
    
    //------------------------------------------------------------------
    bool XMLDocument::readStr(const std::string &str) {
        document.reset(new pugi::xml_document);
        xml_parse_result result = document->load(str.c_str());
        if(result.status == status_ok) {
            return true;
        }
        
        po::log("XML: parse error (error: %s)", result.description());
        return false;
    }
    
    
    //------------------------------------------------------------------
    bool XMLDocument::write(const std::string &url) {
        return document->save_file(url.c_str());
    }
    
    
    //------------------------------------------------------------------
    void XMLDocument::print() const {
        document->print(std::cout);
    }
    
    
    //------------------------------------------------------------------
    pugi::xml_document &XMLDocument::getHandle() const {
        return *document.get();
    }
    
    
    //------------------------------------------------------------------
    XMLDocument XMLDocument::copy() {
        XMLDocument doc;
        doc.document.reset(new pugi::xml_document);
        doc.document->reset(*document);
        return doc;
    }
} /* End po namespace */