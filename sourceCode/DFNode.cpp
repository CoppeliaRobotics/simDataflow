#include "DFNode.h"

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <utility>

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
    DFNode::byId_[id_] = this;
}

DFNode::~DFNode()
{
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
    DFNode *node = DFNode::byId(id);
    delete node;
}

std::vector<DFNodeID> DFNode::nodeIds()
{
    std::vector<DFNodeID> ret;
    BOOST_FOREACH(const DFNodeIDMap::value_type &i, DFNode::byId_)
    {
        ret.push_back(i.first);
    }

    return ret;
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

void DFNode::sendData(size_t outlet, DFData *data)
{
    BOOST_FOREACH(QDataflowModelConnection *conn, this->outlet(outlet)->connections())
    {
        QDataflowModelNode *node = conn->dest()->node();
        if(DFNode *dfnode = dynamic_cast<DFNode*>(node->dataflowMetaObject()))
            dfnode->onDataReceived(conn->dest()->index(), data);
    }
}

void DFNode::onDataReceived(size_t inlet, DFData *data)
{
    throw DFException(this, (boost::format("no method for %s on inlet %d") % data->tag() % inlet).str());
}

int DFNode::getObjectHandle(const std::string &strArg)
{
    try
    {
        return boost::lexical_cast<int>(strArg);
    }
    catch(boost::bad_lexical_cast &ex)
    {
        return simGetObjectHandle(strArg.c_str());
    }
}

