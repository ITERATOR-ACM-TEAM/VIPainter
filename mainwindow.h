#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPaintEvent>
#include <QActionGroup>
#include <QDir>
#include <vcursortype.h>
#include "testwidget.h"
#include "vshape.h"

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
    void cursorChange(VCursorType type);

protected:
    bool eventFilter(QObject * obj, QEvent * ev)override;

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

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void initAction(QDir dir=QDir("plugin"));

    void focusDock(bool checked);

    void on_actionReloadPlugon_triggered();

    void on_actionLoadExPlugin_triggered();

    void on_actionAntialiasing_toggled(bool antialiasing);

    void on_actionDelete_triggered();

    void on_actionClose_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

private:
    QVector<VShape*>plugins;
    Ui::MainWindow *ui;

    VShape *chooseShape;
    QDockWidget *focus;
    QVector<QDockWidget *> docks;

    QActionGroup  *group;
    VCursorType cursorState;

    void loadPlugin(QString filename);

    QDockWidget * newDock(QString dockname="");
    void saveFile(QString filename);

    void focusDock(QDockWidget * target);
    TestWidget * getTestWidget();
    TestWidget * getTestWidget(QDockWidget * target);
};

#endif // MAINWINDOW_H
