/**
 * Copyright (C) 2017 kkkeQAQ
 *               2017 Bcai0797
 *               2017 Penn000
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if defined(Q_OS_WIN32)
#elif defined(Q_OS_LINUX)
    QDir home(QDir::home());
    QString workdir=".VIPainter";
    if(!QDir(home.filePath(workdir)).exists())home.mkdir(workdir);
//    qDebug()<<home.filePath(workdir);
    QDir::setCurrent(home.filePath(workdir));
//    qDebug()<<QDir::currentPath();
#endif

    MainWindow w;
    w.show();

#if defined(Q_OS_WIN32)
    QDesktopWidget *deskdop = QApplication::desktop();
    w.move((deskdop->width() - w.width())/2, (deskdop->height() - w.height())/2);
#elif defined(Q_OS_LINUX)
    QDesktopWidget *dw = QApplication::desktop();
    QRect parentRect = dw->availableGeometry(dw->primaryScreen());

    if (w.parentWidget()) {
        parentRect = w.parentWidget()->geometry();
    }

    w.move(parentRect.center() - w.geometry().center());
#endif

    return a.exec();
}
