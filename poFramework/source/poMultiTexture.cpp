/*
 *  poMultiTexture.cpp
 *  poTestApp
 *
 *  Created by Young Cho on 6/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "poMultiTexture.h"

GLint poMultiTexture::maxNumTextures = -1;


poMultiTexture::poMultiTexture()
{
	if( poMultiTexture::maxNumTextures == -1 )
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS, &poMultiTexture::maxNumTextures);
	}
}



void poMultiTexture::addTexture( poTexture* t )
{
	if( textureList.size() > poMultiTexture::maxNumTextures ){
		printf("error:poMultiTexture::addTexture: exceed maximum number of textures\n");
		return;
	}
	textureList.push_back( t );
}


void poMultiTexture::setup( poObject* obj )
{
	for( int i=0; i< textureList.size(); i++ )
	{
		if( i==0 ) glActiveTextureARB( GL_TEXTURE0_ARB );
		else if( i==1 ) glActiveTextureARB( GL_TEXTURE1_ARB );
		else if( i==2 ) glActiveTextureARB( GL_TEXTURE2_ARB );
		else if( i==3 ) glActiveTextureARB( GL_TEXTURE3_ARB );
		else if( i==4 ) glActiveTextureARB( GL_TEXTURE4_ARB );
		else if( i==5 ) glActiveTextureARB( GL_TEXTURE5_ARB );
		else if( i==6 ) glActiveTextureARB( GL_TEXTURE6_ARB );
		else if( i==7 ) glActiveTextureARB( GL_TEXTURE7_ARB );
		glEnable( GL_TEXTURE_RECTANGLE_EXT );
		glBindTexture( GL_TEXTURE_RECTANGLE_EXT, textureList[i]->bindID );
	}
}

void poMultiTexture::setdown( poObject* obj )
{
	for( int i=textureList.size()-1; i>=0; i-- )
	{
		if( i==0 ) glActiveTextureARB( GL_TEXTURE0_ARB );
		else if( i==1 ) glActiveTextureARB( GL_TEXTURE1_ARB );
		else if( i==2 ) glActiveTextureARB( GL_TEXTURE2_ARB );
		else if( i==3 ) glActiveTextureARB( GL_TEXTURE3_ARB );		
		else if( i==4 ) glActiveTextureARB( GL_TEXTURE4_ARB );
		else if( i==5 ) glActiveTextureARB( GL_TEXTURE5_ARB );
		else if( i==6 ) glActiveTextureARB( GL_TEXTURE6_ARB );
		else if( i==7 ) glActiveTextureARB( GL_TEXTURE7_ARB );
		glDisable(GL_TEXTURE_RECTANGLE_EXT);
	}
}