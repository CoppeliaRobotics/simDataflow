#ifndef DFTRIGGER_H_INCLUDED
#define DFTRIGGER_H_INCLUDED

#include "DFNode.h"

class DFTrigger : public DFNode
{
private:
    char validateArg(const std::string &arg);

public:
    DFTrigger(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);

private:
    std::vector<char> outletType;
};

#endif // DFTRIGGER_H_INCLUDED
