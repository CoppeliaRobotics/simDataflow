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

#include "DFWindow.h"
#include "DFNodeFactory.h"
#include "v_repLib.h"
#include "debug.h"

#include <boost/foreach.hpp>

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
        DBG << "found a Dataflow.Window.Geometry block" << std::endl;
        simInt *d = (simInt*)data;
        setGeometry(QRect(d[0], d[1], d[2], d[3]));
    }
    else
    {
        DBG << "the scene does not have a Dataflow.Window.Geometry block" << std::endl;
    }
}

void DFWindow::complete(QString txt, QStringList &completionList)
{
    BOOST_FOREACH(const std::string &className, nodeFactory.classNames())
    {
        QString className_ = QString::fromStdString(className);
        if(className_.startsWith(txt))
            completionList << className_;
    }
}

