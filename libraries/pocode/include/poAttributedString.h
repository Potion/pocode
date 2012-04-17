/*
 *	Copyright 2012 Potion Design. All rights reserved.
 *	This file is part of pocode.
 *
 *	pocode is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as 
 *	published by the Free Software Foundation, either version 3 of 
 *	the License, or (at your option) any later version.
 *
 *	pocode is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public 
 *	License along with pocode.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//
//  AttributedString.h
//  pocode
//
//  Created by Joshua Fisher on 7/8/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#include "poDictionary.h"
#include <boost/iterator.hpp>

namespace po {
	
	struct Range {
		Range(int s, int e);
		int start, end;
		
		friend std::ostream &operator<<(std::ostream &out, const Range& r) {out<<"{"<<r.start<<","<<r.end<<"}"; return out;}
	};

	class AttributedString {
	public:
		AttributedString &append(const std::string &str);
		AttributedString &append(Range r, const poDictionary &dict);
		
		// all the dictionaries that apply to a given point combined 
		poDictionary attributes(int idx);
		
		bool empty();
		std::string &str();
		const std::string &str() const;
		std::string::const_iterator begin() const;
		std::string::const_iterator end() const;
		
	private:
		struct RangeDict {
			RangeDict(Range r, const poDictionary &d);
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
		
		std::string string;
		DictionaryVec attribs;
	};
	
}
