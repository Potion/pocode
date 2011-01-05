#include "poCommon.h"

#ifndef _CSV_PARSER_H_
#define _CSV_PARSER_H_

class poCsvFile
{
public:
	poCsvFile();
	void parse(const char* filename);

	const char* get(int col, int row);
	const char* get(std::string colName, int row);

	int nRows, nCols;
	std::vector<std::vector<std::string> > table;
	//TODO: REVISIT
	//was std::tr1::unordered_map
	std::map<std::string, int> colNames; 
};

#endif