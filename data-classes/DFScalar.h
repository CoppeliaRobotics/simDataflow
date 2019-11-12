#ifndef DFSCALAR_H_INCLUDED
#define DFSCALAR_H_INCLUDED

#include "DFData.h"
#include "simLib.h"
#include <string>
#include <sstream>

class DFScalar : public DFData
{
public:
    simFloat data;

    DFScalar operator=(const DFScalar &o);

    std::string str();
    std::string tag();
};

#endif // DFSCALAR_H_INCLUDED
