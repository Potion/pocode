//
//  poResource.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

class poResource {
public:
	virtual ~poResource() {}
};

class poResourceStore {
public:
	poResourceStore();
	~poResourceStore();
	
	// don't have to specify template argument
	template <typename T> T* add(T *r);
	template <typename T> T* remove(T *r);
	
private:
	std::set<poResource*> resources;
};



// template implementations
template <typename T>
T* poResourceStore::add(T *r) {
	resources.insert(r);
	return r;
}

template <typename T>
T* poResourceStore::remove(T *r) {
	resources.erase(r);
	return r;
}
