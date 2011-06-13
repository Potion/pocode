#pragma once

#include "poRect.h"

/*	addRect with the image dimensions, get back a handle
 *	use the handle to find where to put the image into digest
 *
 *	sorts by height, creates a "line" with the tallest rectangle
 *	then fills the leftover out to a set width
 *	once a line is full, it starts a new one
 *	bounded by width and height, will start a new page if it runs out of height
 */

class BinPacker {
public:
	// init with page size
	BinPacker(uint width, uint height);
	~BinPacker();

	void reset();
	uint addRect(poRect r);

	void pack();
	void render();

	int numPages() const;
	float wastedPixels() const;
	poRect packPosition(uint handle, uint *page=NULL);

private:
	BinPacker(const BinPacker &lhs) {}
	BinPacker &operator=(const BinPacker &lhs) {return *this;}

	struct insert_rect {
		insert_rect(uint w, uint h, uint handle);
		bool operator<(const insert_rect &r);
		uint w, h, handle;
	};
	std::list<insert_rect> rectangles;

	struct pack_rect {
		pack_rect();
		pack_rect(uint w, uint h);
		~pack_rect();
		void setDims(uint x, uint y, uint w, uint h);
		pack_rect *insert(uint w, uint h);
		
		pack_rect **children;
		uint x, y, width, height, handle;
		int taken;
	};

	typedef boost::unordered_map<uint, pack_rect*> PackMap_t;

	struct pack_page {
		std::vector<pack_rect*> rows;
		PackMap_t pack;
	};
	std::vector<pack_page*> pages;
	
	void render(pack_rect *rect);
	void resetPackMap();
	
	uint width, height, handles;
	float wasted_pixels;
};
