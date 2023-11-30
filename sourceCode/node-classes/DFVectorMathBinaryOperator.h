#ifndef DFVECTORMATHBINARYOPERATOR_H_INCLUDED
#define DFVECTORMATHBINARYOPERATOR_H_INCLUDED

#include "DFNode.h"
#include "DFVector.h"

class DFVectorMathBinaryOperator : public DFNode
{
private:
    DFVector state_;
    std::string op_;

public:
    DFVectorMathBinaryOperator(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);

protected:
    void op(DFVector &x, const DFVector &y);
    void add(DFVector &x, const DFVector &y);
    void mul(DFVector &x, const DFVector &y);
    void sub(DFVector &x, const DFVector &y);
    void div(DFVector &x, const DFVector &y);
};

#endif // DFVECTORMATHBINARYOPERATOR_H_INCLUDED
