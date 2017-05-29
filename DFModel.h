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

#ifndef DFMODEL_H_INCLUDED
#define DFMODEL_H_INCLUDED

#include <QObject>
#include "qdataflowmodel.h"

class DFNode;

class DFModel : public QDataflowModel
{
    Q_OBJECT
public:
    DFModel(QObject *parent = 0);

    void loadGraph(std::string s);
    std::string saveGraph();
    void clearGraph();
    void saveGraphToScene();
    void restoreGraphFromScene();

protected:
    virtual QDataflowModelNode * newNode(QPoint pos, QString text, int inletCount, int outletCount);

public slots:
    void onNodeAdded(QDataflowModelNode *node);
    void onNodeRemoved(QDataflowModelNode *node);
    void onNodeTextChanged(QDataflowModelNode *node, QString text);
    void onConnectionAdded(QDataflowModelConnection *conn);
    void onConnectionRemoved(QDataflowModelConnection *conn);
    void onGraphChanged();

signals:
    void graphChanged();

private:
    bool blockGraphChangeSignal;

    // object factory:

    template<typename T>
    void registerClass(std::string name)
    {
        createFuncs_[name] = &createFunc<T>;
    }

    DFNode * createDFMetaObject(QDataflowModelNode *node, const std::string &args);
    DFNode * createDFMetaObject(QDataflowModelNode *node, const std::vector<std::string> &args);

    template<typename T>
    static DFNode * createFunc(QDataflowModelNode *node, const std::vector<std::string> &args)
    {
        return new T(node, args);
    }

    typedef DFNode* (*PCreateFunc)(QDataflowModelNode *, const std::vector<std::string> &);
    std::map<std::string, PCreateFunc> createFuncs_;

public:
    std::vector<std::string> classNames();
};

extern DFModel *dfModel;

#endif // DFMODEL_H_INCLUDED
