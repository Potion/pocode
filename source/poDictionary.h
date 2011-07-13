/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poColor.h"
#include "poPoint.h"

class poObject;

typedef boost::variant<bool, int, float, std::string, poColor, poPoint, void*> poProperty;

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
	poDictionary&	setString(const std::string &name, std::string &s) {items[name] = s; return *this; }
	
	poColor         getColor(const std::string &name) const {return boost::get<poColor>(items.at(name));}
	poDictionary&	setColor(const std::string &name, poColor c) {items[name] = c; return *this; }
	
	poPoint         getPoint(const std::string &name) const {return boost::get<poPoint>(items.at(name));}
	poDictionary&	setPoint(const std::string &name, poPoint p) {items[name] = p; return *this; }
	
	template <typename T>
	T*				getPtr(const std::string &name) const {return static_cast<T*>(boost::get<void*>(items.at(name)));}
	poDictionary&	setPtr(const std::string &name, void* obj) {items[name] = obj; return *this;}
	
	poProperty		getProperty(const std::string &name) {return items[name];}
	void			setProperty(const std::string &name, const poProperty &prop) {items[name] = prop;}
	
	bool			read(const fs::path &url);
	void			write(const fs::path &url) const;

protected:
	boost::unordered_map<std::string, poProperty> items;
};


class poCommon
:	public poDictionary
{
public:
	static poCommon *get();
	
private:
	explicit poCommon() {}
	~poCommon() {}
};