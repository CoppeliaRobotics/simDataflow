#ifndef DFPRINT_H_INCLUDED
#define DFPRINT_H_INCLUDED

#include "DFNode.h"

class DFPrint : public DFNode
{
private:
    std::string prefix_;

public:
    DFPrint(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);
};

#endif // DFPRINT_H_INCLUDED
