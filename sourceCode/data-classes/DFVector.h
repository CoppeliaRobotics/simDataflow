#ifndef DFVECTOR_H_INCLUDED
#define DFVECTOR_H_INCLUDED

#include "DFData.h"
#include "simLib/simLib.h"
#include <string>
#include <sstream>

class DFVector : public DFData
{
public:
    float data[3];

    DFVector operator=(const DFVector &o);

    std::string str();
    std::string tag();
};

#endif // DFVECTOR_H_INCLUDED
