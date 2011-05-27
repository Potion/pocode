/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poColor.h"
#include "poPoint.h"

class poObject;

typedef boost::variant<int, float, std::string, poColor, poPoint, void*> poProperty;

class poDictionary {
public:
	int             getInt(const std::string &name) {return boost::get<int>(items[name]);}
	poDictionary	setInt(const std::string &name, int i) {items[name] = i; return *this; }
	
	float           getFloat(const std::string &name) {return boost::get<float>(items[name]);}
	poDictionary&	setFloat(const std::string &name, float f) {items[name] = f; return *this; }
	
	std::string     getString(const std::string &name) {return boost::get<std::string>(items[name]);}
	poDictionary&	setString(const std::string &name, const std::string &s) {items[name] = s; return *this; }
	
	poColor         getColor(const std::string &name) {return boost::get<poColor>(items[name]);}
	poDictionary&	setColor(const std::string &name, poColor c) {items[name] = c; return *this; }
	
	poPoint         getPoint(const std::string &name) {return boost::get<poPoint>(items[name]);}
	poDictionary&	setPoint(const std::string &name, poPoint p) {items[name] = p; return *this; }
	
					template <typename T>
	T*				getPtr(const std::string &name) {return static_cast<T*>(boost::get<void*>(items[name]));}
	void			setPtr(const std::string &name, void* obj) {items[name] = obj;}
	
	bool			read(const fs::path &url);
	void			write(const fs::path &url);

protected:
	boost::unordered_map<std::string, poProperty> items;
};
