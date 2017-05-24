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

#include <map>
#include "v_repExtDataflow.h"
#include "v_repPlusPlus/Plugin.h"
#include "DFEventsListener.h"
#include "DFWindow.h"
#include "UIProxy.h"
#include "DFNode.h"
#include "DFNodeFactory.h"
#include "plugin.h"
#include "stubs.h"

simInt menuItemHandle[1];
DFWindow *mainWindow;
DFEventsListener *dfEventsListener;
UIProxy *uiProxy;
std::map<QDataflowModelNode*,DFNodeID> nodeId;
std::map<DFNodeID,QDataflowModelNode*> nodeById;

void add(SScriptCallBack *p, const char *cmd, add_in *in, add_out *out)
{
    out->nodeId = nodeFactory.create(in->cmd, in->x, in->y)->id();
}

void remove(SScriptCallBack *p, const char *cmd, remove_in *in, remove_out *out)
{
    DFNode::deleteById(in->nodeId);
}

void connect(SScriptCallBack *p, const char *cmd, connect_in *in, connect_out *out)
{
    DFNode::connect(in->srcNodeId, in->srcOutlet, in->dstNodeId, in->dstInlet);
}

void disconnect(SScriptCallBack *p, const char *cmd, disconnect_in *in, disconnect_out *out)
{
    DFNode::disconnect(in->srcNodeId, in->srcOutlet, in->dstNodeId, in->dstInlet);
}

void getNodes(SScriptCallBack *p, const char *cmd, getNodes_in *in, getNodes_out *out)
{
    BOOST_FOREACH(const DFNodeID id, DFNode::nodeIds())
    {
        out->nodeIds.push_back(id);
    }
}

void getConnections(SScriptCallBack *p, const char *cmd, getConnections_in *in, getConnections_out *out)
{
    out->numConnections = DFNode::allConnections(out->srcNodeIds, out->srcOutlets, out->dstNodeIds, out->dstInlets);
}

void getNodeInfo(SScriptCallBack *p, const char *cmd, getNodeInfo_in *in, getNodeInfo_out *out)
{
    DFNode::getInfo(in->nodeId, out->cmd, out->inletCount, out->outletCount, out->x, out->y);
}

class Plugin : public vrep::Plugin
{
public:
    void onStart()
    {
        if(!registerScriptStuff())
            throw std::runtime_error("failed to register script stuff");

        simAddModuleMenuEntry("", 1, &menuItemHandle[0]);
        simSetModuleMenuItemState(menuItemHandle[0], 1, "Show dataflow graph");
        mainWindow = new DFWindow(reinterpret_cast<QWidget*>(simGetMainWindow(1)));
        uiProxy = new UIProxy();
    }

    void onInstancePass(bool objectsErased, bool objectsCreated, bool modelLoaded, bool sceneLoaded, bool undoCalled, bool redoCalled, bool sceneSwitched, bool editModeActive, bool objectsScaled, bool selectionStateChanged, bool keyPressed, bool simulationStarted, bool simulationEnded)
    {
        static bool firstInstancePass = true;
        if(firstInstancePass)
        {
            firstInstancePass = false;
            // do some initialization in SIM thread here...
            dfEventsListener = new DFEventsListener();
            initNodeFactory();
        }

        try
        {
            DFNode::tickAll();
        }
        catch(DFException &ex)
        {
            simSetLastError((boost::format("Dataflow:%d") % ex.node()->id()).str().c_str(), ex.what());
        }
    }

    void onMenuItemSelected(int itemHandle, int itemState)
    {
        if(itemHandle == menuItemHandle[0])
        {
            if(mainWindow->isVisible()) mainWindow->hide();
            else mainWindow->show();
            simSetModuleMenuItemState(menuItemHandle[0], mainWindow->isVisible() ? 3 : 1, "Show dataflow graph");
        }
    }
};

VREP_PLUGIN("Dataflow", 1, Plugin)
