#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QDesktopWidget *dw = QApplication::desktop();
    QRect parentRect = dw->availableGeometry(dw->primaryScreen());

    if (w.parentWidget()) {
        parentRect = w.parentWidget()->geometry();
    }

    w.move(parentRect.center() - w.geometry().center());

    return a.exec();
}
