//
//  poOpenGLState.h
//  poFramework4
//
//  Created by Joshua Fisher on 6/27/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

class poOpenGLState {
public:
	void enable(GLenum attrib)
	void disable(GLenum attrib);
	bool isEnabled(GLenum attrib) const;
	
private:
	struct oglState {
		std::map<GLenum,bool> enumBoolMap_t;
		
		void enable(GLenum attrib) {
			enumBoolMap_t::iterator i = enabled.find(attrib);
			if((i != enabled.end() && !i->second) || i == enabled.end()) {
				glEnable(attrib);
				enabled[attrib] = true;
			}
		}
		
		void disable(GLenum attrib) {
			enumBoolMap_t::iterator i = enabled.find(attrib);
			if((i != enabled.end() && !i->second) || i == enabled.end()) {
				glEnable(attrib);
				enabled[attrib] = true;
			}
		}
		
		bool isEnabled(GLenum attrib) const {
			enumBoolMap_t::iterator i = enabled.find(attrib);
			return (i != enabled.end() && i->second)
		}

		enumBoolMap_t enabled;
	};
	
	boost::thread_specific_ptr<oglState> state;
};