#include "DFObjectPos.h"

DFObjectPos::DFObjectPos(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(1);
    setOutletCount(1);

    if(args.size() < 2)
        throw std::runtime_error("too few args");

    handle_ = getObjectHandle(args[1]);
    relToHandle_ = args.size() >= 3 ? getObjectHandle(args[2]) : -1;
}

void DFObjectPos::onDataReceived(size_t inlet, DFData *data)
{
    if(DFVector *vec = dynamic_cast<DFVector*>(data))
    {
        pos_ = *vec;
        simSetObjectPosition(handle_, relToHandle_, &pos_.data[0]);
        return;
    }

    DFNode::onDataReceived(inlet, data);
}

void DFObjectPos::tick()
{
    if(-1 != simGetObjectPosition(handle_, relToHandle_, &pos_.data[0]))
    {
        sendData(0, &pos_);
    }
}
