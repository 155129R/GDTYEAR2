#include "CSVHandler.h"


CCSVHandler::CCSVHandler()
{
}


CCSVHandler::~CCSVHandler()
{
}

bool CCSVHandler::LoadFile(const string fileName)
{
    int lineCount = 0;
    int theMaxNumOfColumns = 0;
   ifstream file(fileName.c_str());
	if(file.is_open())
	{
		int i = 0, k = 0;
		while(file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);



			// If this line is not a comment line, then process it
			if(!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (lineCount == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}

				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while(getline(iss, token, ',') && (theColumnCounter<theMaxNumOfColumns))
					{
						//theScreenMap[theLineCounter][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			lineCount++;
		}
	}
	return true;
}