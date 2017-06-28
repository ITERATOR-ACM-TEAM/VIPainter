#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "testwidget.h"

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

private:
    Ui::MainWindow *ui;
    TestWidget *testWidget;
    void newDock();
};

#endif // MAINWINDOW_H
