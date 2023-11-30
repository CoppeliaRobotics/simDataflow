#include "DFString.h"

DFString DFString::operator=(const DFString &o)
{
    data = o.data;
    return *this;
}

std::string DFString::str()
{
    return data;
}

std::string DFString::tag()
{
    return "string";
}
