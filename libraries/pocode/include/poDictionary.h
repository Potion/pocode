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

    typedef std::map<std::string, DictionaryItem> DictionaryItemMap;

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