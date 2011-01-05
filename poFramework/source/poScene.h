/*
 *  poScene.h
 *  poFramework3
 *
 *  Created by Jared Schiffman on 4/1/08.
 *  Copyright 2008 Potion Design LLC. All rights reserved.
 *
 */

#include "poObject.h"


#ifndef PO_SCENE_H
#define PO_SCENE_H


class poScene : public poObject
{
public:
	poScene();

	virtual poObject*	duplicate( bool copyChildren=false, poObject* copyIntoObject=NULL );
	virtual const char*	getClassName() { return "poScene"; };
	
	virtual void		init();
	virtual void		update();
	
	void				fullInstantUpdate();
	
	
	int		numFrames;
	int		currentFrame;
};


class poSceneShot : public poObjectModifier
{
public:
	poSceneShot();

	virtual const char*			getClassName() { return "poSceneShot"; };
	virtual poObjectModifier*	duplicate() { return new poSceneShot(); };
	
	virtual void			init();
	virtual void			initWithObject( poObject* obj );

	virtual void setup(poObject* obj);
	virtual void setdown(poObject* obj);
	
	virtual poXMLnode*		write();
	virtual void			read(poXMLnode* node);
	
	void					captureState( poObject* obj );
	
	int						currentFrame;
	poObjectPropertyVector	objectPropertySet;
};


#endif