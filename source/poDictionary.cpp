//
//  poDictionary.cpp
//  poFramework4
//
//  Created by Joshua Fisher on 5/26/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poDictionary.h"

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

void poDictionary::write(const fs::path &url) {
	TiXmlDocument doc;
	TiXmlElement *root = new TiXmlElement("root");
	doc.LinkEndChild(root);
	
	TiXmlElement *key, *value;
	
	boost::unordered_map<std::string,poProperty>::iterator iter = items.begin();
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



poCommon *poCommon::get() {
	static poCommon *instance = NULL;
	if(!instance) {
		instance = new poCommon();
	}
	return instance;
}

