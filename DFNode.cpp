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
#include "debug.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <QMutexLocker>

QMutex DFNode::mutex(QMutex::Recursive);
DFNodeIDMap DFNode::byId_;
DFNodeID DFNode::nextNodeId_ = 0;

DFException::DFException(DFNode *node, std::string message)
    : std::runtime_error(message), node_(node), message_(message)
{
}

DFException::~DFException() throw()
{
}

const char * DFException::what() const throw()
{
    return message_.c_str();
}

DFNode * DFException::node() const
{
    return node_;
}

DFNode::DFNode(QDataflowModelNode *node, const std::vector<std::string> &args)
    : QDataflowMetaObject(node), id_(DFNode::nextNodeId_++)
{
    QMutexLocker locker(&mutex);

    DFNode::byId_[id_] = this;
}

DFNode::~DFNode()
{
    QMutexLocker locker(&mutex);

    DFNode::byId_.erase(id_);
}

DFNodeID DFNode::id() const
{
    return id_;
}

DFNode * DFNode::byId(DFNodeID id)
{
    DFNodeIDMap::const_iterator it = DFNode::byId_.find(id);
    if(it == DFNode::byId_.end())
        throw std::range_error((boost::format("invalid node id: %d") % id).str());
    return it->second;
}

DFNode * DFNode::byId(DFNodeID id, DFNode *defaultIfNotFound)
{
    DFNodeIDMap::const_iterator it = DFNode::byId_.find(id);
    return it != DFNode::byId_.end() ? it->second : defaultIfNotFound;
}

void DFNode::deleteById(DFNodeID id)
{
    QMutexLocker locker(&mutex);

    DFNode *node = DFNode::byId(id);
    delete node;
}

std::vector<DFNodeID> DFNode::nodeIds()
{
    QMutexLocker locker(&mutex);

    std::vector<DFNodeID> ret;
    BOOST_FOREACH(const DFNodeIDMap::value_type &i, DFNode::byId_)
    {
        ret.push_back(i.first);
    }

    return ret;
}

std::vector<DFNode*> DFNode::nodes()
{
    QMutexLocker locker(&mutex);

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
    QMutexLocker locker(&mutex);

    BOOST_FOREACH(DFNodeIDMap::value_type &i, DFNode::byId_)
    {
        i.second->tick();
    }
}

void DFNode::onDataReceived(size_t inlet, DFData *data)
{
    throw DFException(this, (boost::format("no method for %s on inlet %d") % data->tag() % inlet).str());
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

