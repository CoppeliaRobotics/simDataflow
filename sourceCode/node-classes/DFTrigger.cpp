#include "DFTrigger.h"
#include "DFBang.h"

DFTrigger::DFTrigger(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(1);
    setOutletCount(args.size() - 1);

    for(size_t i = 1; i < args.size(); i++)
        outletType.push_back(validateArg(args[i]));
}

char DFTrigger::validateArg(const std::string &a)
{
    if(a == "bang" || a == "b") return 'b';
    if(a == "anything" || a == "a") return 'a';
    throw DFException(this, (boost::format("invalid argument: %s") % a).str());
}

void DFTrigger::onDataReceived(size_t inlet, DFData *data)
{
    if(inlet == 0)
    {
        DFBang bang;
        for(size_t i = outletType.size() - 1; i > 0; i--)
            sendData(i - 1, outletType[i] == 'b' ? &bang : data);
        return;
    }

    DFNode::onDataReceived(inlet, data);
}

