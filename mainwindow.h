/**
 * Copyright (C) 2017 VIPainter
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPaintEvent>
#include <QActionGroup>
#include <QDir>
#include <QMenu>
#include <QCloseEvent>
#include <vcursortype.h>
#include "vectorgraphwidget.h"
#include "paintwidget.h"
#include "vshape.h"
#include "vdelegate.h"
#include "vlistview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void cursorChange(VCursorType type,VShape *plugin=nullptr);

protected:
    bool eventFilter(QObject * obj, QEvent * ev)override;
    void closeEvent(QCloseEvent *event)override;

private slots:
    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionResume_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionOpen_triggered();

    void on_actionMove_triggered();

    void on_actionChoose_triggered();

    void changeCursor(VCursorType type);

    void on_actionNew_triggered();

    void on_actionCanvasSize_triggered();

    void on_actionShapeSize_triggered();

    void on_actionBreakUp_triggered();

    void on_actionRotate_triggered();

    void on_actionRedo_triggered();

    void on_actionReloadPlugon_triggered();

    void on_actionLoadExPlugin_triggered();

    void on_actionAntialiasing_toggled(bool antialiasing);

    void on_actionDelete_triggered();

    void on_actionClose_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionGroup_triggered();

    void initAction(QDir dir=QDir("plugin"));

    void focusDock(bool checked);

    void on_actionSelectAll_triggered();

    void on_actionBrushColor_triggered();

    void on_actionPenColor_triggered();

    void on_actionPenStyle_triggered();

    void on_actionPen_triggered();

    void on_actionAbout_triggered();

    void on_actionUndo_triggered();

    void on_actionPolyLine_triggered();

    void on_actionCurveLine_triggered();

    void on_actionForceGroup_triggered();

private:
    QVector<VShape*>plugins;
    Ui::MainWindow *ui;

    VShape *chooseShape;
    QDockWidget *focus;
    QVector<QDockWidget *> docks;

    QActionGroup  *barGroup;
    VCursorType cursorState;
    VDelegate *delegate;
    VListView *listView;

    QMenu *contextMenu;

    void changeMenuAction(VectorgraphWidget *widget, bool flag=false);

    void loadPlugin(QString filename);

    QDockWidget * newDock(QString dockname="");

    void focusDock(QDockWidget * target);
    PaintWidget * getPaintWidget();
    PaintWidget * getPaintWidget(QDockWidget *target);
    bool closeWidget(PaintWidget *widget);
    bool openFile(QString filename);
};

#endif // MAINWINDOW_H
