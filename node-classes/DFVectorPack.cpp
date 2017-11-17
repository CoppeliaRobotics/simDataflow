#include "DFVectorPack.h"

DFVectorPack::DFVectorPack(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(3);
    setOutletCount(1);

    if(args.size() != 1 && args.size() != 4)
        throw std::runtime_error("bad arg count");

    for(int i = 0; i < 3; i++)
        state_.data[i] = (i + 1) < args.size() ? boost::lexical_cast<simInt>(args[i + 1]) : 0;
}

void DFVectorPack::onDataReceived(size_t inlet, DFData *data)
{
    if(DFScalar *sca = dynamic_cast<DFScalar*>(data))
    {
        state_.data[inlet] = sca->data;
        if(inlet == 0) sendData(0, &state_);
        return;
    }
    if(DFVector *vec = dynamic_cast<DFVector*>(data))
    {
        if(inlet == 0)
        {
            state_ = *vec;
            sendData(0, &state_);
            return;
        }
    }

    DFNode::onDataReceived(inlet, data);
}

