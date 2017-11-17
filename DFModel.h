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
