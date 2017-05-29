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

#include "DFModel.h"
#include "DFWindow.h"
#include "DFNode.h"
#include "UIProxy.h"
#include "stubs.h"
#include "debug.h"

#include "DFMathBinaryOperator.h"
#include "DFObjectPos.h"
#include "DFPrint.h"
#include "DFTrigger.h"
#include "DFVectorMathBinaryOperator.h"
#include "DFVectorPack.h"
#include "DFVectorUnpack.h"

#include <boost/tokenizer.hpp>

extern DFWindow *mainWindow;
extern UIProxy *uiProxy;

DFModel::DFModel(QObject *parent)
    : QDataflowModel(parent), blockGraphChangeSignal(false)
{
    QObject::connect(this, &DFModel::nodeAdded, this, &DFModel::onNodeAdded);
    QObject::connect(this, &DFModel::nodeRemoved, this, &DFModel::onNodeRemoved);
    QObject::connect(this, &DFModel::nodeTextChanged, this, &DFModel::onNodeTextChanged);
    QObject::connect(this, &DFModel::connectionAdded, this, &DFModel::onConnectionAdded);
    QObject::connect(this, &DFModel::connectionRemoved, this, &DFModel::onConnectionRemoved);
    QObject::connect(this, &DFModel::graphChanged, this, &DFModel::onGraphChanged);

    registerClass<DFMathBinaryOperator>("+");
    registerClass<DFMathBinaryOperator>("-");
    registerClass<DFMathBinaryOperator>("*");
    registerClass<DFMathBinaryOperator>("/");
    registerClass<DFObjectPos>("object.pos");
    registerClass<DFPrint>("print");
    registerClass<DFTrigger>("trig");
    registerClass<DFVectorMathBinaryOperator>("vec.+");
    registerClass<DFVectorMathBinaryOperator>("vec.-");
    registerClass<DFVectorMathBinaryOperator>("vec.*");
    registerClass<DFVectorMathBinaryOperator>("vec./");
    registerClass<DFVectorPack>("vec.pack");
    registerClass<DFVectorUnpack>("vec.unpack");
}

void DFModel::loadGraph(std::string s)
{
    std::istringstream ss(s);
    std::map<int, int> idmap;

    size_t numNodes = 0, numConnections = 0;

    blockGraphChangeSignal = true;

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
            std::stringstream args_s;
            std::string sep = "";
            while(true)
            {
                ss >> tok;
                if(tok == ";") break;
                args.push_back(tok);
                args_s << sep << tok;
                sep = " ";
            }
            QDataflowModelNode *node = create(QPoint(x, y), QString::fromStdString(args_s.str()), 0, 0);
            DFNode *dfnode = dynamic_cast<DFNode*>(node->dataflowMetaObject());
            if(dfnode)
            {
                idmap[id] = dfnode->id();
                DBG << "NODE " << dfnode->id() << " " << x << " " << y << " " << dfnode->node()->text().toStdString() << std::endl;
                numNodes++;
            }
        }
        else if(key == "CONNECTION")
        {
            int srcId, srcOutlet, dstId, dstInlet;
            ss >> srcId >> srcOutlet >> dstId >> dstInlet >> stop;
            srcId = idmap[srcId];
            dstId = idmap[dstId];
            DBG << "CONNECTION " << srcId << " " << srcOutlet << " " << dstId << " " << dstInlet << std::endl;
            DFNode *src = DFNode::byId(srcId);
            DFNode *dst = DFNode::byId(dstId);
            connect(src->node(), srcOutlet, dst->node(), dstInlet);
            numConnections++;
        }
    }

    blockGraphChangeSignal = false;

    emit graphChanged();

    DBG << "loaded " << numNodes << " nodes and " << numConnections << " connections" << std::endl;
}

std::string DFModel::saveGraph()
{
    std::stringstream ss;

    size_t numNodes = 0, numConnections = 0;

    BOOST_FOREACH(QDataflowModelNode *node, nodes())
    {
        if(node->text() == "") continue; // skip empty (tmp) nodes
        QDataflowMetaObject *meta = node->dataflowMetaObject();
        if(!meta)
        {
            DBG << "error: node " << (void*)node << " has no dataflowMetaObject" << std::endl;
            continue;
        }
        DFNode *dfnode = dynamic_cast<DFNode*>(node->dataflowMetaObject());
        if(!dfnode)
        {
            DBG << "error: node " << (void*)node << " has alien dataflowMetaObject" << std::endl;
            continue;
        }
        QPoint pos = node->pos();
        ss << "NODE " << dfnode->id() << " " << pos.x() << " " << pos.y() << " " << node->text().toStdString() << " " << ";" << std::endl;
        numNodes++;
    }

    BOOST_FOREACH(const QDataflowModelConnection *conn, connections())
    {
        QDataflowModelOutlet *s = conn->source();
        QDataflowModelInlet *d = conn->dest();
        QDataflowMetaObject *mosrc = s->node()->dataflowMetaObject();
        QDataflowMetaObject *modst = d->node()->dataflowMetaObject();
        if(!mosrc || !modst)
        {
            DBG << "error: connection " << (void*)conn << " source/dest node " << (void*)s->node() << "/" << (void*)d->node() << " has no dataflowMetaObject (" << (void*)mosrc << "/" << (void*)modst << ")" << std::endl;
            continue;
        }
        DFNode *src = dynamic_cast<DFNode*>(s->node()->dataflowMetaObject());
        DFNode *dst = dynamic_cast<DFNode*>(d->node()->dataflowMetaObject());
        if(!src || !dst)
        {
            DBG << "error: connection " << (void*)conn << " source/dest node " << (void*)s->node() << "/" << (void*)d->node() << " has alien dataflowMetaObject (" << (void*)src << "/" << (void*)dst << ")" << std::endl;
            continue;
        }
        ss << "CONNECTION " << src->id() << " " << s->index() << " " << dst->id() << " " << d->index() << " " << ";" << std::endl;
        numConnections++;
    }

    DBG << "saved " << numNodes << " nodes and " << numConnections << " connections" << std::endl;

    return ss.str();
}

void DFModel::clearGraph()
{
    BOOST_FOREACH(QDataflowModelNode *node, nodes())
    {
        remove(node);
    }
}

void DFModel::saveGraphToScene()
{
    std::string graph = saveGraph();
    DBG << "new graph:" << std::endl << graph;
    simWriteCustomDataBlock(sim_handle_scene, "Dataflow.Graph", graph.c_str(), graph.length());
}

void DFModel::restoreGraphFromScene()
{
    clearGraph();
    simInt size = 0;
    simChar *data = simReadCustomDataBlock(sim_handle_scene, "Dataflow.Graph", &size);
    if(data)
    {
        DBG << "found a Dataflow.Graph block" << std::endl;
        std::string graph(data, size);
        try
        {
            loadGraph(graph);
        }
        catch(std::exception &ex)
        {
            DBG << "failed to restore graph: " << ex.what() << std::endl;
            clearGraph();
        }
    }
    else
    {
        DBG << "the scene does not have a Dataflow.Graph block" << std::endl;
    }
}

QDataflowModelNode * DFModel::newNode(QPoint pos, QString text, int inletCount, int outletCount)
{
    DBG << std::endl;

    QDataflowModelNode *node = QDataflowModel::newNode(pos, text, inletCount, outletCount);
    node->setValid(false);

    if(text != "")
    {
        try
        {
            DFNode *dfnode = createDFMetaObject(node, text.toStdString());
            DBG << "created DFNode " << dfnode->id() << " text='" << text.toStdString() << "'" << std::endl;
        }
        catch(std::exception &ex) {}
    }

    return node;
}

void DFModel::onNodeAdded(QDataflowModelNode *node)
{
    DBG << std::endl;

    emit graphChanged();
}

void DFModel::onNodeRemoved(QDataflowModelNode *node)
{
    DBG << std::endl;

    emit graphChanged();
}

void DFModel::onNodeTextChanged(QDataflowModelNode *node, QString text)
{
    DBG << "node=" << (void*)node << " text='" << node->text().toStdString() << "'" << std::endl;

    DFNode *dfnode = 0L;

    if(node->text() != "")
    {
        try
        {
            dfnode = createDFMetaObject(node, node->text().toStdString());
            DBG << "created DFNode " << (void*)dfnode << " id=" << dfnode->id() << " text='" << node->text().toStdString() << "' node=" << (void*)dfnode->node() << " node.meta=" << (void*)node->dataflowMetaObject() << std::endl;
        }
        catch(std::runtime_error &ex)
        {
            DBG << "creation error: " << ex.what() << std::endl;
            simAddStatusbarMessage((boost::format("Dataflow: object creation error: %s") % ex.what()).str().c_str());
        }
    }

    if(!dfnode)
    {
        node->setValid(false);
        node->setDataflowMetaObject(0L);
        node->setInletCount(0);
        node->setOutletCount(0);
    }

    emit graphChanged();
}

void DFModel::onConnectionAdded(QDataflowModelConnection *conn)
{
    DBG << std::endl;

    emit graphChanged();
}

void DFModel::onConnectionRemoved(QDataflowModelConnection *conn)
{
    DBG << std::endl;

    emit graphChanged();
}

void DFModel::onGraphChanged()
{
    DBG << std::endl;

    if(!blockGraphChangeSignal)
        saveGraphToScene();
}

DFNode * DFModel::createDFMetaObject(QDataflowModelNode *node, const std::string &args)
{
    boost::char_separator<char> sep(" ");
    boost::tokenizer< boost::char_separator<char> > tokenizer(args, sep);
    std::vector<std::string> tokens;
    BOOST_FOREACH(std::string token, tokenizer)
        tokens.push_back(token);
    return createDFMetaObject(node, tokens);
}

DFNode * DFModel::createDFMetaObject(QDataflowModelNode *node, const std::vector<std::string> &args)
{
    if(args.size() == 0)
        throw std::runtime_error("cannot instantiate object with empty commandline");

    std::map<std::string, PCreateFunc>::const_iterator it = createFuncs_.find(args[0]);
    if(it == createFuncs_.end())
        throw std::runtime_error((boost::format("no such class: %s") % args[0]).str());

    try
    {
        DFNode *dfnode = it->second(node, args);
        node->setDataflowMetaObject(dfnode);
        node->setValid(true);
        return dfnode;
    }
    catch(std::exception &ex)
    {
        throw std::runtime_error((boost::format("%s: instantiation error: %s") % args[0] % ex.what()).str());
    }
}

std::vector<std::string> DFModel::classNames()
{
    std::vector<std::string> ret;
    for(std::map<std::string, PCreateFunc>::const_iterator it = createFuncs_.begin(); it != createFuncs_.end(); ++it)
        ret.push_back(it->first);
    return ret;
}

