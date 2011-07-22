#include "poResourceStore.h"

poResourceStore<poFontLoader> *fontStore() {
	static poResourceStore<poFontLoader> *store = NULL;
	if(!store)
		store = new poResourceStore<poFontLoader>();
	return store;
}

poResourceStore<poImageLoader>* imageStore() {
	static poResourceStore<poImageLoader> *store = NULL;
	if(!store)
		store = new poResourceStore<poImageLoader>();
	return store;
}



poFont *getFont(const std::string &n, int s, const std::string &w, int g) {
	return fontStore()->get(poFontSpec(n,s,w),g);
}

poImage *getImage(const std::string &n, int g) {
	return imageStore()->get(poImageSpec(n),g);
}

poImage *getImage(const std::string &n, ImageBitDepth b, int g) {
	return imageStore()->get(poImageSpec(n,b),g);
}

poImage *getImage(const std::string &n, uint w, uint h, ImageBitDepth b, const ubyte *p, int g) {
	return imageStore()->get(poImageSpec(n,w,h,b,p),g);
}