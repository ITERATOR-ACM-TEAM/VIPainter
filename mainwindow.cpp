#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QScrollArea>
#include <QDockWidget>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);
    setWindowTitle("VIPainter");

    newDock();
    update();
}

void MainWindow::newDock()
{
    testWidget=new TestWidget(this);
    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->setWidget(testWidget);
    this->addDockWidget(Qt::TopDockWidgetArea,dockWidget);
    dockWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{
    testWidget->scale*=1.1;
    qDebug()<<"scale:"<<testWidget->scale<<endl;
    testWidget->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    testWidget->scale/=1.1;
    qDebug()<<"scale:"<<testWidget->scale<<endl;
    testWidget->update();
}

void MainWindow::on_actionResume_triggered()
{
    testWidget->scale=1.0;
    testWidget->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<testWidget->scale<<endl;
    testWidget->update();
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::saveFile(QString filename)
{
    if(filename.split('.').back()==tr("vp"))
    {
        QJsonDocument jsonDocument;
        jsonDocument.setObject(testWidget->groupShape.toJsonObject());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
    }
    else
    {
        QImage image(testWidget->canvasSize.x,testWidget->canvasSize.y,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(testWidget->canvasSize.x/2,testWidget->canvasSize.y/2);
        testWidget->groupShape.draw(&painter);
        image.save(filename);
    }
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename=
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         tr("image.vp"),
                                         tr("vp file (*.vp);;"
                                            "png file (*.png);;"
                                            "jpg file (*.jpg);;"
                                            "bmp file (*.bmp)"));
    saveFile(filename);
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename=
            QFileDialog::getOpenFileName(this,
                                         tr("Open File"),
                                         tr(""),
                                         tr("vp file (*.vp)"));

    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    testWidget->groupShape=QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    testWidget->update();
}
