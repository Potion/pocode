#pragma once

#include "poDictionary.h"

class poCommon
:	public poDictionary
{
public:
	static poCommon *get();
	
	void read(const fs::path &url);
	void write(const fs::path &url);
	
private:
	explicit poCommon() {}
	~poCommon() {}
};