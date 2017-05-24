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

#ifndef NODEFACTORY_H_INCLUDED
#define NODEFACTORY_H_INCLUDED

#include <vector>
#include <string>
#include <map>

class DFNode;

class DFNodeFactory
{
public:
    template<typename T>
    void registerClass(std::string name)
    {
        createFuncs_[name] = &createFunc<T>;
    }

    DFNode * create(const std::string &args, int x, int y);

    DFNode * create(const std::string &args);

    DFNode * create(const std::vector<std::string> &args);

    inline size_t size() {return createFuncs_.size();}

    std::vector<std::string> classNames();

private:
    template<typename T>
    static DFNode * createFunc(const std::vector<std::string> &args)
    {
        return new T(args);
    }

    typedef DFNode* (*PCreateFunc)(const std::vector<std::string> &);
    std::map<std::string, PCreateFunc> createFuncs_;
};

extern DFNodeFactory nodeFactory;

void initNodeFactory();

#endif // NODEFACTORY_H_INCLUDED
