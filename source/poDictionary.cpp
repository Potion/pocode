//
//  poDictionary.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poDictionary.h"
#include <sstream>

std::vector<std::string> poDictionary::keys() const {
	std::vector<std::string> response;
	boost::unordered_map<std::string,poProperty>::const_iterator i;
	for(i=items.begin(); i!=items.end(); ++i)
		response.push_back(i->first);
	return response;
}

poDictionary& poDictionary::addDictionaryElements(const poDictionary &dict){
    std::vector<std::string> keys = dict.keys();
    std::vector<std::string>::const_iterator i;
    for(i=keys.begin(); i!=keys.end(); ++i) {
        poProperty thisProperty = dict.getProperty(*i);
        this->setProperty(*i, thisProperty);
    }
    
    return *this;
}

bool poDictionary::read(const fs::path &url) {
	if(!fs::exists(url) || !fs::is_regular_file(url))
		return false;

	using namespace pugi;
	xml_document doc;
	xml_parse_result result = doc.load_file(url.c_str(), parse_default, encoding_utf8);
	if(!result) {
		return false;
	}

	set(doc.first_child());
	return true;
}

void poDictionary::write(const fs::path &url) const {
	using namespace pugi;
	
	xml_document doc;
	xml_node root = doc.append_child("root");
	
	unset(root);
	doc.save_file(url.c_str());
}

void poDictionary::set(const std::string &str) {
	using namespace pugi;
	xml_document doc;
	xml_parse_result result = doc.load(str.c_str());
	if(!result) {
		return;
	}
	set(doc.first_child());
}

std::string poDictionary::toString() const {
	using namespace pugi;
	
	xml_document doc;
	xml_node root = doc.append_child("root");

	unset(root);
	
	std::ostringstream ss;
	doc.save(ss);
	
	return ss.str();
}

void poDictionary::unset(pugi::xml_node root) const {
	using namespace pugi;
	
	boost::unordered_map<std::string,poProperty>::const_iterator iter = items.begin();
	while(iter != items.end()) {
		xml_node node = root.append_child("entry");
		
		node.append_attribute("type").set_value(iter->second.which());
		node.append_attribute("key").set_value(iter->first.c_str());
		node.append_child(node_pcdata).set_value(boost::lexical_cast<std::string>(iter->second).c_str());
		
		iter++;
	}
}

void poDictionary::set(pugi::xml_node node) {
	items.clear();
	
	node = node.first_child();
	while(node) {
		uint type = node.attribute("type").as_uint();
		std::string key = node.attribute("key").value();
		const char *value = node.child_value();
		
		switch(type) {
			case 0:
				setBool(key, boost::lexical_cast<bool>(value));
				break;
			case 1:
				setInt(key, boost::lexical_cast<int>(value));
				break;
			case 2:
				setFloat(key, boost::lexical_cast<float>(value));
				break;
			case 3:
				setString(key, value);
				break;
			case 4:
			{
				poColor color;
				if(color.set(value))
					setColor(key, color);
				break;
			}
			case 5:
			{
				poPoint point;
				if(point.set(value))
					setPoint(key, point);
				break;
			}
		}
		
		node = node.next_sibling();
	}
}

void deleteCommon(poDictionary* dict) {
	dict->write("common.xml");
	delete dict;
}

poDictionary *poCommon::get() {
	using namespace boost;
	
	static shared_ptr<poDictionary> instance;
	if(!instance) {
		instance = shared_ptr<poDictionary>(new poDictionary(), deleteCommon);
		instance->read("common.xml");
	}
	return instance.get();
}

