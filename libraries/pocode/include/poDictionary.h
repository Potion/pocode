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
#include "poFont.h"
#include "poImage.h"
#include "poXML.h"

#include <boost/variant.hpp>

class poDictionary;

typedef boost::variant<int,float,std::string,poPoint,poColor,void*,boost::recursive_wrapper<poDictionary> > poDictionaryItem_t;

enum poDictionaryType {
	PO_INT_T=0,
	PO_FLOAT_T,
	PO_STRING_T,
	PO_POINT_T,
	PO_COLOR_T,
	PO_VOID_PTR_T,
	PO_DICTIONARY_T
};

class poDictionaryItem {
	friend std::ostream& operator<<(std::ostream &out, const poDictionaryItem& item);
	
public:
	poDictionaryItem();
	poDictionaryItem(const poDictionaryItem_t &item);
	
	bool				getBool() const;
	int					getInt() const;
	float				getFloat() const;
	std::string			getString() const;
	poPoint				getPoint() const;
	poColor				getColor() const;
	void*				getPtr() const;
	poDictionary		getDictionary() const;
	poDictionaryType	getType() const;
	
	void				setBool(const poDictionaryItem_t &i);
	std::string			toString();
	
private:
	poDictionaryItem_t item;
};

typedef std::map<std::string, poDictionaryItem> poDictionaryItemMap;

class poDictionary {
	friend std::ostream& operator<<(std::ostream &out, const poDictionary &dict);
	
public:
	poDictionary();
	poDictionary		copy();
	
	bool				getBool(const std::string &s) const;
	int					getInt(const std::string &s) const;
	float				getFloat(const std::string &s) const;
	std::string			getString(const std::string &s) const;
	poPoint				getPoint(const std::string &s) const;
	poColor				getColor(const std::string &s) const;
	void*				getPtr(const std::string &s) const;
	poDictionary		getDictionary(const std::string &s) const;
	poDictionaryType	getType(const std::string &s) const;
	
	poDictionaryItem	get(const std::string &s) const;
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
	struct DictionaryImpl {
		poDictionaryItemMap items;
	};
	boost::shared_ptr<DictionaryImpl> shared;
};
