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
    group = new QActionGroup(this);
    group->addAction(ui->actionChoose);
    group->addAction(ui->actionMove);
    connect(this, SIGNAL(cursorChange(int)), this, SLOT(changeCursor(int)));
    focus = newDock();
    widgetVector.push_back(focus);
    update();
}

TestWidget* MainWindow::newDock()
{
    TestWidget* newWidget=new TestWidget(this);
    connect(this, SIGNAL(cursorChange(int)), newWidget, SLOT(changeCursor(int)));
    emit cursorChange(this->cursorState);
    QDockWidget *dockWidget=new QDockWidget;
    dockWidget->setWidget(newWidget);
    this->addDockWidget(Qt::TopDockWidgetArea,dockWidget);

    newWidget->setFocusPolicy(Qt::StrongFocus);
    newWidget->installEventFilter(this);

    dockWidget->show();

    return newWidget;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionZoomIn_triggered()
{
    focus->scale*=1.1;
    qDebug()<<"scale:"<<focus->scale<<endl;
    focus->update();
}

void MainWindow::on_actionZoomOut_triggered()
{
    focus->scale/=1.1;
    qDebug()<<"scale:"<<focus->scale<<endl;
    focus->update();
}

void MainWindow::on_actionResume_triggered()
{
    focus->scale=1.0;
    focus->canvasLocation=VPoint(0,0);
    qDebug()<<"scale:"<<focus->scale<<endl;
    focus->update();
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
        jsonDocument.setObject(focus->groupShape.toJsonObject());
        QFile file(filename);
        file.open(QFile::WriteOnly|QFile::Text);
        file.write(jsonDocument.toJson());
        file.close();
    }
    else
    {
        QImage image(focus->canvasSize.x,focus->canvasSize.y,QImage::Format_ARGB32);
        image.fill(0x00ffffff);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.translate(focus->canvasSize.x/2,focus->canvasSize.y/2);
        focus->groupShape.draw(&painter);
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
    TestWidget * newWidget = newDock();
    widgetVector.push_back(newWidget);
    newWidget->groupShape=QJsonDocument::fromJson(file.readAll()).object();
    file.close();
    newWidget->update();
}

void MainWindow::on_actionTestShape1_triggered()
{
    QString filename = "F:\\OOC\\VIPainter\\plugin\\testShape1.vp";
    QFile file(filename);
    file.open(QFile::ReadOnly|QFile::Text);
    VGroupShape * gs= new VGroupShape(QJsonDocument::fromJson(file.readAll()).object());
    focus->groupShape.insertShape(gs);
    file.close();
    focus->update();
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
    widgetVector.push_back(newDock());
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev)
{
    for(auto it = widgetVector.begin(); it != widgetVector.end(); it++)
    {
        if(obj == *it)
        {
            if(ev->type() == QEvent::FocusIn)
            {
                qDebug() << "focusing";
                focus = *it;
                return false;
            }else if(ev->type() == QEvent::Hide)
            {
                qDebug() << "closing";
                //delete *it;
                widgetVector.erase(it);
                if(widgetVector.empty()) focus = nullptr;
                else widgetVector.back()->setFocus();
                return true;
            }

        }
    }

    return QWidget::eventFilter(obj, ev);
}
