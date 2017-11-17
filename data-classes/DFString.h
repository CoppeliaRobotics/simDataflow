#ifndef DFSTRING_H_INCLUDED
#define DFSTRING_H_INCLUDED

#include "DFData.h"
#include "v_repLib.h"
#include <string>
#include <sstream>

class DFString : public DFData
{
public:
    std::string data;

    DFString operator=(const DFString &o);

    std::string str();
    std::string tag();
};

#endif // DFSTRING_H_INCLUDED
