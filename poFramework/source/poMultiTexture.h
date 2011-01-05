/*
 *  poMultiTexture.h
 *  poTestApp
 *
 *  Created by Young Cho on 6/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef _PO_MULTI_TEXTURE_
#define _PO_MULTI_TEXTURE_
#include "poObjectModifier.h"
#include "poTexture.h"

class poMultiTexture : public poObjectModifier
{
public:
	poMultiTexture();
	
	virtual void				setup(poObject*);
	virtual void				setdown(poObject*);
	
	void						addTexture( poTexture* t );
	
	static GLint				maxNumTextures;
	
	std::vector<poTexture*>		textureList;
	
};
#endif