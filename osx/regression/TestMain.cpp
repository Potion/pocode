//
//  TestMain.cpp
//  pocode
//
//  Created by filippo on 10/18/11.
//  Copyright 2011 Potion Design. All rights reserved.
//

#define BOOST_TEST_MODULE pocode Test Suite
#include <boost/test/unit_test.hpp>

#include "poWindow.h"

poObject*	createObjectForID(uint id);
int			applicationNumberWindows();
poWindow*	applicationCreateWindow(uint, poWindowType, const char*, int, int, int, int);
poWindow*	applicationGetWindow(int index);
poWindow*	applicationCurrentWindow();
std::string	applicationGetSupportDirectory();
void		applicationMakeWindowCurrent(poWindow*);
void		applicationMakeWindowFullscreen(poWindow*,bool);
void		applicationMoveWindow(poWindow*,poPoint);
void		applicationReshapeWindow(poWindow*,poRect);
void        applicationQuit();

poObject*	createObjectForID(uint id)						{return NULL;}
int			applicationNumberWindows()						{return 0;}
poWindow*	applicationCreateWindow(uint, poWindowType, const char*, int, int, int, int) {return NULL;}
poWindow*	applicationGetWindow(int index)					{return NULL;}
poWindow*	applicationCurrentWindow()						{return NULL;}
std::string	applicationGetSupportDirectory()				{return "";}
void		applicationMakeWindowCurrent(poWindow*)			{}
void		applicationMakeWindowFullscreen(poWindow*,bool)	{}
void		applicationMoveWindow(poWindow*,poPoint)		{}
void		applicationReshapeWindow(poWindow*,poRect)		{}
void        applicationQuit()								{}
