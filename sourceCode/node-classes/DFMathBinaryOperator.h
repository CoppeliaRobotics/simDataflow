#ifndef DFMATHBINARYOPERATOR_H_INCLUDED
#define DFMATHBINARYOPERATOR_H_INCLUDED

#include "DFNode.h"
#include "DFScalar.h"

class DFMathBinaryOperator : public DFNode
{
private:
    DFScalar state_;
    std::string op_;

public:
    DFMathBinaryOperator(QDataflowModelNode *node, const std::vector<std::string> &args);
    void onDataReceived(size_t inlet, DFData *data);

protected:
    void op(DFScalar &x, const DFScalar &y);
    void add(DFScalar &x, const DFScalar &y);
    void mul(DFScalar &x, const DFScalar &y);
    void sub(DFScalar &x, const DFScalar &y);
    void div(DFScalar &x, const DFScalar &y);
};

#endif // DFMATHBINARYOPERATOR_H_INCLUDED
