/**
    qpiskvork is another gomoku manager for adapting to Windows and Linux systems.
    Copyright (C) 2022  Jintao Yang <yjt950840@outlook.com>

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QMap>

#include "board.h"

#define CHESS_ROWS		15
#define CHESS_COLUMES	15
#define RECT_WIDTH		50
#define RECT_HEIGHT		50

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *gWidget;
    QGridLayout *gridLayout;
    QMenuBar *pMenuBar;
    QMenu *pMenuSetting;
    QMenu *pMenuPlayer;
    QAction *pActionBoardSize;

//public slots:
//    void OnActionBoardSize();

protected:
    void paintEvent(QPaintEvent *);

private:
    void DrawChessboard();
    void DrawItems();
    void DrawItemWithMouse();

    void DrawChessAtPoint(QPainter& painter,QPoint& pt);

private:
    Board *mBoard;
    bool mIsBlackTurn;
};
#endif // MAINWINDOW_H
