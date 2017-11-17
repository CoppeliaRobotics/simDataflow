#include "DFPrint.h"
#include "DFData.h"

DFPrint::DFPrint(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(1);

    if(args.size() == 1)
        prefix_ = "print";
    else if(args.size() == 2)
        prefix_ = args[1];
    else
        throw std::runtime_error("bad arg count");
}

void DFPrint::onDataReceived(size_t inlet, DFData *data)
{
    simAddStatusbarMessage((boost::format("%s: %s") % prefix_ % data->str()).str().c_str());
}
