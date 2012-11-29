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
//  poResourceLoader.h
//  pocode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#pragma once


#include <map>
#include <typeinfo>
#include "poHelpers.h"
#include "poFileLocation.h"

namespace po {
    class Font;
    class BitmapFont;
    class Texture;
    
    // simple helper functions to retreive the resources you want
    Font *getFont(const FilePath &filePath, int group=0);
    Font *getFont(const FilePath &filePath, const std::string &style, int group=0);

    Font *getFontByName(const std::string &family, int group=0);
    Font *getFontByName(const std::string &family, const std::string &style, int group=0);

    BitmapFont *getBitmapFont(Font* font, uint size, int group=0);
    BitmapFont *getBitmapFont(const FilePath &filePath, uint size, int group=0);

    BitmapFont *getBitmapSystemFont(const std::string &family, const std::string &style, uint size, int group=0);

    Texture *getTexture(const FilePath &filePath, bool keepImage=false, int group=0);

    void DeleteResourceGroup(int group);
    void DeleteResourceType(const std::type_info &type);

    // base class for resources
    class Resource {
    public:
        virtual ~Resource()		{}

        void resourceMarkUsed() {
            timestamp = getElapsedTime();
        }
        
        float resourceLastUsed() {
            return timestamp;
        }
        
    private:
        float timestamp;
    };

    // helper function to retrieve the type_info of a given Resource subclass
    inline const std::type_info &ResourceType(Resource *rez) {
        return typeid(*rez);
    }
    
    // helper class to find stored resources
    struct ResourceLocator {
        // each class type should have a consisten hashing method
        size_t hash;
        // resource groups let you delete a batch
        int group;
        // type_info keeps resources distinct from eachother
        const std::type_info &type;
        
        // this is the only way to make one of these
        ResourceLocator(size_t h, int g, const std::type_info &t);
        // must define to make this usable in a std::map
        bool operator<(const ResourceLocator &rhs) const;
    };
    
    inline std::ostream &operator<<(std::ostream &out, ResourceLocator const& loc) {
        out << "resource(" << loc.hash << "," << loc.group << "," << loc.type.name() << ")";
        return out;
    }

    // a place to stash pointers to resources
    // to facilitate reuse and help managed memory
    class ResourceStore {
    public:
        // grab the singleton
        static ResourceStore *get();
        
        // build a locator to find a resource of a given type
        ResourceLocator locatorForFont(const FilePath &filePath, const std::string &style="", int group=-1);
        ResourceLocator locatorForBitmapFont(Font* font, uint size, int group=-1);
        ResourceLocator locatorForBitmapFont(const FilePath &filePath, uint size, int group=-1);
        ResourceLocator locatorForBitmapFont(const std::string &family, const std::string &style, uint size, int group=-1);
        
        ResourceLocator locatorForTexture(const FilePath &filePath, int group=-1);
        // pull up a Resource* based on locator
        Resource *findResource(ResourceLocator locator);
        // add any old Resource* subclass you'd like
        void addResource(ResourceLocator locator, Resource *resource);
        // delete a resource group
        void deleteResourceGroup(int group);
        // delete all resources of a given type
        void deleteAllResourcesOfType(const std::type_info &type);
        bool resourceIsCached(Resource* rez);
        
    private:
        ResourceStore();
        std::map<ResourceLocator, Resource*> resources;
    };
} /* End po Namespace */