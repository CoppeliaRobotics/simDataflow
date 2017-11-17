#ifndef DFNODE_H_INCLUDED
#define DFNODE_H_INCLUDED

#include "v_repLib.h"
#include "debug.h"
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
    void sendData(size_t outlet, DFData *data);
    virtual simInt getObjectHandle(const std::string &arg);

protected:
    virtual void onDataReceived(size_t inlet, DFData *data);
};

#endif // DFNODE_H_INCLUDED
