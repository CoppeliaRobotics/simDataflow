#include "DFMathBinaryOperator.h"
#include "DFData.h"

DFMathBinaryOperator::DFMathBinaryOperator(QDataflowModelNode *node, const std::vector<std::string> &args)
    : DFNode(node, args)
{
    setInletCount(2);
    setOutletCount(1);

    op_ = args[0];
    if(op_ != "+" && op_ != "-" && op_ != "*" && op_ != "/")
        throw std::runtime_error((boost::format("invalid operator: %s") % op_).str());

    state_.data = args.size() == 2 ? boost::lexical_cast<float>(args[1]) : 0;
}

void DFMathBinaryOperator::op(DFScalar &x, const DFScalar &y)
{
    if(op_ == "+") add(x, y);
    else if(op_ == "-") sub(x, y);
    else if(op_ == "*") mul(x, y);
    else if(op_ == "/") div(x, y);
}

void DFMathBinaryOperator::add(DFScalar &x, const DFScalar &y)
{
    x.data += y.data;
}

void DFMathBinaryOperator::sub(DFScalar &x, const DFScalar &y)
{
    x.data -= y.data;
}

void DFMathBinaryOperator::mul(DFScalar &x, const DFScalar &y)
{
    x.data *= y.data;
}

void DFMathBinaryOperator::div(DFScalar &x, const DFScalar &y)
{
    x.data /= y.data;
}

void DFMathBinaryOperator::onDataReceived(size_t inlet, DFData *data)
{
    if(DFScalar *sca = dynamic_cast<DFScalar*>(data))
    {
        if(inlet == 0)
        {
            DFScalar tmp = *sca;
            op(tmp, state_);
            sendData(0, &tmp);
        }
        else
        {
            state_ = *sca;
        }
        return;
    }

    DFNode::onDataReceived(inlet, data);
}
