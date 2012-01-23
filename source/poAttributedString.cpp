//
//  AttributedString.cpp
//  pocode
//
//  Created by Joshua Fisher on 10/20/11.
//  Copyright (c) 2011 Potion Design. All rights reserved.
//
#include "poAttributedString.h"

namespace po {
	
	RangeDictSorter::RangeDictSorter(int idx) 
	:	idx(idx) 
	{}
	
	bool RangeDictSorter::operator()(const RangeDict &a, const RangeDict &b) {
		float as_dist = abs(a.range.start - idx);
		float ae_dist = abs(a.range.end - idx);
		float bs_dist = abs(b.range.start - idx);
		float be_dist = abs(b.range.end - idx);
		
		if(as_dist == bs_dist)
			return ae_dist > be_dist;
		return as_dist > bs_dist;
	}

	OutsideRange::OutsideRange(int idx) : idx(idx) {}
	bool OutsideRange::operator()(const RangeDict &a) {
		return !(a.range.start <= idx && a.range.end > idx);
	}

	AttributedString &AttributedString::append(const std::string &str) {
		this->string += str; return *this;
	}
	
	AttributedString &AttributedString::append(Range r, const poDictionary &dict) {
		RangeDict range;
		range.range = r;
		range.dict = dict;
		attribs.push_back(range);
		return *this;
	}
	
	AttributedString &AttributedString::append(RangeDict r) {
		attribs.push_back(r);
		return *this;
	}
		
	// all the dictionaries that apply to a given point combined 
	poDictionary AttributedString::attributes(int idx) {
		// TODO verify this does what i want
		DictionaryVec filtered;
		// strip out any ranges that just don't apply
		std::remove_copy_if(attribs.begin(), attribs.end(), back_inserter(filtered), OutsideRange(idx));
		// sort by distance to the current idx
		// criteria:
		//	start closest to idx should be towards the end of the list
		//	if starts are same distance then the closest end should be further back
		//	eg	|---------|		//1
		//		--|----|---		//2
		//		-|------|--		//3
		//		---o-------
		//	should sort to 1,3,2
		std::sort(filtered.begin(), filtered.end(), RangeDictSorter(idx));
		
		poDictionary response = filtered.front().dict;
		
		DictionaryVec::iterator i;
		for(i=filtered.begin()+1; i!=filtered.end(); ++i) {
			response.append(i->dict);
		}
		return response;
	}
		
	bool AttributedString::empty() {
		return string.empty();
	}
	
	std::string &AttributedString::str() {
		return string;
	}
	
	const std::string &AttributedString::str() const {
		return string;
	}
	
	std::string::const_iterator AttributedString::begin() const {
		return string.begin();
	}
	
	std::string::const_iterator AttributedString::end() const {
		return string.end();
	}

}


