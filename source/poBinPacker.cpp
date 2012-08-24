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

#include "poBinPacker.h"
#include <boost/foreach.hpp>

BinPacker::insertRect::insertRect(uint w, uint h, uint handle) : w(w), h(h), handle(handle) {}
bool BinPacker::insertRect::operator<(const insertRect &r) {
	if(w == r.w)
		return w > r.w;
	return h > r.h;
}

BinPacker::packRect::packRect() : x(0), y(0), width(0), height(0), handle(0), children(NULL), taken(false) {}

BinPacker::packRect::packRect(uint w, uint h) : x(0), y(0), width(w), height(h), handle(0), children(NULL), taken(false) {}

BinPacker::packRect::~packRect() {
	if(NULL != children) {
		delete children[0];
		delete children[1];
		delete [] children;
	}
}

void BinPacker::packRect::setDims(uint x, uint y, uint w, uint h) {
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

// insert dimensions of the image, recursively
BinPacker::packRect *BinPacker::packRect::insert(uint w, uint h) {
	if(taken)
		return NULL;
	
	// if we're a branch, pass image onto children
	if(NULL != children) {
		packRect *rect = children[0]->insert(w,h);
		if(NULL != rect)
			return rect;
		return children[1]->insert(w,h);
	}
	
	// make sure we don't have an image up in us
	if(0 != handle)
		return NULL;
	// if it won't fit, bail
	else if(width < w || height < h)
		return NULL;
	// maybe it'll fit perfectly?
	else if(width == w && height == h ) {
		taken = true;
		return this;
	}

	// otherwise, split or try to fit the image
	children = new packRect*[2];
	children[0] = new packRect();
	children[1] = new packRect();

	uint dw = width - w;
	uint dh = height - h;
	// this bin has more horizontal room so split vertically
	if(dw > dh) {
		children[0]->setDims(x,y,w,height);
		children[1]->setDims(x+w,y,dw,height);
	}
	// more vertical room so split horizontally
	else {
		children[0]->setDims(x,y,width,h);
		children[1]->setDims(x,y+h,width,dh);
	}

	return children[0]->insert(w,h);
}



BinPacker::BinPacker(uint w, uint h, uint p)
:	width(w), height(h), padding(p), handles(0)
{}

BinPacker::~BinPacker() {
	resetPackMap();
}

void BinPacker::reset() {
	rectangles.clear();
	handles = 0;
}

void BinPacker::resetPackMap() {
	BOOST_FOREACH(pack_page *page, pages) {
		BOOST_FOREACH(packRect* row, page->rows) {
			delete row;
		}
		page->rows.clear();
		page->pack.clear();
		delete page;
	}
	pages.clear();

	wastedPixels = 0.f;
}

uint BinPacker::addRect(poRect r) {
	uint tmp = handles++;
	rectangles.push_back(insertRect(r.width, r.height, tmp));
	return tmp;
}

int BinPacker::getNumPages() const {
	return pages.size();
}

float BinPacker::getWastedPixels() const {
	return wastedPixels;
}

poRect BinPacker::packPosition(uint handle, uint *page) {
	for(int i=0; i<pages.size(); i++) {
		if(pages[i]->pack.find(handle) != pages[i]->pack.end()) {
			if(page != NULL)
				*page = i;
			
			float x, y, w, h;
			x = pages[i]->pack[handle]->x + padding/2;
			y = pages[i]->pack[handle]->y + padding/2;
			w = pages[i]->pack[handle]->width - padding;
			h = pages[i]->pack[handle]->height - padding;
			
			return poRect(x, y, w, h);
		}
	}
	return poRect();
}

void BinPacker::pack() {
	resetPackMap();

	// sort by height
	rectangles.sort();
	
//	// find the widest
	std::list<insertRect>::iterator i, e;
//	for(i=rectangles.begin(),e=rectangles.end(); i!=e; i++)
//		width = std::max(width, i->w);

	std::list<insertRect> unused(rectangles.begin(),rectangles.end());

	uint insert_height=0, count=0, used_area=0;

	pack_page *page = new pack_page();
	pages.push_back(page);

	while(!unused.empty()) {
		// save this new row's height
		uint row_height = unused.front().h + padding;

		// we've run out of space on this page
		if(insert_height + row_height >= height) {
			// print stats
			wastedPixels += sqrtf(float(width*height-used_area));
			// and make the new page
			page = new pack_page();
			pages.push_back(page);
			insert_height = used_area = count = 0;
		}

		// start a new row
		packRect *row = new packRect();
		row->setDims(0,insert_height,width,row_height);
		page->rows.push_back(row);

		// go through all remaining rectangles and see who fits in the row
		i=unused.begin(), e=unused.end();
		while(i != e) {
			insertRect rect = *i;
			packRect *packed = row->insert(rect.w+padding, rect.h+padding);
			// managed to insert into this row
			if(NULL != packed) {
				// store this guy in the packing manifest
				packed->handle = rect.handle;
				page->pack[packed->handle] = packed;
				// remove this rect from consideration
				i = unused.erase(i);
				// update the used area
				used_area += rect.w * rect.h;
				// and the counter
				count++;
			}
			else {
				// otherwise move the iteration forward
				i++;
			}
		}

		// bump the height in preparation for the next row
		insert_height += row_height;
	}
}

#include "poOpenGLState.h"
#include "poSimpleDrawing.h"

void BinPacker::render(BinPacker::packRect *r) {
	if(NULL != r->children) {
		render(r->children[0]);
		render(r->children[1]);
	}
	else {
		if(r->handle != 0)
			po::setColor(poColor(1,.5,.5,1));
		else
			po::setColor(poColor(.5,.5,.5,1));

		po::drawFilledRect(r->x+1, r->y+1, (int)r->width-2, (int)r->height-2);
	}
}

void BinPacker::render() {
	po::saveModelviewThenIdentity();
	
	BOOST_FOREACH(pack_page *page, pages) {
		po::setColor(poColor::white);
		po::drawFilledRect(0,0,width,height);

		BOOST_FOREACH(packRect *rect, page->rows) {
			render(rect);
		}

		po::translate(poPoint(width, 0, 0));
	}
	
	po::restoreModelview();
}



