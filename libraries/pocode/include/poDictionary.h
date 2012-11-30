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

#pragma once


#include "poColor.h"
#include "poPoint.h"
#include "poXML.h"
#include "poEnums.h"

#include <boost/variant.hpp>

namespace po {
    class Dictionary;

    typedef boost::variant<int,float,std::string,Point,Color,void*,boost::recursive_wrapper<Dictionary> > DictionaryItem_t;

<<<<<<< HEAD
    class DictionaryItem {
        friend std::ostream& operator<<(std::ostream &out, const DictionaryItem& item);
        
    public:
        DictionaryItem();
        DictionaryItem(const DictionaryItem_t &item);
        
        bool				getBool() const;
        int					getInt() const;
        float				getFloat() const;
        std::string			getString() const;
        Point				getPoint() const;
        Color				getColor() const;
        void*				getPtr() const;
        Dictionary		getDictionary() const;
        DictionaryType	getType() const;
        
        void				setBool(const DictionaryItem_t &i);
        std::string			toString();
        
    private:
        DictionaryItem_t item;
    };
=======
enum poDictionaryType {
	PO_INT_T=0,
	PO_FLOAT_T,
	PO_STRING_T,
	PO_POINT_T,
	PO_COLOR_T,
	PO_VOID_PTR_T,
	PO_DICTIONARY_T,
	PO_DICTIONARY_NONE_T
};
>>>>>>> master

    typedef std::map<std::string, DictionaryItem> DictionaryItemMap;

<<<<<<< HEAD
    class Dictionary {
        friend std::ostream& operator<<(std::ostream &out, const Dictionary &dict);
        
    public:
        Dictionary();
        Dictionary		copy();
        
        bool				getBool(const std::string &s) const;
        int					getInt(const std::string &s) const;
        float				getFloat(const std::string &s) const;
        std::string			getString(const std::string &s) const;
        Point				getPoint(const std::string &s) const;
        Color				getColor(const std::string &s) const;
        void*				getPtr(const std::string &s) const;
        Dictionary		getDictionary(const std::string &s) const;
        DictionaryType	getType(const std::string &s) const;
        
        DictionaryItem	get(const std::string &s) const;
        Dictionary&		set(const std::string &s, const DictionaryItem_t &di);
        Dictionary&		append(const Dictionary &d);
        
        bool				has(const std::string &s) const;
        size_t				count() const;
        std::vector<std::string> keys() const;
        
        void				write(XMLNode node);
        void				write(XMLDocument &doc);
        void				write(const std::string &url);
        void				read(XMLNode node);
        void				read(const std::string &url);
        
        DictionaryItemMap::iterator begin();
        DictionaryItemMap::iterator end();
        DictionaryItemMap::const_iterator begin() const;
        DictionaryItemMap::const_iterator end() const;
        
    private:
        DictionaryItemMap items;
    };
}/* End po namespace */
=======
typedef std::map<std::string, poDictionaryItem> poDictionaryItemMap;

class poDictionary {
	friend std::ostream& operator<<(std::ostream &out, const poDictionary &dict);
	
public:
	poDictionary();
	poDictionary(std::string const k, poDictionaryItem_t const& v);
	poDictionary		copy();
	
	bool				getBool(const std::string &s, bool def=false) const;
	int					getInt(const std::string &s, int def=0) const;
	float				getFloat(const std::string &s, float def=0.f) const;
	std::string			getString(const std::string &s, std::string const& def="") const;
	poPoint				getPoint(const std::string &s, poPoint def=poPoint()) const;
	poColor				getColor(const std::string &s, poColor def=poColor::transparent) const;
	void*				getPtr(const std::string &s, void* def=NULL) const;
	poDictionary		getDictionary(const std::string &s, poDictionary def=poDictionary()) const;
	poDictionaryType	getType(const std::string &s, poDictionaryType def=PO_DICTIONARY_NONE_T) const;
	
	poDictionaryItem	get(const std::string &s, poDictionaryItem def=poDictionaryItem()) const;
	poDictionary&		set(const std::string &s, const poDictionaryItem_t &di);
	poDictionary&		append(const poDictionary &d);
	
	bool				has(const std::string &s) const;
	size_t				count() const;
	std::vector<std::string> keys() const;
	
	void				write(poXMLNode node);
	void				write(poXMLDocument &doc);
	void				write(const std::string &url);
	void				read(poXMLNode node);
	void				read(const std::string &url);
	
	poDictionaryItemMap::iterator begin();
	poDictionaryItemMap::iterator end();
	poDictionaryItemMap::const_iterator begin() const;
	poDictionaryItemMap::const_iterator end() const;
	
private:
	poDictionaryItemMap items;
};
>>>>>>> master
