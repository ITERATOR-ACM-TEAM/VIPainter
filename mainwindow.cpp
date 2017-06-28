#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include "vellipse.h"
#include "vpolyline.h"
#include "vsize.h"
#include <QWidget>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vpolyline.setSize(VSize(40.0,50.0));
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(100,200);
//    ellipse.draw(&painter);
    vpolyline.draw(&painter);
}

void MainWindow::on_BtWrite_clicked()
{
    QFile file("heiheihei.vp");
    file.open(QFile::WriteOnly|QFile::Text);
    QJsonDocument doc;
//    doc.setObject(ellipse.toJsonObject());
    doc.setObject(vpolyline.toJsonObject());
    file.write(doc.toJson());
    file.close();
}

void MainWindow::on_BtRead_clicked()
{
    QFile file("haha.vp");
    file.open(QFile::ReadOnly|QFile::Text);
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
//    VEllipse *ell=new VEllipse(doc.object());
//    ellipse=*ell;
//    delete ell;
//    VPolyline pl = new VPolyline(doc.object());
//    vpolyline = *pl;
//    delete pl;
//    file.close();
//    update();
}

void MainWindow::on_BtSave_clicked()
{
//    QImage image(ellipse.getSize().x+4,ellipse.getSize().y+4,QImage::Format_ARGB32);
    QImage image(vpolyline.getSize().x+4,vpolyline.getSize().y+4,QImage::Format_ARGB32);
    image.fill(0);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.translate(2+ellipse.getSize().x/2,2+ellipse.getSize().y/2);
//    ellipse.draw(&painter);
    painter.translate(2+vpolyline.getSize().x/2,2+vpolyline.getSize().y/2);
    vpolyline.draw(&painter);
    image.save("file.png");
}
