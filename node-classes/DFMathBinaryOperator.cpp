// Copyright 2016 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// -------------------------------------------------------------------
// Authors:
// Federico Ferri <federico.ferri.it at gmail dot com>
// -------------------------------------------------------------------

#include "DFMathBinaryOperator.h"
#include "DFData.h"

DFMathBinaryOperator::DFMathBinaryOperator(const std::vector<std::string> &args)
    : DFNode(args)
{
    setNumInlets(2);
    setNumOutlets(1);

    op_ = args[0];
    if(op_ != "+" && op_ != "-" && op_ != "*" && op_ != "/")
    {
        std::stringstream ss;
        ss << "invalid operator: " << op_;
        throw std::runtime_error(ss.str());
    }

    state_.data = args.size() == 2 ? boost::lexical_cast<simFloat>(args[1]) : 0;
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
    }
}
