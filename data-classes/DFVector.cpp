#include "DFVector.h"

DFVector DFVector::operator=(const DFVector &o)
{
    for(int i = 0; i < 3; i++)
        data[i] = o.data[i];
    return *this;
}

std::string DFVector::str()
{
    return (boost::format("{%f, %f, %f}") % data[0] % data[1] % data[2]).str();
}

std::string DFVector::tag()
{
    return "vector";
}
