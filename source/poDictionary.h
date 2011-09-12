/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poColor.h"
#include "poPoint.h"
#include <pugixml.hpp>

class poObject;

typedef boost::variant<bool, int, float, std::string, poColor, poPoint, void*> poProperty;

// CLASS NOTES
//
// poDictionary implements a light-weight dictionary object. The dictionary can hold any number of uniquely
// named objects. The objects may be bools, ints, floats, strings, pointers, poPoints or poColors.
//
// A poDictionary is constructed and used as follows:
//
//      poDictionary D;
//
//      D.setInt( "age", 30 );
//      D.setString( "name", "bob" );
//      D.setColor( "hairColor", poColor::red );
// 
//      int age = D.getInt( "age" );
//      std::string name = D.getInt( "name" );
//      poColor hairColor = D.getColor( "hairColor" );
//
// poDictionary is used within poEvent system and by the messageHandler method of poObject.
// poDictionary allows for quick and simple way to pass around small bits of arbitrary data.
//
// poDictionary's may written and read to an XML file very easily using the read()
// and write() methods.
//
// Also, see the poCommon class below for application wide setting using a poDictionary.
//

class poDictionary {
public:
	bool			has(const std::string &name) const {return items.find(name) != items.end();}

	std::vector<std::string> keys() const;
	
	bool			getBool(const std::string &name) const {return boost::get<bool>(items.at(name));}
	poDictionary&	setBool(const std::string &name, bool b) {items[name] = b; return *this;}
	
	int             getInt(const std::string &name) const {return boost::get<int>(items.at(name));}
	poDictionary&	setInt(const std::string &name, int i) {items[name] = i; return *this; }
	
	float           getFloat(const std::string &name) const {return boost::get<float>(items.at(name));}
	poDictionary&	setFloat(const std::string &name, float f) {items[name] = f; return *this; }
	
	std::string     getString(const std::string &name) const {return boost::get<std::string>(items.at(name));}
	poDictionary&	setString(const std::string &name, const std::string &s) {items[name] = s; return *this; }
	
	poColor         getColor(const std::string &name) const {return boost::get<poColor>(items.at(name));}
	poDictionary&	setColor(const std::string &name, poColor c) {items[name] = c; return *this; }
	
	poPoint         getPoint(const std::string &name) const {return boost::get<poPoint>(items.at(name));}
	poDictionary&	setPoint(const std::string &name, poPoint p) {items[name] = p; return *this; }
	
//	poDictionary	getDictionary(const std::string &name) const {return boost::get<poDictionary>(items.at(name));}
//	poDictionary&	setDictionary(const std::string &name, const poDictionary &dict) {items[name] = dict; return *this;}
	
	template <typename T>
	T*				getPtr(const std::string &name) const {return static_cast<T*>(boost::get<void*>(items.at(name)));}
	poDictionary&	setPtr(const std::string &name, void* obj) {items[name] = obj; return *this;}
	
	poProperty		getProperty(const std::string &name) {return items[name];}
	void			setProperty(const std::string &name, const poProperty &prop) {items[name] = prop;}
	
	bool			read(const fs::path &url);
	void			write(const fs::path &url) const;
	
	void			set(const std::string &str);
	std::string		toString() const;

protected:
	void			set(pugi::xml_node root);
	void			unset(pugi::xml_node root) const;
	boost::unordered_map<std::string, poProperty> items;
};


class poCommon : public poDictionary
{
public:
	static poDictionary *get();
	
private:
	explicit poCommon() {}
	~poCommon() {}
};

