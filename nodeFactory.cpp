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

#include "nodeFactory.h"
#include "DFObjectPos.h"
#include "DFPrint.h"

#define BEGIN_OBJECT_MAP() if(0) {}
#define MAP_OBJECT_CLASS(STR,CLS) \
    else if(args[0] == STR) \
    { \
        try \
        { \
            return new CLS(args); \
        } \
        catch(std::exception &ex) \
        { \
            std::stringstream ss; \
            ss << args[0] << " initialization failed: " << ex.what(); \
            throw std::runtime_error(ss.str().c_str()); \
        } \
    }
#define END_OBJECT_MAP() else \
    { \
        std::stringstream ss; \
        ss << "object does not exist: " << args[0]; \
        throw std::runtime_error(ss.str().c_str()); \
    }

DFNode * nodeFactory(const std::vector<std::string> &args)
{
    BEGIN_OBJECT_MAP()
    MAP_OBJECT_CLASS("objectpos", DFObjectPos)
    MAP_OBJECT_CLASS("print", DFPrint)
    END_OBJECT_MAP()
}
