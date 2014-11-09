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
//  pocode
//
//  Created by Joshua Fisher on 10/11/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#include "poResourceStore.h"
#include "poFont.h"
#include "poTexture.h"
#include "poBitmapFont.h"
#include <boost/functional/hash.hpp>

namespace po { 
    //------------------------------------------------------------------
    Font *getFont(const FilePath &filePath, int group) {
        return getFont(filePath, "", group);
    }
    
    
    //------------------------------------------------------------------
    Font *getFont(const FilePath &filePath, const std::string &style, int group) {
        ResourceStore *store = ResourceStore::get();
        ResourceLocator lookup = store->locatorForFont(filePath, style, group);
        Resource *found = store->findResource(lookup);
        if(found)
            return (Font*)found;
        
        Font *font = new Font(filePath, style);
        store->addResource(lookup, font);
        return font;
    }
    
    
    //------------------------------------------------------------------
    Font *getFontByName(const std::string &family, int group) {
        return getFontByName(family, "", group);
    }
    
    
    //------------------------------------------------------------------
    Font *getFontByName(const std::string &family, const std::string &style, int group) {
        FilePath path;
        urlForFontFamilyName(family, style, path);
        
        return getFont(path, "", group);
    }
    
    
    //------------------------------------------------------------------
    BitmapFont *getBitmapFont(const FilePath &filePath, uint size, int group) {
        ResourceStore *store = ResourceStore::get();
        ResourceLocator lookup = store->locatorForBitmapFont(filePath, size, group);
        Resource *found = store->findResource(lookup);
        if(found)
            return (BitmapFont*)found;
        
        BitmapFont* bmpFont = new BitmapFont(filePath, size);
        store->addResource(lookup, bmpFont);
        return bmpFont;
    }
    
    
    //------------------------------------------------------------------
    BitmapFont *getBitmapFont(Font* font, uint size, int group) {
        return getBitmapFont(font->getFilePath(), size, group);
    }
    
    
    //------------------------------------------------------------------
    BitmapFont *getBitmapSystemFont(const std::string &family, const std::string &style, uint size, int group) {
        ResourceStore *store = ResourceStore::get();
        ResourceLocator lookup = store->locatorForBitmapFont(family, style, size, group);
        Resource *found = store->findResource(lookup);
        if(found)
            return (BitmapFont*)found;

        BitmapFont* bmpFont = new BitmapFont(family, style, size);
        store->addResource(lookup, bmpFont);
        return bmpFont;
    }
    
    
    //------------------------------------------------------------------
    Texture *getTexture(const FilePath &filePath, bool keepImage, int group) {
        ResourceStore *store = ResourceStore::get();
        ResourceLocator lookup = store->locatorForTexture(filePath);
        Resource *found = store->findResource(lookup);
        if(found)
            return (Texture*)found;
        
        Texture *tex = new Texture(filePath,keepImage);
        store->addResource(lookup, tex);
        return tex;
    }
    
    
    //------------------------------------------------------------------
    void deleteResourceGroup(int group);
    void deleteResourceType(const std::type_info &type);
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: ResourceLocator ===========================
    #pragma mark - ResourceLocator -

    //------------------------------------------------------------------
    // this is the only way to make one of these
    ResourceLocator::ResourceLocator(size_t h, int g, const std::type_info &t)
    :	hash(h)
    ,	group(g)
    ,	type(t)
    {
        boost::hash_combine(hash, group);
        boost::hash_combine(hash, type.name());
    }
    
    
    //------------------------------------------------------------------
    // must define to make this usable in a std::map
    bool ResourceLocator::operator<(const ResourceLocator &rhs) const {
        return hash < rhs.hash;
    }

    static boost::hash<std::string> string_hasher;
    
    
    
    
    // -----------------------------------------------------------------------------------
    // ================================ Class: ResourceStore =============================
    #pragma mark - ResourceStore -

    ResourceStore *ResourceStore::get() {
        static ResourceStore *instance = NULL;
        if(!instance)
            instance = new ResourceStore;
        return instance;
    }

    ResourceStore::ResourceStore() {
    }
    
    
    //------------------------------------------------------------------
    ResourceLocator ResourceStore::locatorForFont(const FilePath &filePath, const std::string &style, int group) {
        ResourceLocator lookup(0, group, typeid(Font));
        lookup.hash = string_hasher(filePath.toString() + style);
        return lookup;
    }
    
    
    //------------------------------------------------------------------
    ResourceLocator ResourceStore::locatorForBitmapFont(const FilePath &filePath, uint size, int group) {
        return locatorForBitmapFont(getFont(filePath, group), size, group);
    }
    
    
    //------------------------------------------------------------------
    ResourceLocator ResourceStore::locatorForBitmapFont(Font* font, uint size, int group) {
        return locatorForBitmapFont(font->getFamilyName(), font->getStyleName(), size, group);
    }
    
    
    //------------------------------------------------------------------
    ResourceLocator ResourceStore::locatorForBitmapFont(const std::string &family, const std::string &style, uint size, int group) {
        ResourceLocator lookup(0, group, typeid(BitmapFont));
        boost::hash_combine(lookup.hash, family + style);
        boost::hash_combine(lookup.hash, size);
        return lookup;
    }
    
    
    //------------------------------------------------------------------
    ResourceLocator ResourceStore::locatorForTexture(const FilePath &filePath, int group) {
        ResourceLocator lookup(0, group, typeid(Texture));
        lookup.hash = string_hasher(filePath.toString());
        return lookup;
    }
    
    void ResourceStore::deleteSingleResource(Resource* r){
        std::map<ResourceLocator,Resource*>::iterator iter = resources.begin();
        while(iter != resources.end()) {
            printf("comparing %p to\nsecond %p\tfirst\n" , r , iter->second);
            if(iter->second == r) {
                delete iter->second;
                resources.erase(iter++);
            }
            else
                iter++;
        }
    }
    
    //------------------------------------------------------------------
    void ResourceStore::deleteResourceGroup(int group) {
        std::map<ResourceLocator,Resource*>::iterator iter = resources.begin();
        while(iter != resources.end()) {
            if(iter->first.group == group) {
                delete iter->second;
                resources.erase(iter++);
            }
            else
                iter++;
        }
    }
    
    
    //------------------------------------------------------------------
    void ResourceStore::deleteAllResourcesOfType(const std::type_info &type) {
        std::map<ResourceLocator,Resource*>::iterator iter = resources.begin();
        while(iter != resources.end()) {
            if(iter->first.type == type) {
                delete iter->second;
                resources.erase(iter++);
            }
            else
                iter++;
        }
    }
    
    
    //------------------------------------------------------------------
    Resource *ResourceStore::findResource(ResourceLocator locator) {
        std::map<ResourceLocator, Resource*>::iterator iter = resources.find(locator);
        if(iter != resources.end())
            return iter->second;
        return NULL;
    }
    
    
    //------------------------------------------------------------------
    void ResourceStore::addResource(ResourceLocator locator, Resource *resource) {
        if(!findResource(locator)) {
            resources[locator] = resource;
        }
    }
    
    
    //------------------------------------------------------------------
    bool ResourceStore::resourceIsCached(Resource *rez) {
        std::map<ResourceLocator, Resource*>::iterator iter = resources.begin();
        while(iter != resources.end()) {
            if(iter->second == rez)
                return true;
            ++iter;
        }
        return false;
        
    }
}/* End po namespace */