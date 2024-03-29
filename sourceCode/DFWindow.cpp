#include "DFWindow.h"
#include "DFModel.h"
#include "stubs.h"

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
    int d[4];
    d[0] = g.x();
    d[1] = g.y();
    d[2] = g.width();
    d[3] = g.height();
    simWriteCustomDataBlock(sim_handle_scene, "Dataflow.Window.Geometry", (char*)&d[0], sizeof(int) * 4);
}

void DFWindow::restoreGeometryFromScene()
{
    int size = 0;
    char *data = simReadCustomDataBlock(sim_handle_scene, "Dataflow.Window.Geometry", &size);
    if(data)
    {
        sim::addLog(sim_verbosity_debug, "found a Dataflow.Window.Geometry block");
        int *d = (int*)data;
        setGeometry(QRect(d[0], d[1], d[2], d[3]));
    }
    else
    {
        sim::addLog(sim_verbosity_debug, "the scene does not have a Dataflow.Window.Geometry block");
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

