#include "DFVectorUnpack.h"
#include "DFScalar.h"

DFVectorUnpack::DFVectorUnpack(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(1);
    setOutletCount(3);

    if(args.size() != 1)
        throw std::runtime_error("bad arg count");
}

void DFVectorUnpack::onDataReceived(size_t inlet, DFData *data)
{
    if(DFVector *vec = dynamic_cast<DFVector*>(data))
    {
        if(inlet == 0)
        {
            DFScalar sca;
            for(int i = 2; i >= 0; i--)
            {
                sca.data = vec->data[i];
                sendData(i, &sca);
            }
            return;
        }
    }

    DFNode::onDataReceived(inlet, data);
}

