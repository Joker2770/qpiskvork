/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
    Copyright (C) 2022-2023  Jintao Yang <yjt950840@outlook.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include "customs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator qts;
    QString q_lang;
    Customs g_customs("qpiskvork");
    g_customs.getCfgValue("View", "language", q_lang);
    if (0 == QString::compare(q_lang, "zh_CN"))
    {
        qts.load(":/language/zh_CN.qm");
        a.installTranslator(&qts);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
