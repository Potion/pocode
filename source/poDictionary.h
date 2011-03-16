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
	int		getInt(const std::string &name) {return boost::get<int>(items[name]);}
	void	setInt(const std::string &name, int i) {items[name] = i;}
	
	float	getFloat(const std::string &name) {return boost::get<float>(items[name]);}
	void	setFloat(const std::string &name, float f) {items[name] = f;}
	
	std::string getString(const std::string &name) {return boost::get<std::string>(items[name]);}
	void	setString(const std::string &name, const std::string &s) {items[name] = s;}
	
	poColor	getColor(const std::string &name) {return boost::get<poColor>(items[name]);}
	void	getColor(const std::string &name, poColor c) {items[name] = c;}
	
	poPoint getPoint(const std::string &name) {return boost::get<poPoint>(items[name]);}
	void	getPoint(const std::string &name, poPoint p) {items[name] = p;}
	
	template <typename T>
	T*		getPtr(const std::string &name) {return static_cast<T*>(boost::get<void*>(items[name]));}
	void	setPtr(const std::string &name, void* obj) {items[name] = obj;}

protected:
	boost::unordered_map<std::string, poProperty> items;
};
