#include <map>
#include "v_repPlusPlus/Plugin.h"
#include "DFModel.h"
#include "DFWindow.h"
#include "DFNode.h"
#include "plugin.h"
#include "stubs.h"

simInt menuItemHandle[1];
std::map<QDataflowModelNode*,DFNodeID> nodeId;
std::map<DFNodeID,QDataflowModelNode*> nodeById;
bool pendingSceneLoad = false;

void add(SScriptCallBack *p, const char *cmd, add_in *in, add_out *out)
{
    QPoint pos(in->x, in->y);
    QString text(QString::fromStdString(in->cmd));
    QDataflowModelNode *node = dfModel->create(pos, text, 0, 0);
    DFNode *dfnode = dynamic_cast<DFNode*>(node->dataflowMetaObject());
    if(dfnode)
        out->nodeId = dfnode->id();
    else
        out->nodeId = -1;
}

void remove(SScriptCallBack *p, const char *cmd, remove_in *in, remove_out *out)
{
    DFNode *dfnode = DFNode::byId(in->nodeId);
    dfModel->remove(dfnode->node());
}

void connect(SScriptCallBack *p, const char *cmd, connect_in *in, connect_out *out)
{
    DFNode *src = DFNode::byId(in->srcNodeId);
    DFNode *dst = DFNode::byId(in->dstNodeId);
    dfModel->connect(src->node(), in->srcOutlet, dst->node(), in->dstInlet);
}

void disconnect(SScriptCallBack *p, const char *cmd, disconnect_in *in, disconnect_out *out)
{
    DFNode *src = DFNode::byId(in->srcNodeId);
    DFNode *dst = DFNode::byId(in->dstNodeId);
    dfModel->disconnect(src->node(), in->srcOutlet, dst->node(), in->dstInlet);
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
    out->numConnections = 0;
    // TODO
}

void getNodeInfo(SScriptCallBack *p, const char *cmd, getNodeInfo_in *in, getNodeInfo_out *out)
{
    //DFNode::getInfo(in->nodeId, out->cmd, out->inletCount, out->outletCount, out->x, out->y);
    // TODO
}

void initInUiThread()
{
    uiThread();
    DBG << std::endl;

    if(!registerScriptStuff())
        throw std::runtime_error("failed to register script stuff");

    simAddModuleMenuEntry("", 1, &menuItemHandle[0]);
    simSetModuleMenuItemState(menuItemHandle[0], 1, "Show dataflow graph");

    DBG << "create main window..." << std::endl;
    mainWindow = new DFWindow(reinterpret_cast<QWidget*>(simGetMainWindow(1)));
}

void initInSimThread()
{
    if(dfModel) return;

    simThread();

    DBG << "init DFModel..." << std::endl;
    dfModel = new DFModel();

    DBG << "set QDataflowCanvas model..." << std::endl;
    mainWindow->canvas->setModel(dfModel);
}

class Plugin : public vrep::Plugin
{
public:
    void onStart()
    {
        initInUiThread();

        simSetModuleInfo(PLUGIN_NAME, 0, "Dataflow Plugin", 0);
        simSetModuleInfo(PLUGIN_NAME, 1, __DATE__, 0);
    }

    void onInstancePass(vrep::InstancePassFlags &flags, bool first)
    {
        if(first)
        {
            initInSimThread();
        }

        if(pendingSceneLoad)
        {
            pendingSceneLoad = false;
            dfModel->restoreGraphFromScene();
            mainWindow->restoreGeometryFromScene();
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

    void onInstanceSwitch(int sceneID)
    {
        DBG << "sceneID=" << sceneID << std::endl;
        dfModel->restoreGraphFromScene();
        mainWindow->restoreGeometryFromScene();
    }

    void onSceneLoaded()
    {
        DBG << std::endl;
        pendingSceneLoad = true;
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

VREP_PLUGIN(PLUGIN_NAME, PLUGIN_VERSION, Plugin)
