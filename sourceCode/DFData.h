#ifndef DFDATA_H_INCLUDED
#define DFDATA_H_INCLUDED

#include <string>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

class DFData
{
public:
    virtual std::string str() = 0;
    virtual std::string tag() = 0;
};

#endif // DFDATA_H_INCLUDED
