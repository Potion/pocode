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

#include "poDictionary.h"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace boost;


namespace po {

    namespace {
        
        struct item_printer : public static_visitor<> {
            ostream &out;
            item_printer(ostream &os) : out(os) {}
            
            void operator()(const int& i) const			{out << i;}
            void operator()(const float& d) const		{out << std::fixed << d;}
            void operator()(const string& s) const		{out << "'" << s << "'";}
            void operator()(const Point& p) const		{out << p;}
            void operator()(const Color& c) const		{out << c;}
            void operator()(const void* p) const		{out << "&" << hex << (size_t)p;}
            void operator()(const Dictionary& d) const{out << d;}
        };
        
        struct string_converter : public static_visitor<string> {
            ostringstream ss;
            
            string operator()(int& i)			{ss << i; return ss.str();}
            string operator()(float& d)			{ss << std::fixed << d; return ss.str();}
            string operator()(string& s)		{ss << "'" << s << "'"; return ss.str();}
            string operator()(Point& p)		{ss << p; return ss.str();}
            string operator()(Color& c)		{ss << c; return ss.str();}
            string operator()(void* p)			{ss << "&" << hex << (size_t)p; return ss.str();}
            string operator()(Dictionary& d)	{ss << d; return ss.str();}
        };
        
    }
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: DictionaryItem ============================
    #pragma mark - Dictionary Item -
    
    DictionaryItem::DictionaryItem()
    : item(0)
    {}
    
    DictionaryItem::DictionaryItem(const DictionaryItem_t &item) 
    : item(item)
    {}
    
    
    //------------------------------------------------------------------
    bool DictionaryItem::getBool() const {
		
		try {
			return (bool)boost::get<int>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not a boolean\n";
		}
    }
    
    //------------------------------------------------------------------
    int DictionaryItem::getInt() const {
		try {
			return boost::get<int>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not an int\n";
		}
    }
    
    //------------------------------------------------------------------
    float DictionaryItem::getFloat() const {
		try {
			return boost::get<float>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not a float " << item << " \n";
		}
    }
    
    
    //------------------------------------------------------------------
    string DictionaryItem::getString() const {
        try {
			return boost::get<string>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not a string\n";
		}
    }
    
    //------------------------------------------------------------------
    Point DictionaryItem::getPoint() const {
        try {
			return boost::get<Point>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not a po::Point\n";
		}
    }
    
    
    //------------------------------------------------------------------
    Color DictionaryItem::getColor() const {
        try {
			return boost::get<Color>(item);
		} catch (boost::bad_get const &e) {
			std::cerr << boost::diagnostic_information(e);
			cerr << "DictionaryItem: type is not a po::Color\n";
		}
    }
    
    //------------------------------------------------------------------
    void* DictionaryItem::getPtr() const {
        return boost::get<void*>(item);
    }
    
    
    //------------------------------------------------------------------
    Dictionary DictionaryItem::getDictionary() const {
        return boost::get<Dictionary>(item);
    }
    
    
    //------------------------------------------------------------------
    DictionaryType DictionaryItem::getType() const {
        return (DictionaryType)item.which();
    }
    
    
    //------------------------------------------------------------------
    void DictionaryItem::setBool(const DictionaryItem_t &i) {
        item = i;
    }
    
    
    //------------------------------------------------------------------
    string DictionaryItem::toString() {
        string_converter conv;
        return boost::apply_visitor(conv, item);
    }
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: Dictionary ================================
    #pragma mark - Dictionary -

    Dictionary::Dictionary() {
    }
        
        
    Dictionary::Dictionary(std::string const k, DictionaryItem_t const& v) {
        set(k,v);
    }
    
    Dictionary Dictionary::copy() {
        Dictionary dict;
        for(DictionaryItemMap::iterator iter=begin(); iter!=end(); ++iter) {
            if(iter->second.getType() == po::DICTIONARY_T)
                dict.set(iter->first, iter->second.getDictionary().copy());
            else {
                dict.items[iter->first] = iter->second;
            }
        }
        return dict;
    }
    
    
    //------------------------------------------------------------------
    bool Dictionary::getBool(const string &s) const {
        return items.at(s).getBool();
    }
    
    
    //------------------------------------------------------------------
    int Dictionary::getInt(const string &s) const {
        return items.at(s).getInt();
    }
    
    
    //------------------------------------------------------------------
    float Dictionary::getFloat(const string &s) const {
        return items.at(s).getFloat();
    }
    
    
    //------------------------------------------------------------------
    string Dictionary::getString(const string &s) const {
        return items.at(s).getString();
    }
    
    
    //------------------------------------------------------------------
    Point Dictionary::getPoint(const std::string &s) const {
        return items.at(s).getPoint();
    }
    
    
    //------------------------------------------------------------------
    Color Dictionary::getColor(const std::string &s) const {
        return items.at(s).getColor();
    }
    
    
    //------------------------------------------------------------------
    void* Dictionary::getPtr(const string &s) const {
        return items.at(s).getPtr();
    }
    
    
    //------------------------------------------------------------------
    Dictionary Dictionary::getDictionary(const string &s) const {
        return items.at(s).getDictionary();
    }
    
    
    //------------------------------------------------------------------
    DictionaryType Dictionary::getType(const string &s) const {
        return items.at(s).getType();
    }
    
    
    //------------------------------------------------------------------
    DictionaryItem Dictionary::get(const string &s) const {
        return items.at(s);
    }
    
    
    //------------------------------------------------------------------
    Dictionary& Dictionary::set(const string &s, const DictionaryItem_t &di) {
        items[s] = di;
        return *this;
    }
    
    //------------------------------------------------------------------
    Dictionary& Dictionary::append(const Dictionary &d) {
        DictionaryItemMap::const_iterator iter = d.begin();
        for(; iter!=d.end(); ++iter) {
            items[iter->first] = iter->second;
        }
        return *this;
    }
    
    
    //------------------------------------------------------------------
    bool Dictionary::has(const string &s) const {
        return items.count(s) > 0;
    }
    
    
    //------------------------------------------------------------------
    size_t Dictionary::count() const {
        return items.size();
    }
    
    
    //------------------------------------------------------------------
    vector<string> Dictionary::keys() const {
        vector<string> response;
        for(DictionaryItemMap::const_iterator iter=begin(); iter!=end(); iter++) {
            response.push_back(iter->first);
        }
        return response;
    }
    
    
    //------------------------------------------------------------------
    void Dictionary::write(XMLNode node) {
        node.setName("item");
        for(DictionaryItemMap::iterator iter=items.begin(); iter!=items.end(); ++iter) {
            XMLNode item = node.addChild("item");
            
            item.setAttribute("name", iter->first);
            item.setAttribute("type", iter->second.getType());

            if(iter->second.getType() == po::DICTIONARY_T) {
                Dictionary dict = iter->second.getDictionary();
                dict.write(item);
            }
            else {
                item.setInnerString(iter->second.toString().c_str());
            }
        }
    }
    
    
    //------------------------------------------------------------------
    void Dictionary::write(XMLDocument &doc) {
        XMLNode root = doc.getRootNode();
        root.setAttribute("name", "root");
        root.setAttribute("type", po::DICTIONARY_T);
        write(root);
    }
    
    
    //------------------------------------------------------------------
    void Dictionary::write(const std::string &url) {
        XMLDocument doc;
        write(doc);
        doc.write(url.c_str());
    }
    
    
    //------------------------------------------------------------------
    void Dictionary::read(XMLNode node) {
        int type = node.getIntAttribute("type");
        std::string name = node.getStringAttribute("name");
        
        switch(type) {
            case po::INT_T:
                set(name, node.getInnerInt());
                break;
            case po::FLOAT_T:
                set(name, node.getInnerFloat());
                break;
            case po::STRING_T:
                set(name, node.getInnerString());
                break;
            case po::POINT_T: {
                Point p;
                if(p.fromString(node.getInnerString()))
                    set(name, p);
                break;
            }
            case po::COLOR_T: {
                Color c;
                if(c.set(node.getInnerString()))
                    set(name, c);
                break;
            }
            case po::VOID_PTR_T:
                printf("reading void ptr from dictionary not supported; inserting address as string, though its basically meaningless\n");
                set(name, node.getInnerString());
                break;
            case po::DICTIONARY_T: {
                Dictionary dict;
                XMLNode item = node.getFirstChild();
                while(item.isValid()) {
                    dict.read(item);
                    item = item.getNextSibling();
                }
                set(node.getStringAttribute("name"), dict);
                break;
            }
        }
    }
    
    
    //------------------------------------------------------------------
    void Dictionary::read(const std::string &url) {
        XMLDocument doc(url);
        if(doc.isValid()) {
            XMLNode item = doc.getRootNode().getFirstChild();
            while(item.isValid()) {
                read(item);
                item = item.getNextSibling();
            }
        }
    }
    
    
    //------------------------------------------------------------------
    DictionaryItemMap::iterator Dictionary::begin() {
        return items.begin();
    }
    
    
    //------------------------------------------------------------------
    DictionaryItemMap::iterator Dictionary::end() {
        return items.end();
    }
    
    
    //------------------------------------------------------------------
    DictionaryItemMap::const_iterator Dictionary::begin() const {
        return items.begin();
    }
    
    
    //------------------------------------------------------------------
    DictionaryItemMap::const_iterator Dictionary::end() const {
        return items.end();
    }


    //------------------------------------------------------------------
    // stream-out operator for DictionaryItem
    ostream& operator<<(ostream &out, const DictionaryItem& item) {
        boost::apply_visitor(item_printer(out), item.item);
        return out;
    }
    
    
    //------------------------------------------------------------------
    // stream-out operator for Dictionary
    ostream& operator<<(ostream &out, const Dictionary &d) {
        out << "(";
        DictionaryItemMap::const_iterator iter=d.begin();
        while(iter!=d.end()) {
            out << "'" << iter->first << "':" << iter->second;
            if(++iter != d.end())
                out << ",";
        }
        return out << ") ";
    }
}/*End po namespace */
