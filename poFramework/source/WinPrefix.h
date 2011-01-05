//////////
//
//	File:		WinPrefix.h
//
//	Contains:	Prefix file for our Windows projects.
//
//	Written by:	Tim Monroe
//
//	Copyright:	© 1999 by Apple Computer, Inc., all rights reserved.
//
//	Change History (most recent first):
//	   
//	   <1>	 	11/11/99	rtm		first file
//
//////////

#pragma once

#ifndef __Prefix_File__
#define __Prefix_File__


//////////
//
// header files
//
//////////

#if !defined(_MSC_VER)
#include <Win32Headers.mch>
#else
#include <ConditionalMacros.h>
#endif


//////////
//
// compiler macros
//
//////////

#ifndef PASCAL_RTN
#define PASCAL_RTN
#endif


//////////
//
// compiler pragmas
//
//////////

// if we're being compiled by Microsoft Visual C++, turn off some warnings
#if defined(_MSC_VER) && !defined(__MWERKS__) 
	#pragma warning(disable:4068)		// ignore unknown pragmas
	#pragma warning(disable:4244)		// ignore conversion from "long" to "short", possible loss of data
	#pragma warning(disable:4761)		// ignore integral size mismatch in argument: conversion supplied
	#pragma warning(disable:4129)		// ignore 'p': unrecognized character escape sequence
	#pragma warning(disable:4229)		// ignore anachronism used: modifiers on data are ignored
#endif

#define GetDialogPort(d)				(GrafPtr)d

#endif	// __Prefix_File__