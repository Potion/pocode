/*
 *  Copyright 2011 Potion Design. All rights reserved.
 */

#pragma once

#include "poCommon.h"

class poObject;

class poDictionary {
public:
	int		getInt(const std::string& name) {return boost::any_cast<int>(items[name]);}
	float	getFloat(const std::string& name) {return boost::any_cast<float>(items[name]);}
	bool	getBool(const std::string& name) {return boost::any_cast<bool>(items[name]);}
	char	getChar(const std::string& name) {return boost::any_cast<char>(items[name]);}
	std::string	getString(const std::string& name) {return boost::any_cast<std::string>(items[name]);}
	ci::ColorA	getColor(const std::string& name) {return boost::any_cast<ci::ColorA>(items[name]);}
	poPoint	getPoint(const std::string& name) {return boost::any_cast<poPoint>(items[name]);}
	void*	getVoidPtr(const std::string& name) {return boost::any_cast<void*>(items[name]);}
	poObject* getObjectPtr(const std::string& name) {return boost::any_cast<poObject*>(items[name]);}
	
	void	addInt(const std::string& name, int val) {items[name] = val;}
	void	addFloat(const std::string& name, float val) {items[name] = val;}
	void	addBool(const std::string& name, bool val) {items[name] = val;}
	void	addChar(const std::string& name, char val) {items[name] = val;}
	void	addString(const std::string& name, const std::string& val) {items[name] = val;}
	void	addColor(const std::string& name, ci::ColorA val) {items[name] = val;}
	void	addPoint(const std::string& name, poPoint val) {items[name] = val;}
	void	addVoidPtr(const std::string& name, void* val) {items[name] = val;}
	void	addObjectPtr(const std::string& name, poObject* val) {items[name] = val;}
	
private:
	boost::unordered_map<std::string, boost::any> items;
};
