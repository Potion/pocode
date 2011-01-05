/*
*  poScene.cpp
*  poFramework3
*
*  Created by Jared Schiffman on 4/1/08.
*  Copyright 2008 Potion Design LLC. All rights reserved.
*
*/

#include "poScene.h"


// ====================== poScene ====================== //

poScene::poScene()
{
	numFrames = 1;
	currentFrame = 0;
	
	registry.addSeparator( getClassName() );
	registry.add("numFrames", &numFrames);
	registry.add("currentFrame", &currentFrame);
}

poObject*	poScene::duplicate( bool copyChildren, poObject* copyIntoObject )
{
	poScene* obj = (poScene*) copyIntoObject;
	if ( obj==NULL )
		obj = new poScene();
	
	obj->numFrames = numFrames;
	obj->currentFrame = currentFrame;
	
	poObject::duplicate( copyChildren, obj );
	return obj;
}

void		poScene::init()
{
}

void		poScene::update()
{
	if ( currentFrame >= numFrames )
		currentFrame = numFrames-1;

	for( int i=0; i<numChildren(); i++ )
	{
		poObject*		childObj = getChild(i);
		poSceneShot*	objShot = (poSceneShot*) childObj->getModifier( "poSceneShot" );
		if ( objShot == NULL )
		{
			objShot = new poSceneShot();
			childObj->addModifier( objShot );
		}
		
		if ( objShot->objectPropertySet.size() < numFrames )
			objShot->objectPropertySet.resize( numFrames );
		
		objShot->currentFrame = currentFrame;
	}
}

void	poScene::fullInstantUpdate()
{
	update();
	for( int i=0; i<numChildren(); i++ )
	{
		poObject*		childObj = getChild(i);
		poSceneShot*	objShot = (poSceneShot*) childObj->getModifier( "poSceneShot" );
		objShot->setup( childObj );
	}
}

// ====================== poSceneShot ====================== //

poSceneShot::poSceneShot()
{
	currentFrame = 0;
	
	registry.addSeparator( getClassName() );
}

void poSceneShot::init()
{
}

void poSceneShot::initWithObject( poObject* obj )
{

}

void poSceneShot::setup(poObject* obj)
{
	if ( currentFrame >= objectPropertySet.size() )
	{
		printf("ERROR: poSceneShot::setup - currentFrame is greater than number of frames\n" );
		return;
	}
	
	obj->setProperties( objectPropertySet[ currentFrame ] );
}

void poSceneShot::setdown(poObject* obj )
{
}

poXMLnode* poSceneShot::write()
{
	poXMLnode* node = poObjectModifier::write();
	
	poXMLnode* objPropSetNode = new poXMLnode("SCENE_PROP_SET");
	for( int i=0; i<objectPropertySet.size(); i++ )
	{
		poXMLnode* objPropNode = new poXMLnode("OBJECT_PROP");
		objPropSetNode->addChild( objPropNode );
		
		objPropNode->addChild( new poXMLnode("visible", (int)objectPropertySet[i].visible ) );
		objPropNode->addChild( new poXMLnode("alpha", objectPropertySet[i].alpha ) );
		
		objPropNode->addChild( new poXMLnode("pos_x", objectPropertySet[i].position.x ) );
		objPropNode->addChild( new poXMLnode("pos_y", objectPropertySet[i].position.y ) );
		objPropNode->addChild( new poXMLnode("pos_z", objectPropertySet[i].position.z ) );
		
		objPropNode->addChild( new poXMLnode("sca_x", objectPropertySet[i].scale.x ) );
		objPropNode->addChild( new poXMLnode("sca_y", objectPropertySet[i].scale.y ) );
		objPropNode->addChild( new poXMLnode("sca_z", objectPropertySet[i].scale.z ) );
		
		objPropNode->addChild( new poXMLnode("rot_x", objectPropertySet[i].rotationAxis.x ) );
		objPropNode->addChild( new poXMLnode("rot_y", objectPropertySet[i].rotationAxis.y ) );
		objPropNode->addChild( new poXMLnode("rot_z", objectPropertySet[i].rotationAxis.z ) );
		
		objPropNode->addChild( new poXMLnode("rotation", objectPropertySet[i].rotation ) );
		
		
	}
	node->addChild( objPropSetNode );
	
	return node;
}

void poSceneShot::read(poXMLnode* node)
{
	poObjectModifier::read( node );
	
	poXMLnode* objPropSetNode = node->getNode( "SCENE_PROP_SET" );
	objectPropertySet.resize( objPropSetNode->numChildren );
	
	for( int i=0; i<objPropSetNode->numChildren; i++ )
	{
		poXMLnode* objPropNode = objPropSetNode->getNode( "OBJECT_PROP", i );
		
		objectPropertySet[i].visible = objPropNode->getNodeAsInt( "visible" );
		objectPropertySet[i].alpha = objPropNode->getNodeAsFloat( "alpha" );
		
		objectPropertySet[i].position.x = objPropNode->getNodeAsFloat( "pos_x" );
		objectPropertySet[i].position.y = objPropNode->getNodeAsFloat( "pos_y" );
		objectPropertySet[i].position.z = objPropNode->getNodeAsFloat( "pos_z" );
		
		objectPropertySet[i].scale.x = objPropNode->getNodeAsFloat( "sca_x" );
		objectPropertySet[i].scale.y = objPropNode->getNodeAsFloat( "sca_y" );
		objectPropertySet[i].scale.z = objPropNode->getNodeAsFloat( "sca_z" );
		
		objectPropertySet[i].rotationAxis.x = objPropNode->getNodeAsFloat( "rot_x" );
		objectPropertySet[i].rotationAxis.y = objPropNode->getNodeAsFloat( "rot_y" );
		objectPropertySet[i].rotationAxis.z = objPropNode->getNodeAsFloat( "rot_z" );
		
		objectPropertySet[i].rotation = objPropNode->getNodeAsFloat( "rotation" );
	}

}

void poSceneShot::captureState( poObject* obj )
{
	if ( currentFrame >= objectPropertySet.size() )
	{
		printf("ERROR: poSceneShot::captureState - currentFrame is greater than number of frames\n" );
		return;
	}
	
	objectPropertySet[ currentFrame ] = obj->getProperties();
}

