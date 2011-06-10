#include "TestApp.h"

#include "Helpers.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"
#include "poShapeBasics2D.h"
#include "BinPacker.h"

struct poTextureAtlas {
	poTextureAtlas(uint w, uint h)
	:	width(w)
	,	height(h)
	{}
	~poTextureAtlas() {
		clearImages();
		clearPages();
	}
	
	void clearImages() {
		BOOST_FOREACH(poImage *img, images) {
			delete img;
		}
	}
	
	uint addImage(poImage *img) {
		images.push_back(img->copy());
	}
	
	void layoutAtlas() {
		BinPacker pack(512,512);
		map<int,int> pack_ids;
	}

private:
	void clearPages() {
		BOOST_FOREACH(poTexture* tex, textures) {
			delete tex;
		}
	}
	
	struct Item {
		uint page;
		poRect coords;
	};
	
	int width, height;
	map<int,Item> items;
	vector<poImage*> images;
	vector<poTexture*> pages;
};

using namespace std;

poObject *createObjectForID(uint uid) {
	return new TestObj();
}

void setupApplication() {
	applicationCreateWindow(0, WINDOW_TYPE_NORMAL, "TestObj", 100, 100, 1050, 600);
}

void cleanupApplication() {
}

TestObj::TestObj() {
    addModifier(new poCamera2D());
	
	poFont font("Lucida Grande", PO_FONT_ITALIC);
	if(font.valid()) {
		font.pointSize(125);

		double now = getTime();


		
		now = getTime();
		std::vector<poImage*> images;
		for(int i=0; i<pack.numPages(); i++) {
			images.push_back(new poImage(512,512,IMAGE_8,NULL));
		}
		
		textures.resize(pack.numPages());

		map<int,int>::iterator iter = pack_ids.begin();
		while(iter != pack_ids.end()) {
			int glyph_id = iter->first;
			int pack_id = iter->second;
			iter++;

			uint pg;
			poRect pack_loc = pack.packPosition(pack_id, &pg);

			Item item;
			item.page = pg;
			
			item.coords = poRect(pack_loc.origin/512.f, pack_loc.size/512.f);
			item.coords.origin.y = 1.f - item.coords.origin.y - item.coords.size.y;
			
			item.location = pack_loc;
			items[glyph_id] = item;
			
			font.glyph(glyph_id);
			images[pg]->composite(font.glyphImage(), pack_loc);
		}
		printf("composited in %f seconds\n", getTime()-now);
		
		for(int i=0; i<images.size(); i++) {
			poImage *img = images[i];
			textures[i] = new poTexture(GL_ALPHA, img->width(), img->height(), img->storageSize(), img->pixels());
			delete img;
		}
		images.clear();
		
		addEvent(PO_KEY_DOWN_EVENT, this);
	}
}

void TestObj::draw() {
}

void TestObj::update() {
}

void TestObj::eventHandler(poEvent *event) {
}


