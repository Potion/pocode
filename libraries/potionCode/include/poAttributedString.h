//
//  AttributedString.h
//  poFramework4
//
//  Created by Joshua Fisher on 7/8/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poDictionary.h"
#include <boost/iterator.hpp>

namespace po {
	
	struct Range {
		int start, end;
	};

	struct RangeDict {
		Range range;
		poDictionary dict;
	};

	struct RangeDictSorter {
		int idx;
		RangeDictSorter(int idx);
		bool operator()(const RangeDict &a, const RangeDict &b);
	};

	struct OutsideRange {
		int idx;
		OutsideRange(int idx);
		bool operator()(const RangeDict &a);
	};

	typedef std::vector<RangeDict> DictionaryVec;

	class AttributedString {
	public:
		AttributedString &append(const std::string &str);
		AttributedString &append(Range r, const poDictionary &dict);
		AttributedString &append(RangeDict r);
		
		// all the dictionaries that apply to a given point combined 
		poDictionary attributes(int idx);
		
		bool empty();
		std::string &str();
		const std::string &str() const;
		std::string::const_iterator begin() const;
		std::string::const_iterator end() const;
		
	private:
		std::string string;
		DictionaryVec attribs;
	};
	
}
