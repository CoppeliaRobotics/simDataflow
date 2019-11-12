#ifndef DFBANG_H_INCLUDED
#define DFBANG_H_INCLUDED

#include "DFData.h"
#include "simLib.h"
#include <string>

class DFBang : public DFData
{
public:
    std::string str();
    std::string tag();
};

#endif // DFBANG_H_INCLUDED
