#ifndef DFWINDOW_H_INCLUDED
#define DFWINDOW_H_INCLUDED

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include "qdataflowcanvas.h"

class DFWindow : public QMainWindow, public QDataflowTextCompletion
{
public:
    DFWindow(QWidget *parent);

    void moveEvent(QMoveEvent* event);
    void resizeEvent(QResizeEvent* event);

    void saveGeometryToScene();
    void restoreGeometryFromScene();

    void complete(QString txt, QStringList &completionList);

    QDataflowCanvas *canvas;
};

extern DFWindow *mainWindow;

#endif // DFWINDOW_H_INCLUDED
