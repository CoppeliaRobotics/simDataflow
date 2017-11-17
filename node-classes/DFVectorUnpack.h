#ifndef DFVECTORUNPACK_H_INCLUDED
#define DFVECTORUNPACK_H_INCLUDED

#include "DFNode.h"
#include "DFVector.h"

class DFVectorUnpack : public DFNode
{
private:

public:
    DFVectorUnpack(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);
};

#endif // DFVECTORUNPACK_H_INCLUDED
