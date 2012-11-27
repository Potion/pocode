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

#pragma once

#include "poRect.h"

#include <map>
#include <list>

/*	addRect with the image dimensions, get back a handle
 *	use the handle to find where to put the image into digest
 *
 *	sorts by height, creates a "line" with the tallest rectangle
 *	then fills the leftover out to a set width
 *	once a line is full, it starts a new one
 *	bounded by width and height, will start a new page if it runs out of height
 */
namespace po {
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

        typedef std::map<uint, packRect*> PackMap_t;

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
} /* End po Namespace */