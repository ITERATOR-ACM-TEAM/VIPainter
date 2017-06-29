#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QActionGroup>
#include "testwidget.h"

class TestWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int STATE_MOVE;
    static const int STATE_CHOOSE;

    explicit MainWindow(QWidget *parent = 0);
    int getCursonState();
    ~MainWindow();
private slots:
    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionResume_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionOpen_triggered();

    void on_actionTestShape1_triggered();

    void on_actionMove_triggered();

    void on_actionChoose_triggered();

private:
    Ui::MainWindow *ui;
    TestWidget *testWidget;
    QActionGroup  *group;
    int cursorState;
    void newDock();
    void saveFile(QString filename);
};

#endif // MAINWINDOW_H
