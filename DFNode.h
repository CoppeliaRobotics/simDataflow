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

#ifndef DFNODE_H_INCLUDED
#define DFNODE_H_INCLUDED

#include "v_repLib.h"
#include "DFData.h"
#include "qdataflowmodel.h"
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

class DFNode;
class DFData;

class DFException : public std::runtime_error
{
public:
    DFException(DFNode *node, std::string message);
    virtual ~DFException() throw();
    virtual const char * what() const throw();

    DFNode * node() const;

private:
    DFNode *node_;
    std::string message_;
};

typedef size_t DFNodeID;
typedef std::map<DFNodeID, DFNode*> DFNodeIDMap;

class DFNode : public QDataflowMetaObject
{
private:
    static DFNodeIDMap byId_;
    static DFNodeID nextNodeId_;
    DFNodeID id_;

public:
    DFNode(QDataflowModelNode *node, const std::vector<std::string> &args);
    virtual ~DFNode();
    DFNodeID id() const;
    static DFNode * byId(DFNodeID id);
    static DFNode * byId(DFNodeID id, DFNode *defaultIfNotFound);
    static void deleteById(DFNodeID id);
    static std::vector<DFNodeID> nodeIds();
    static std::vector<DFNode*> nodes();
    virtual void tick();
    static void tickAll();

    virtual simInt getObjectHandle(const std::string &arg);

protected:
    virtual void onDataReceived(size_t inlet, DFData *data);
};

#endif // DFNODE_H_INCLUDED
