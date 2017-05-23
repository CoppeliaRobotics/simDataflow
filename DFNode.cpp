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

#include "DFNode.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <utility>

DFNodeIDMap DFNode::byId_;
DFNodeID DFNode::nextNodeId_ = 0;

bool DFNodeIOlet::operator<(const DFNodeIOlet &o) const
{
    return std::make_pair(node, index) < std::make_pair(o.node, o.index);
}

bool DFConnection::operator<(const DFConnection &o) const
{
    return std::make_pair(std::make_pair(src, srcOutlet), std::make_pair(dst, dstInlet))
        < std::make_pair(std::make_pair(o.src, o.srcOutlet), std::make_pair(o.dst, o.dstInlet));
}

void DFNode::validateInlet(size_t i) const
{
    validateIOlet(inlets_, i, "inlet");
}

void DFNode::validateOutlet(size_t i) const
{
    validateIOlet(outlets_, i, "outlet");
}

void DFNode::validateNode(DFNode *node) const
{
    if(!node)
        throw std::runtime_error("node cannot be null");
}

DFNode::DFNode(const std::vector<std::string> &args)
    : id_(DFNode::nextNodeId_++), args_(args)
{
    DFNode::byId_[id_] = this;

    std::stringstream ss;
    bool first = true;
    BOOST_FOREACH(const std::string &s, args)
    {
        ss << (first ? "" : " ") << s;
        first = false;
    }
    text_ = ss.str();
}

DFNode::~DFNode()
{
    DFNode::byId_.erase(id_);
}

DFNodeID DFNode::id() const
{
    return id_;
}

std::string DFNode::str() const
{
    return text_;
}

DFNodeInlet DFNode::inlet(size_t i) const
{
    validateInlet(i);
    return inlets_[i];
}

size_t DFNode::inletCount() const
{
    return inlets_.size();
}

DFNodeOutlet DFNode::outlet(size_t i) const
{
    validateOutlet(i);
    return outlets_[i];
}

size_t DFNode::outletCount() const
{
    return outlets_.size();
}

std::set<DFNodeOutlet> DFNode::inboundConnections(size_t inlet) const
{
    return inboundConnections_.at(inlet);
}

std::set<DFNodeInlet> DFNode::outboundConnections(size_t outlet) const
{
    return outboundConnections_.at(outlet);
}

std::set<DFConnection> DFNode::connections()
{
    std::set<DFConnection> ret;
    BOOST_FOREACH(DFNodeInboundConnections::value_type x, inboundConnections_)
    {
        BOOST_FOREACH(DFNodeOutlet src, x.second)
        {
            DFConnection c;
            c.src = src.node;
            c.srcOutlet = src.index;
            c.dst = this;
            c.dstInlet = x.first;
            ret.insert(c);
        }
    }
    BOOST_FOREACH(DFNodeOutboundConnections::value_type x, outboundConnections_)
    {
        BOOST_FOREACH(DFNodeInlet dst, x.second)
        {
            DFConnection c;
            c.src = this;
            c.srcOutlet = x.first;
            c.dst = dst.node;
            c.dstInlet = dst.index;
            ret.insert(c);
        }
    }
    return ret;
}

bool DFNode::isConnected(size_t outlet, DFNode *node, size_t inlet) const
{
    validateNode(node);
    validateOutlet(outlet);
    node->validateInlet(inlet);
    BOOST_FOREACH(DFNodeInlet i, outboundConnections_.at(outlet))
    {
        if(i.node == node && i.index == inlet) return true;
    }
    return false;
}

void DFNode::connect(size_t outlet, DFNode *node, size_t inlet)
{
    validateNode(node);
    validateOutlet(outlet);
    node->validateInlet(inlet);
    if(isConnected(outlet, node, inlet)) return;
    outboundConnections_[outlet].insert(node->inlets_[inlet]);
    node->inboundConnections_[inlet].insert(outlets_[outlet]);
}

void DFNode::disconnect(size_t outlet, DFNode *node, size_t inlet)
{
    validateNode(node);
    validateOutlet(outlet);
    node->validateInlet(inlet);
    std::set<DFNodeInlet> &outConns = outboundConnections_[outlet];
    for(std::set<DFNodeInlet>::iterator it = outConns.begin(); it != outConns.end();)
    {
        if(it->node == node && it->index == inlet)
            outConns.erase(it++);
        else
            ++it;
    }
    std::set<DFNodeOutlet> &inConns = inboundConnections_[inlet];
    for(std::set<DFNodeOutlet>::iterator it = inConns.begin(); it != inConns.end();)
    {
        if(it->node == this && it->index == outlet)
            inConns.erase(it++);
        else
            ++it;
    }
}

DFNode * DFNode::byId(DFNodeID id)
{
    DFNodeIDMap::const_iterator it = DFNode::byId_.find(id);
    if(it == DFNode::byId_.end())
    {
        std::stringstream ss;
        ss << "invalid node id: " << id;
        throw std::range_error(ss.str());
    }
    return it->second;
}

DFNode * DFNode::byId(DFNodeID id, DFNode *defaultIfNotFound)
{
    DFNodeIDMap::const_iterator it = DFNode::byId_.find(id);
    return it != DFNode::byId_.end() ? it->second : defaultIfNotFound;
}

void DFNode::deleteById(DFNodeID id)
{
    DFNode *node = DFNode::byId(id);
    delete node;
}

std::vector<DFNode*> DFNode::nodes()
{
    std::vector<DFNode*> ret;
    BOOST_FOREACH(const DFNodeIDMap::value_type &i, DFNode::byId_)
    {
        ret.push_back(i.second);
    }
    return ret;
}

void DFNode::tick()
{
}

void DFNode::tickAll()
{
    BOOST_FOREACH(DFNodeIDMap::value_type &i, DFNode::byId_)
    {
        i.second->tick();
    }
}

void DFNode::setNumInlets(size_t n)
{
    setNumIOlets(inlets_, n);
}

void DFNode::setNumOutlets(size_t n)
{
    setNumIOlets(outlets_, n);
}

void DFNode::onDataReceived(size_t inlet, DFData *data)
{
}

void DFNode::sendData(size_t outlet, DFData *data)
{
    validateOutlet(outlet);
    BOOST_FOREACH(DFNodeInlet o, outboundConnections_[outlet])
    {
        o.node->onDataReceived(o.index, data);
    }
}

simInt DFNode::getObjectHandle(const std::string &strArg)
{
    try
    {
        return boost::lexical_cast<simInt>(strArg);
    }
    catch(boost::bad_lexical_cast &ex)
    {
        return simGetObjectHandle(strArg.c_str());
    }
}

