#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include "vgroupshape.h"
#include "vellipse.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*)override;

private slots:
    void on_BtWrite_clicked();

    void on_BtRead_clicked();

private:
    Ui::MainWindow *ui;
    VGroupShape canvas;

    VEllipse ellipse;
};

#endif // MAINWINDOW_H
