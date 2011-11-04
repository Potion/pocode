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
	BinPacker(uint width, uint height, uint padding=0);
	~BinPacker();

	void reset();
	uint addRect(poRect r);

	void pack();
	void render();

	int getNumPages() const;
	float getWastedPixels() const;
	poRect packPosition(uint handle, uint *page=NULL);

private:
	BinPacker(const BinPacker &lhs) {}
	BinPacker &operator=(const BinPacker &lhs) {return *this;}

	struct insertRect {
		insertRect(uint w, uint h, uint handle);
		bool operator<(const insertRect &r);
		uint w, h, handle;
	};
	std::list<insertRect> rectangles;

	struct packRect {
		packRect();
		packRect(uint w, uint h);
		~packRect();
		void setDims(uint x, uint y, uint w, uint h);
		packRect *insert(uint w, uint h);
		
		packRect **children;
		uint x, y, width, height, handle;
		int taken;
	};

	typedef boost::unordered_map<uint, packRect*> PackMap_t;

	struct pack_page {
		std::vector<packRect*> rows;
		PackMap_t pack;
	};
	std::vector<pack_page*> pages;
	
	void render(packRect *rect);
	void resetPackMap();
	
	uint width, height, padding, handles;
	float wastedPixels;
};
