#ifndef DFOBJECTPOS_H_INCLUDED
#define DFOBJECTPOS_H_INCLUDED

#include "DFNode.h"
#include "DFVector.h"

class DFObjectPos : public DFNode
{
private:
    simInt handle_, relToHandle_;
    DFVector pos_;

public:
    DFObjectPos(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);
    void tick();
};

#endif // DFOBJECTPOS_H_INCLUDED
