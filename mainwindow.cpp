#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QScrollArea>
#include <QDockWidget>

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
