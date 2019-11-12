#include "DFWindow.h"
#include "DFModel.h"
#include "simLib.h"
#include "debug.h"

#include <boost/foreach.hpp>

DFWindow *mainWindow = 0L;

DFWindow::DFWindow(QWidget *parent)
    : QMainWindow(parent)
{
    canvas = new QDataflowCanvas(this);
    canvas->setCompletion(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(canvas);

    QWidget *window = new QWidget();
    window->setLayout(layout);

    setCentralWidget(window);
}

void DFWindow::moveEvent(QMoveEvent* event)
{
    QMainWindow::moveEvent(event);

    saveGeometryToScene();
}

void DFWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    saveGeometryToScene();
}

void DFWindow::saveGeometryToScene()
{
    QRect g = geometry();
    simInt d[4];
    d[0] = g.x();
    d[1] = g.y();
    d[2] = g.width();
    d[3] = g.height();
    simWriteCustomDataBlock(sim_handle_scene, "Dataflow.Window.Geometry", (simChar*)&d[0], sizeof(simInt) * 4);
}

void DFWindow::restoreGeometryFromScene()
{
    simInt size = 0;
    simChar *data = simReadCustomDataBlock(sim_handle_scene, "Dataflow.Window.Geometry", &size);
    if(data)
    {
        DEBUG_OUT << "found a Dataflow.Window.Geometry block" << std::endl;
        simInt *d = (simInt*)data;
        setGeometry(QRect(d[0], d[1], d[2], d[3]));
    }
    else
    {
        DEBUG_OUT << "the scene does not have a Dataflow.Window.Geometry block" << std::endl;
    }
}

void DFWindow::complete(QString txt, QStringList &completionList)
{
    BOOST_FOREACH(const std::string &className, dfModel->classNames())
    {
        QString className_ = QString::fromStdString(className);
        if(className_.startsWith(txt))
            completionList << className_;
    }
}

