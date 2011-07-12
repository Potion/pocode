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
		RangeDictSorter(int idx) : idx(idx) {}
		bool operator()(const RangeDict &a, const RangeDict &b) {
			float as_dist = abs(a.range.start - idx);
			float ae_dist = abs(a.range.end - idx);
			float bs_dist = abs(b.range.start - idx);
			float be_dist = abs(b.range.end - idx);
			
			if(as_dist == bs_dist)
				return ae_dist < be_dist;
			return as_dist < bs_dist;
		}
	};

	struct OutsideRange {
		int idx;
		OutsideRange(int idx) : idx(idx) {}
		bool operator()(const RangeDict &a) {
			return !(a.range.start <= idx && a.range.end >= idx);
		}
	};

	typedef std::vector<RangeDict> DictionaryVec;

	class AttributedString {
	public:
		AttributedString &append(const std::string &str) {this->string += str; return *this;}
		AttributedString &append(Range r, const poDictionary &dict) {
			RangeDict range;
			range.range = r;
			range.dict = dict;
			attribs.push_back(range);
			return *this;
		}
		AttributedString &append(RangeDict r) {
			attribs.push_back(r);
			return *this;
		}
		
		// all the dictionaries that apply to a given point combined 
		poDictionary attributes(int idx) {
			// TODO verify this does what i want
			DictionaryVec filtered;
			// strip out any ranges that just don't apply
			std::remove_copy_if(attribs.begin(), attribs.end(), filtered.begin(), OutsideRange(idx));
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
				std::vector<std::string> keys = i->dict.keys();
				BOOST_FOREACH(std::string &key, keys) {
					response.setProperty(key, i->dict.getProperty(key));
				}
			}
			return response;
		}
		
		bool empty()								{return string.empty();}
		std::string &str()							{return string;}
		const std::string &str() const				{return string;}
		std::string::const_iterator begin() const	{return string.begin();}
		std::string::const_iterator end() const		{return string.end();}
		
	private:
		std::string string;
		DictionaryVec attribs;
	};
	
}
