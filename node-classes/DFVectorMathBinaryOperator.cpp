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

#include "DFVectorMathBinaryOperator.h"
#include "DFData.h"

DFVectorMathBinaryOperator::DFVectorMathBinaryOperator(const std::vector<std::string> &args)
    : DFNode(args)
{
    setNumInlets(2);
    setNumOutlets(1);

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
    }
}
