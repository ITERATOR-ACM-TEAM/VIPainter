#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QScrollArea>
#include <QDockWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);

    newDock();
    update();
}

void MainWindow::newDock()
{
    testWidget=new TestWidget;
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
    qDebug()<<"scale:"<<testWidget->scale<<endl;
    testWidget->update();
}
