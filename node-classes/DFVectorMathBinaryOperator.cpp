#include "DFVectorMathBinaryOperator.h"
#include "DFData.h"

DFVectorMathBinaryOperator::DFVectorMathBinaryOperator(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(2);
    setOutletCount(1);

    std::string prefix = args[0].substr(0, args[0].length() - 1);
    if(prefix != "vec.")
        throw std::runtime_error((boost::format("invalid node prefix: %s") % prefix).str());

    op_ = args[0].substr(args[0].length() - 1);
    if(op_ != "+" && op_ != "-" && op_ != "*" && op_ != "/")
        throw std::runtime_error((boost::format("invalid operator: %s") % op_).str());

    for(size_t i = 0, j = 1; i < 3 && j < args.size(); i++, j++)
        state_.data[i] = (j < args.size()) ?  boost::lexical_cast<simFloat>(args[j]) : 0;
}

void DFVectorMathBinaryOperator::op(DFVector &x, const DFVector &y)
{
    if(op_ == "+") add(x, y);
    else if(op_ == "-") sub(x, y);
    else if(op_ == "*") mul(x, y);
    else if(op_ == "/") div(x, y);
}

void DFVectorMathBinaryOperator::add(DFVector &x, const DFVector &y)
{
    for(int i = 0; i < 3; i++)
        x.data[i] += y.data[i];
}

void DFVectorMathBinaryOperator::sub(DFVector &x, const DFVector &y)
{
    for(int i = 0; i < 3; i++)
        x.data[i] -= y.data[i];
}

void DFVectorMathBinaryOperator::mul(DFVector &x, const DFVector &y)
{
    for(int i = 0; i < 3; i++)
        x.data[i] *= y.data[i];
}

void DFVectorMathBinaryOperator::div(DFVector &x, const DFVector &y)
{
    for(int i = 0; i < 3; i++)
        x.data[i] /= y.data[i];
}

void DFVectorMathBinaryOperator::onDataReceived(size_t inlet, DFData *data)
{
    if(DFVector *vec = dynamic_cast<DFVector*>(data))
    {
        if(inlet == 0)
        {
            DFVector tmp = *vec;
            op(tmp, state_);
            sendData(0, &tmp);
        }
        else
        {
            state_ = *vec;
        }
        return;
    }

    DFNode::onDataReceived(inlet, data);
}
