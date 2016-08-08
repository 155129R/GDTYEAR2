#include "CSVHandler.h"


CCSVHandler::CCSVHandler()
{
}


CCSVHandler::~CCSVHandler()
{
}

bool CCSVHandler::LoadFile(const string fileName)
{
	int count = 0;
   ifstream file(fileName.c_str());
	if(file.is_open())
	{
		while (!file.eof())
		{
			string currLine;


			getline(file, currLine);

			cards.push_back(CCard(currLine));
			
			count++;
		}
	}
	return true;
}