#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vgroupshape.h"
#include "vpolyline.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    void on_BtWrite_clicked();
    void on_BtRead_clicked();
    void on_BtSave_clicked();
private:
    Ui::MainWindow *ui;
    VGroupShape canvas;
    VPolyline vpolyline;
};

#endif // MAINWINDOW_H
