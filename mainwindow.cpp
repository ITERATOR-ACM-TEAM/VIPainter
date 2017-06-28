#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include "vellipse.h"
#include "vsize.h"
#include <QWidget>
#include <QFile>
#include <QScrollArea>
#include <QDockWidget>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);

    newDock();
    newDock();
    newDock();
    newDock();
    update();
}

void MainWindow::newDock()
{
    testWidget=new TestWidget;
    QDockWidget *dockWidget=new QDockWidget;
    QScrollArea *scrollArea=new QScrollArea;
    dockWidget->setWidget(scrollArea);
    scrollArea->setWidget(testWidget);
    this->addDockWidget(Qt::TopDockWidgetArea,dockWidget);
    dockWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(100,200);
    ellipse.draw(&painter);
}

void MainWindow::on_BtWrite_clicked()
{
    QFile file("heiheihei.vp");
    file.open(QFile::WriteOnly|QFile::Text);
    QJsonDocument doc;
    doc.setObject(ellipse.toJsonObject());
    file.write(doc.toJson());
    file.close();
}

void MainWindow::on_BtRead_clicked()
{
    QFile file("heiheihei.vp");
    file.open(QFile::ReadOnly|QFile::Text);
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
    VEllipse *ell=new VEllipse(doc.object());
    ellipse=*ell;
    delete ell;
    file.close();
    update();
}

void MainWindow::on_BtSave_clicked()
{
    QImage image(ellipse.getSize().x+4,ellipse.getSize().y+4,QImage::Format_ARGB32);
    image.fill(0);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.translate(2+ellipse.getSize().x/2,2+ellipse.getSize().y/2);
    ellipse.draw(&painter);
    image.save("file.png");
}
*/
