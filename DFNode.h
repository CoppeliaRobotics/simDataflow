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
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

class DFNode;
class DFData;
struct DFNodeInlet;
struct DFNodeOutlet;

struct DFNodeIOlet
{
    DFNode *node;
    size_t index;
};

struct DFNodeInlet : public DFNodeIOlet
{
    std::vector<DFNodeOutlet*> connections;
};

struct DFNodeOutlet : public DFNodeIOlet
{
    std::vector<DFNodeInlet*> connections;
};

typedef size_t DFNodeID;
typedef std::map<DFNodeID, DFNode*> DFNodeIDMap;

class DFNode
{
private:
    static DFNodeIDMap byId_;
    static DFNodeID nextNodeId_;
    DFNodeID id_;
    std::vector<std::string> args_;
    std::string text_;
    std::vector<DFNodeInlet> inlets_;
    std::vector<DFNodeOutlet> outlets_;

    template<typename T>
    void setNumIOlets(std::vector<T> &v, size_t n)
    {
        size_t oldSize = v.size();
        v.resize(n);
        for(size_t i = oldSize; i < n; i++)
        {
            v[i].node = this;
            v[i].index = i;
        }
    }

    template<typename T>
    void validateIOlet(const std::vector<T> &v, size_t i, const char *n) const
    {
        if(i >= v.size())
        {
            std::stringstream ss;
            ss << "invalid " << n << " index: " << i;
            throw std::range_error(ss.str());
        }
    }

    void validateInlet(size_t i) const;
    void validateOutlet(size_t i) const;
    void validateNode(DFNode *node) const;

public:
    DFNode(const std::vector<std::string> &args);
    virtual ~DFNode();
    DFNodeID id() const;
    std::string str() const;
    DFNodeInlet inlet(size_t i) const;
    size_t inletCount() const;
    DFNodeOutlet outlet(size_t i) const;
    size_t outletCount() const;
    bool isConnected(size_t outlet, DFNode *node, size_t inlet) const;
    void connect(size_t outlet, DFNode *node, size_t inlet);
    void disconnect(size_t outlet, DFNode *node, size_t inlet);
    static DFNode * byId(DFNodeID id);
    static DFNode * byId(DFNodeID id, DFNode *defaultIfNotFound);
    static std::vector<DFNode*> nodes();
    virtual void tick();
    static void tickAll();

protected:
    void setNumInlets(size_t n);
    void setNumOutlets(size_t n);
    virtual void onDataReceived(size_t inlet, DFData *data);
    void sendData(size_t outlet, DFData *data);
};

#endif // DFNODE_H_INCLUDED
