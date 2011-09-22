#include "BinPacker.h"

BinPacker::insert_rect::insert_rect(uint w, uint h, uint handle) : w(w), h(h), handle(handle) {}
bool BinPacker::insert_rect::operator<(const insert_rect &r) {
	if(w == r.w)
		return w > r.w;
	return h > r.h;
}

BinPacker::pack_rect::pack_rect() : x(0), y(0), width(0), height(0), handle(0), children(NULL), taken(false) {}
BinPacker::pack_rect::pack_rect(uint w, uint h) : x(0), y(0), width(w), height(h), handle(0), children(NULL), taken(false) {}
BinPacker::pack_rect::~pack_rect() {
	if(NULL != children) {
		delete children[0];
		delete children[1];
		delete [] children;
	}
}
void BinPacker::pack_rect::setDims(uint x, uint y, uint w, uint h) {
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}
// insert dimensions of the image, recursively
BinPacker::pack_rect *BinPacker::pack_rect::insert(uint w, uint h) {
	if(taken)
		return NULL;
	
	// if we're a branch, pass image onto children
	if(NULL != children) {
		pack_rect *rect = children[0]->insert(w,h);
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
	children = new pack_rect*[2];
	children[0] = new pack_rect();
	children[1] = new pack_rect();

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
		BOOST_FOREACH(pack_rect* row, page->rows) {
			delete row;
		}
		page->rows.clear();
		page->pack.clear();
		delete page;
	}
	pages.clear();

	wasted_pixels = 0.f;
}

uint BinPacker::addRect(poRect r) {
	uint tmp = handles++;
	rectangles.push_back(insert_rect(r.width, r.height, tmp));
	return tmp;
}

int BinPacker::numPages() const {return pages.size();}
float BinPacker::wastedPixels() const {return wasted_pixels;}

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
	std::list<insert_rect>::iterator i, e;
//	for(i=rectangles.begin(),e=rectangles.end(); i!=e; i++)
//		width = std::max(width, i->w);

	std::list<insert_rect> unused(rectangles.begin(),rectangles.end());

	uint insert_height=0, count=0, used_area=0;

	pack_page *page = new pack_page();
	pages.push_back(page);

	while(!unused.empty()) {
		// save this new row's height
		uint row_height = unused.front().h + padding;

		// we've run out of space on this page
		if(insert_height + row_height >= height) {
			// print stats
			wasted_pixels += sqrtf(float(width*height-used_area));
			// and make the new page
			page = new pack_page();
			pages.push_back(page);
			insert_height = used_area = count = 0;
		}

		// start a new row
		pack_rect *row = new pack_rect();
		row->setDims(0,insert_height,width,row_height);
		page->rows.push_back(row);

		// go through all remaining rectangles and see who fits in the row
		i=unused.begin(), e=unused.end();
		while(i != e) {
			insert_rect rect = *i;
			pack_rect *packed = row->insert(rect.w+padding, rect.h+padding);
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

void BinPacker::render(BinPacker::pack_rect *r) {
//	if(NULL != r->children) {
//		render(r->children[0]);
//		render(r->children[1]);
//	}
//	else {
//		bool is_filled = r->handle != 0;
//		if(is_filled)
//			glColor4f(1.f,.5f,.5f,1);
//		else
//			glColor4f(.5f,.5f,.5f,1);
//		glRecti(r->x+1,r->y+1,r->x+r->width-2,r->y+r->height-2);
//	}
}

#include "poMatrixStack.h"
void BinPacker::render() {
//	poMatrixStack::get()->pushModelview();
//	
//	BOOST_FOREACH(pack_page *page, pages) {
//		glColor4f(1.f,1.f,1.f,1);
//		glRecti(0,0,width,height);
//
//		BOOST_FOREACH(pack_rect *rect, page->rows) {
//			render(rect);
//		}
//
//		poMatrixStack::get()->translate(poPoint(width, 0, 0));
//	}
//	
//	poMatrixStack::get()->popModelview();
}



