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

#include "DFTrigger.h"
#include "DFBang.h"

DFTrigger::DFTrigger(const std::vector<std::string> &args)
    : DFNode(args)
{
    setNumInlets(1);
    setNumOutlets(args.size() - 1);

    for(size_t i = 1; i < args.size(); i++)
        validateArg(args[i]);
}

void DFTrigger::validateArg(const std::string &a)
{
    if(a == "bang" || a == "b") return;
    if(a == "anything" || a == "a") return;
    throw DFException(this, (boost::format("invalid argument: %s") % a).str());
}

void DFTrigger::onDataReceived(size_t inlet, DFData *data)
{
    if(inlet == 0)
    {
        DFBang bang;
        for(size_t i = argCount() - 1; i > 0; i--)
            sendData(i - 1, arg(i)[0] == 'b' ? &bang : data);
        return;
    }

    DFNode::onDataReceived(inlet, data);
}

