#ifndef DFVECTORPACK_H_INCLUDED
#define DFVECTORPACK_H_INCLUDED

#include "DFNode.h"
#include "DFBang.h"
#include "DFScalar.h"
#include "DFVector.h"

class DFVectorPack : public DFNode
{
private:
    DFVector state_;

public:
    DFVectorPack(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);
};

#endif // DFVECTORPACK_H_INCLUDED
