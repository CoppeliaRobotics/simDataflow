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
#include "DFNodeFactory.h"
#include "debug.h"
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <utility>
#include <QMutexLocker>

QMutex DFNode::mutex(QMutex::Recursive);
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
    QMutexLocker locker(&mutex);

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
    QMutexLocker locker(&mutex);

    disconnect();

    DFNode::byId_.erase(id_);
}

DFNodeID DFNode::id() const
{
    return id_;
}

int DFNode::x() const
{
    return x_;
}

int DFNode::y() const
{
    return y_;
}

void DFNode::setPos(int x, int y)
{
    x_ = x;
    y_ = y;
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

std::string DFNode::arg(size_t i)
{
    return args_[i];
}

size_t DFNode::argCount()
{
    return args_.size();
}

std::set<DFNodeOutlet> DFNode::inboundConnections(size_t inlet) const
{
    return inboundConnections_[inlet];
}

std::set<DFNodeInlet> DFNode::outboundConnections(size_t outlet) const
{
    return outboundConnections_[outlet];
}

std::set<DFConnection> DFNode::connections(bool inbound, bool outbound)
{
    std::set<DFConnection> ret;
    if(inbound)
    {
        for(size_t inlet = 0; inlet < inletCount(); inlet++)
        {
            BOOST_FOREACH(const DFNodeOutlet &src, inboundConnections_[inlet])
            {
                DFConnection c;
                c.src = src.node;
                c.srcOutlet = src.index;
                c.dst = this;
                c.dstInlet = inlet;
                ret.insert(c);
            }
        }
    }
    if(outbound)
    {
        for(size_t outlet = 0; outlet < outletCount(); outlet++)
        {
            BOOST_FOREACH(const DFNodeInlet &dst, outboundConnections_[outlet])
            {
                DFConnection c;
                c.src = this;
                c.srcOutlet = outlet;
                c.dst = dst.node;
                c.dstInlet = dst.index;
                ret.insert(c);
            }
        }
    }
    return ret;
}

std::set<DFConnection> DFNode::allConnections()
{
    std::set<DFConnection> ret;
    BOOST_FOREACH(DFNode *node, nodes())
    {
        BOOST_FOREACH(const DFConnection &conn, node->connections(false, true))
        {
            ret.insert(conn);
        }
    }
    return ret;
}

size_t DFNode::allConnections(std::vector<int> &srcNodeIds, std::vector<int> &srcOutlets, std::vector<int> &dstNodeIds, std::vector<int> &dstInlets)
{
    size_t n = 0;
    BOOST_FOREACH(const DFConnection &conn, allConnections())
    {
        n++;
        srcNodeIds.push_back(conn.src->id());
        srcOutlets.push_back(conn.srcOutlet);
        dstNodeIds.push_back(conn.dst->id());
        dstInlets.push_back(conn.dstInlet);
    }
    return n;
}

bool DFNode::isConnected(size_t outlet, DFNode *node, size_t inlet) const
{
    validateNode(node);
    validateOutlet(outlet);
    node->validateInlet(inlet);
    BOOST_FOREACH(DFNodeInlet i, outboundConnections_[outlet])
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

void DFNode::disconnectInlet(size_t inlet)
{
    BOOST_FOREACH(const DFNodeOutlet &x, inboundConnections_[inlet])
    {
        x.node->disconnect(x.index, this, inlet);
    }
}

void DFNode::disconnectOutlet(size_t outlet)
{
    BOOST_FOREACH(const DFNodeInlet &x, outboundConnections_[outlet])
    {
        this->disconnect(outlet, x.node, x.index);
    }
}

void DFNode::disconnect()
{
    for(size_t inlet = 0; inlet < inletCount(); inlet++)
        disconnectInlet(inlet);

    for(size_t outlet = 0; outlet < outletCount(); outlet++)
        disconnectOutlet(outlet);
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

void DFNode::connect(DFNodeID srcNodeId, size_t srcOutlet, DFNodeID dstNodeId, size_t dstInlet)
{
    QMutexLocker locker(&mutex);

    DFNode *srcNode = DFNode::byId(srcNodeId),
           *dstNode = DFNode::byId(dstNodeId);
    srcNode->connect(srcOutlet, dstNode, dstInlet);
}

void DFNode::disconnect(DFNodeID srcNodeId, size_t srcOutlet, DFNodeID dstNodeId, size_t dstInlet)
{
    QMutexLocker locker(&mutex);

    DFNode *srcNode = DFNode::byId(srcNodeId),
           *dstNode = DFNode::byId(dstNodeId);
    srcNode->disconnect(srcOutlet, dstNode, dstInlet);
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

void DFNode::getInfo(DFNodeID id, std::string &cmd, int &inletCount, int &outletCount, int &x, int &y)
{
    QMutexLocker locker(&mutex);

    DFNode *node = DFNode::byId(id);
    cmd = node->str();
    inletCount = node->inletCount();
    outletCount = node->outletCount();
    x = -1;
    y = -1;
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

void DFNode::setNumInlets(size_t n)
{
    QMutexLocker locker(&mutex);

    size_t oldN = inletCount();
    for(size_t inlet = n; inlet < oldN; inlet++)
        disconnectInlet(inlet);

    setNumIOlets(inlets_, n);
    inboundConnections_.resize(n);
}

void DFNode::setNumOutlets(size_t n)
{
    QMutexLocker locker(&mutex);

    size_t oldN = outletCount();
    for(size_t outlet = n; outlet < oldN; outlet++)
        disconnectOutlet(outlet);

    setNumIOlets(outlets_, n);
    outboundConnections_.resize(n);
}

void DFNode::onDataReceived(size_t inlet, DFData *data)
{
    throw DFException(this, (boost::format("no method for %s on inlet %d") % data->tag() % inlet).str());
}

void DFNode::sendData(size_t outlet, DFData *data)
{
    QMutexLocker locker(&mutex);

    validateOutlet(outlet);
    BOOST_FOREACH(DFNodeInlet o, outboundConnections_[outlet])
    {
        o.node->onDataReceived(o.index, data);
    }
}

void DFNode::clearGraph()
{
    BOOST_FOREACH(DFNode *node, nodes())
    {
        delete node;
    }
}

void DFNode::loadGraph(std::string s)
{
    std::istringstream ss(s);
    std::map<int, int> idmap;

    while(!ss.eof())
    {
        std::string key, stop;
        ss >> key;
        if(key == "NODE")
        {
            int id, x, y;
            ss >> id >> x >> y;
            std::string tok;
            std::vector<std::string> args;
            while(true)
            {
                ss >> tok;
                if(tok == ";") break;
                args.push_back(tok);
            }
            DFNode *node = nodeFactory.create(args, x, y);
            idmap[id] = node->id();
            DBG << "NODE " << node->id() << " " << x << " " << y << " " << node->str() << std::endl;
        }
        else if(key == "CONNECTION")
        {
            int srcId, srcOutlet, dstId, dstInlet;
            ss >> srcId >> srcOutlet >> dstId >> dstInlet >> stop;
            srcId = idmap[srcId];
            dstId = idmap[dstId];
            DBG << "CONNECTION " << srcId << " " << srcOutlet << " " << dstId << " " << dstInlet << std::endl;
            DFNode::connect(srcId, srcOutlet, dstId, dstInlet);
        }
    }
}

std::string DFNode::saveGraph()
{
    std::stringstream ss;

    BOOST_FOREACH(DFNode *node, nodes())
    {
        ss << "NODE " << node->id() << " " << node->x() << " " << node->y() << " " << node->str() << " " << ";" << std::endl;
    }

    BOOST_FOREACH(const DFConnection &conn, allConnections())
    {
        ss << "CONNECTION " << conn.src->id() << " " << conn.srcOutlet << " " << conn.dst->id() << " " << conn.dstInlet << " " << ";" << std::endl;
    }

    return ss.str();
}

void DFNode::saveGraphToScene()
{
    DBG << std::endl;
    std::string graph = DFNode::saveGraph();
    simWriteCustomDataBlock(sim_handle_scene, "Dataflow.Graph", graph.c_str(), graph.length());
    DBG << "new graph:" << std::endl << graph;
}

void DFNode::restoreGraphFromScene()
{
    DFNode::clearGraph();
    simInt size = 0;
    simChar *data = simReadCustomDataBlock(sim_handle_scene, "Dataflow.Graph", &size);
    if(data)
    {
        DBG << "found a Dataflow.Graph block" << std::endl;
        std::string graph(data, size);
        try
        {
            DFNode::loadGraph(graph);
        }
        catch(std::exception &ex)
        {
            DBG << "failed to restore graph: " << ex.what() << std::endl;
            DFNode::clearGraph();
        }
    }
    else
    {
        DBG << "the scene does not have a Dataflow.Graph block" << std::endl;
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

