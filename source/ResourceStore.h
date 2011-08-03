//
//  poResource.h
//  poFramework4
//
//  Created by Joshua Fisher on 4/28/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once

#define DEFAULT_REZ_GROUP -1

template <typename LoaderT>
class poResourceStore {
public:
	typedef LoaderT Loader;
	typedef typename LoaderT::Key Key;
	typedef typename LoaderT::Obj Obj;
	
	poResourceStore();
	virtual ~poResourceStore();
	
	Obj* notFound();
	Obj* get(Key k, int gid=DEFAULT_REZ_GROUP);
	
	std::set<Obj*> getGroup(int gid);
	
	void deleteResource(Key k);
	void deleteResource(Obj *o);
	void deleteGroup(int gid);
	
private:
	struct Resource {
		int gid;
		Obj *obj;
	};

	typedef std::map<Key,Resource> Resources;
	
	Loader loader;
	Resources resources;
};



// implementation, don't look
template <typename LoaderT>
poResourceStore<LoaderT>::poResourceStore() {}

template <typename LoaderT>
poResourceStore<LoaderT>::~poResourceStore() {
	typename Resources::iterator iter = resources.begin();
	for(iter; iter!=resources.end(); ++iter)
		delete iter->second.obj;
	resources.clear();
}

template <typename LoaderT>
typename LoaderT::Obj* poResourceStore<LoaderT>::notFound() {
	return &LoaderT::not_found;
}

template <typename LoaderT>
typename LoaderT::Obj* poResourceStore<LoaderT>::get(Key k, int gid) {
	if(resources.find(k) != resources.end()) {
		return resources[k].obj;
	}
	
	Obj *obj = loader.load(k);
	if(!obj)
		return NULL;
	
	Resource rez;
	rez.obj = obj;
	rez.gid = gid;
	resources[k] = rez;
	
	return rez.obj;
}

template <typename LoaderT>
std::set<typename LoaderT::Obj*> poResourceStore<LoaderT>::getGroup(int gid) {
	std::set<Obj*> response;
	typename Resources::iterator iter = resources.begin();
	for(iter; iter!=resources.end(); ++iter)
		if(iter->second.gid == gid)
			response.insert(response);
	return response;
}

template <typename LoaderT>
void poResourceStore<LoaderT>::deleteResource(Key k) {
	if(resources.find(k) != resources.end()) {
		delete resources[k].obj;
		resources.erase(k);
	}
}

template <typename LoaderT>
void poResourceStore<LoaderT>::deleteResource(Obj *obj) {
	typename Resources::iterator iter = resources.begin();
	for(iter; iter!=resources.end(); ++iter)
		if(iter->second.obj == obj) {
			delete iter->second.obj;
			resources.erase(iter++);
			break;
		}
}

template <typename LoaderT>
void poResourceStore<LoaderT>::deleteGroup(int gid)	{
	typename Resources::iterator iter = resources.begin();
	while(iter != resources.end()) {
		if(iter->second.gid == gid) {
			delete iter->second.obj;
			resources.erase(iter++);
		}
		else 
			++iter;
	}
}

