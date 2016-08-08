#ifndef CCSVHANDLER_H
#define CCSVHANDLER_H

#include <string>
#include <fstream>
#include <sstream>

using std::istringstream;
using std::ifstream;
using std::string;

class CCSVHandler
{
public:
    CCSVHandler();
    virtual ~CCSVHandler();

    bool LoadFile(const string fileName);
};

#endif /*CCSVHANDLER_H*/