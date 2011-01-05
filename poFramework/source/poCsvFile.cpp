#include "poCsvFile.h"
#include "boost/algorithm/string.hpp"

poCsvFile::poCsvFile()
: nRows(0), nCols(0)
{}

void poCsvFile::parse(const char* filename)
{
	using namespace std;
	using namespace boost::algorithm;
	
	ifstream csv(filename);
	if(!csv.is_open())
	{
		printf("ERROR: csv->parse: can't open %s\n", filename);
		exit(-1);
	}

	bool firstLine = true;
	string line;
	while(!csv.eof())
	{
		getline(csv, line, '\n');
		if(line == "")
			continue;
			
		vector<string> rez;
		string current;
		bool insideQuote = false;
		for(int i=0; i<line.size(); i++)
		{
			if(line[i] == '"') 
				insideQuote = !insideQuote;
			else if(line[i] == ',' && !insideQuote)
			{
				rez.push_back(current);
				current.clear();
			} else
				current += line[i];
		}
		rez.push_back(current);
		if(firstLine)
		{
			nCols = rez.size();
			firstLine = false;
			
			for(int i=0; i<rez.size(); i++) 
//				colNames.insert(std::pair<string, int>(rez[i], i));
				colNames[rez[i]] = i;
			
			continue;
		} else
		{
			if(rez.size() > nCols)
			{}//	printf("WARNING: csv->parse: malformed csv, row %d has too many entries (%d / %d)\n", nRows, rez.size(), nCols);
			else if(rez.size() < nCols)
			{
				printf("ERROR: csv->parse: malformed csv, row %d has too few entries (%d / %d)\n", nRows, rez.size(), nCols);
				exit(-1);
			}
		}

		table.push_back(rez);
		nRows += 1;
	}
}

const char* poCsvFile::get(int col, int row)
{
	if(col >= nCols || row >= nRows)
	{
		printf("WARNING: csv->get: %d %d out of bounds (%d %d)\n", col, row, nCols, nRows);
		return NULL;
	}
	
	return table[row][col].c_str();
}

const char* poCsvFile::get(std::string colName, int row)
{
	//std::tr1::unordered_map<std::string, int>::iterator column = colNames.find(colName);
	std::map<std::string, int>::iterator column = colNames.find(colName);
	if(column == colNames.end())
	{
		printf("WARNING: csv->get: column %s doesn't exist\n", colName.c_str());
		return NULL;
	}
	if(row >= nRows)
	{
		printf("WARNING: csv->get: %d %d out of bounds (%d %d)\n", colNames[colName], row, nCols, nRows);
		return NULL;
	}
	
	return table[row][colNames[colName]].c_str();
}
