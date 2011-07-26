//
//  poDictionary.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poDictionary.h"

std::vector<std::string> poDictionary::keys() const {
	std::vector<std::string> response;
	boost::unordered_map<std::string,poProperty>::const_iterator i;
	for(i=items.begin(); i!=items.end(); ++i)
		response.push_back(i->first);
	return response;
}

bool poDictionary::read(const fs::path &url) {
	if(!fs::exists(url) || !fs::is_regular_file(url))
		return false;
	
	TiXmlDocument doc(url.c_str());
	doc.LoadFile();
	
	TiXmlElement *root = doc.RootElement();
	
	TiXmlNode *child = NULL;
	while((child = root->IterateChildren("key", child))) {
		TiXmlElement *k = child->ToElement();
		int type;
		k->Attribute("type", &type);
		
		std::string key = k->GetText();
		std::string value = child->NextSibling()->ToElement()->GetText();
		
		std::stringstream ss(value);
		
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
				ss >> color;
				setColor(key, color);
				break;
			}
			case 5:
			{
				poPoint point;
				ss >> point;
				setPoint(key, point);
				break;
			}
		}
	}
	
	return true;
}

void poDictionary::write(const fs::path &url) const {
	TiXmlDocument doc;
	TiXmlElement *root = new TiXmlElement("root");
	doc.LinkEndChild(root);
	
	TiXmlElement *key, *value;
	
	boost::unordered_map<std::string,poProperty>::const_iterator iter = items.begin();
	while(iter != items.end()) {
		key = new TiXmlElement("key");
		key->SetAttribute("type", iter->second.which());
		key->LinkEndChild(new TiXmlText(iter->first));
		root->LinkEndChild(key);
		
		value = new TiXmlElement("value");
		value->LinkEndChild(new TiXmlText(boost::lexical_cast<std::string>(iter->second)));
		root->LinkEndChild(value);
		
		iter++;
	}
	
	doc.SaveFile(url.c_str());
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

