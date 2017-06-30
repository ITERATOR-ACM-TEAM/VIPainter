#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vdocktitlebar.h"
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
    ui(new Ui::MainWindow),focus(nullptr)
{
    ui->setupUi(this);
    delete takeCentralWidget();
    setDockNestingEnabled(true);
    setWindowTitle("VIPainter");
    group = new QActionGroup(this);
    group->addAction(ui->actionChoose);
    group->addAction(ui->actionMove);
    connect(this, SIGNAL(cursorChange(int)), this, SLOT(changeCursor(int)));
    update();
}

QDockWidget* MainWindow::newDock()
{
    QDockWidget *old = focus;

    static int id = 0;

    TestWidget* newWidget=new TestWidget(this);
    connect(this, SIGNAL(cursorChange(int)), newWidget, SLOT(changeCursor(int)));
    emit cursorChange(this->cursorState);

    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->setWidget(newWidget);
    //dockWidget->setAttribute(Qt::WA_DeleteOnClose);
    this->addDockWidget(Qt::TopDockWidgetArea,dockWidget);

    dockWidget->setFocusPolicy(Qt::StrongFocus);
    dockWidget->installEventFilter(this);

    dockWidget->show();
    if(old != nullptr && !old->isFloating())
    {
        this->tabifyDockWidget(old, dockWidget);
    }
    else
    {
        for(auto it: docks)
        {
            if(!it->isFloating())
            {
                this->tabifyDockWidget(it, dockWidget);
            }
        }
    }
    //dockWidget->setTitleBarWidget(new VDockTitleBar());
    dockWidget->setBaseSize(400,300);

    this->focusDock(dockWidget);

//    connect(dockWidget->toggleViewAction(), &QAction::toggled,
//            [dockWidget](bool checked){
//        qDebug()<<"triggled"<<checked;
//        dockWidget->raise();
//        dockWidget->setFocus();
//        dockWidget->activateWindow();
//    });

    dockWidget->setWindowTitle(QString("untitled %1").arg(id++));
    docks.push_back(dockWidget);
    return dockWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{

    getTestWidget()->scale*=1.1;
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    getTestWidget()->scale/=1.1;
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionResume_triggered()
{
    getTestWidget()->scale=1.0;
    getTestWidget()->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<getTestWidget()->scale<<endl;
    getTestWidget()->update();
}

void MainWindow::on_actionSave_triggered()
{
}

void MainWindow::saveFile(QString filename)
{
    if(focus == nullptr) return;
    if(filename.split('.').back()==tr("vp"))
    {
        QJsonDocument jsonDocument;
        jsonDocument.setObject(getTestWidget()->groupShape.toJsonObject());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
    }
    else
    {
        QImage image(getTestWidget()->canvasSize.width,getTestWidget()->canvasSize.height,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(getTestWidget()->canvasSize.width/2,getTestWidget()->canvasSize.height/2);
        getTestWidget()->groupShape.draw(&painter,VMagnification(1,1));
        image.save(filename);
    }
    focus->setWindowTitle(filename.split("/").back());
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
    QDockWidget * newWidget = newDock();
    getTestWidget(newWidget)->groupShape=QJsonDocument::fromJson(file.readAll()).object();
    newWidget->setWindowTitle(filename.split("/").back());
    file.close();
    newWidget->update();
}

void MainWindow::on_actionTestShape1_triggered()
{
    QString filename = "plugin/testShape1.vp";
    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    VGroupShape * gs= new VGroupShape(QJsonDocument::fromJson(file.readAll()).object());
    if(focus==nullptr)return;
    qDebug() << focus ;
    getTestWidget()->groupShape.insertShape(gs);
    file.close();
    getTestWidget()->update();
}

void MainWindow::on_actionMove_triggered()
{
    if(ui->actionMove->isChecked())
        emit cursorChange(VCursorType::MOVE);
    else
        emit cursorChange(VCursorType::CHOOSE);
}

void MainWindow::on_actionChoose_triggered()
{
    if(ui->actionChoose->isChecked())
        emit cursorChange(VCursorType::CHOOSE);
    else
        emit cursorChange(VCursorType::CHOOSE);
}

void MainWindow::changeCursor(int type)
{
    cursorState = type;
}

void MainWindow::on_actionNew_triggered()
{
    newDock();
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev)
{
    for(auto it = docks.begin(); it != docks.end(); it++)
    {
        if(obj == *it)
        {
            if(ev->type() == QEvent::FocusIn)
            {
                qDebug() << "focusing";
                focus = *it;
                return false;
            }else if(ev->type() == QEvent::Close)
            {
                qDebug() << "closing";
                delete *it;
                docks.erase(it);
                focus = nullptr;
                if(!docks.empty())
                {
                    this->focusDock(docks.back());
                }
                qDebug() << docks.size();
                return true;
            }
        }
    }

    return false;
}


void MainWindow::focusDock(QDockWidget * target)
{
    target->raise();
    target->setFocus();
    qApp->postEvent(target, new QFocusEvent(QEvent::FocusIn));
//    qDebug() << "focusing";
//    focus = target;
}

TestWidget * MainWindow::getTestWidget()
{
    return qobject_cast<TestWidget *>(focus->widget());
}

TestWidget * MainWindow::getTestWidget(QDockWidget * target)
{
    return qobject_cast<TestWidget *>(target->widget());
}
