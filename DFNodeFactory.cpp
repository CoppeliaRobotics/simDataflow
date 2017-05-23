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

#include "DFNodeFactory.h"
#include "DFMathBinaryOperator.h"
#include "DFObjectPos.h"
#include "DFPrint.h"
#include "plugin.h"

#include <iostream>
#include <boost/tokenizer.hpp>

DFNodeFactory nodeFactory;

DFNode * DFNodeFactory::create(const std::string &args)
{
    boost::tokenizer<> tokenizer(args);
    std::vector<std::string> tokens;
    for(boost::tokenizer<>::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
        tokens.push_back(*it);
    return create(tokens);
}

DFNode * DFNodeFactory::create(const std::vector<std::string> &args)
{
    if(args.size() == 0)
    {
        throw std::runtime_error("cannot instantiate object with empty commandline");
    }
    std::map<std::string, PCreateFunc>::const_iterator it = createFuncs_.find(args[0]);
    if(it == createFuncs_.end())
    {
        std::stringstream ss;
        ss << "no such class: " << args[0];
        throw std::runtime_error(ss.str());
    }
    try
    {
        return it->second(args);
    }
    catch(std::exception &ex)
    {
        std::stringstream ss;
        ss << args[0] << ": instantiation error: " << ex.what();
        throw std::runtime_error(ss.str());
    }
}

std::vector<std::string> DFNodeFactory::classNames()
{
    std::vector<std::string> ret;
    for(std::map<std::string, PCreateFunc>::const_iterator it = createFuncs_.begin(); it != createFuncs_.end(); ++it)
        ret.push_back(it->first);
    return ret;
}

void initNodeFactory()
{
    std::cout << PLUGIN_NAME << ": initializing node factory" << std::endl;
    nodeFactory.registerClass<DFMathBinaryOperator>("+");
    nodeFactory.registerClass<DFMathBinaryOperator>("-");
    nodeFactory.registerClass<DFMathBinaryOperator>("*");
    nodeFactory.registerClass<DFMathBinaryOperator>("/");
    nodeFactory.registerClass<DFObjectPos>("objectpos");
    nodeFactory.registerClass<DFPrint>("print");
    std::cout << PLUGIN_NAME << ": initialized node factory (" << nodeFactory.size() << " classes)" << std::endl;
}

