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

const int MainWindow::STATE_CHOOSE = 0;
const int MainWindow::STATE_MOVE = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);
    setWindowTitle("VIPainter");
    group = new QActionGroup(this);
    group->addAction(ui->actionChoose);
    group->addAction(ui->actionMove);

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

void MainWindow::on_actionTestShape1_triggered()
{
    QString filename = "F:\\OOC\\VIPainter\\plugin\\testShape1.vp";
    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    VGroupShape * gs= new VGroupShape(QJsonDocument::fromJson(file.readAll()).object());
    testWidget->groupShape.insertShape(gs);
    file.close();
    testWidget->update();
}

int MainWindow::getCursonState()
{
    if(ui->actionChoose->isChecked())
    {
        return STATE_CHOOSE;
    }else if(ui->actionMove->isChecked())
    {
        return STATE_MOVE;
    }
    return -1;
}


void MainWindow::on_actionMove_triggered()
{
    if(ui->actionMove->isChecked())
        testWidget->setCursor(QCursor(Qt::OpenHandCursor));
    else
        testWidget->setCursor(Qt::ArrowCursor);
}

void MainWindow::on_actionChoose_triggered()
{
    if(ui->actionChoose->isChecked())
        testWidget->setCursor(QCursor(Qt::ArrowCursor));
    else
        testWidget->setCursor(Qt::ArrowCursor);
}
