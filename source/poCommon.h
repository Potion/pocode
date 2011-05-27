#pragma once

#include "poDictionary.h"

class poCommon
:	public poDictionary
{
public:
	static poCommon *get();
	
private:
	explicit poCommon() {}
	~poCommon() {}
};