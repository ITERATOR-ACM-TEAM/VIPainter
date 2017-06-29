#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "testwidget.h"

class TestWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionResume_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionOpen_triggered();

    void on_actionTestShape1_triggered();

private:
    static const int STATE_MOVE;
    static const int STATE_CHOOSE;

    Ui::MainWindow *ui;
    TestWidget *testWidget;
    int cursorState;
    void newDock();
    void saveFile(QString filename);
};

#endif // MAINWINDOW_H
