#include "DFScalar.h"

DFScalar DFScalar::operator=(const DFScalar &o)
{
    data = o.data;
    return *this;
}

std::string DFScalar::str()
{
    return boost::lexical_cast<std::string>(data);
}

std::string DFScalar::tag()
{
    return "scalar";
}
