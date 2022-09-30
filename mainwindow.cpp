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

#include <QMessageBox>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize((CHESS_COLUMES + 1)*RECT_WIDTH  ,(CHESS_ROWS + 1)*RECT_HEIGHT);
    mIsBlackTurn = true;
}

MainWindow::~MainWindow()
{
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    DrawChessboard();
    DrawItemWithMouse();
    DrawItems();

    update();
}

void MainWindow::DrawChessboard()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(QColor(Qt::black),2));

    for(int i = 0;i<CHESS_COLUMES; i++)
    {
        for (int j = 0; j<CHESS_ROWS; j++)
        {
            painter.drawRect( (i+0.5)*RECT_WIDTH,(j+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
        }
    }
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));

    for (int i = 0; i<mItems.size(); i++)
    {
        Item item = mItems[i];
        if (item.mBlack)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::white);
        }
        DrawChessAtPoint(painter,item.mPt);
    }
}

void MainWindow::DrawChessAtPoint(QPainter& painter,QPoint& pt)
{
    //painter.drawRect( (pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);

    QPoint ptCenter((pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT);
    painter.drawEllipse(ptCenter,RECT_WIDTH / 2,RECT_HEIGHT / 2);
}

void MainWindow::DrawItemWithMouse()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));

    if (mIsBlackTurn)
    {
        painter.setBrush(Qt::black);
    }
    else
    {
        painter.setBrush(Qt::white);
    }
    //QPoint pt;
    //pt.setX( (QCursor::pos().x() ) / RECT_WIDTH);
    //pt.setY( (QCursor::pos().y() ) / RECT_HEIGHT);

    //DrawChessAtPoint(painter,pt);

    painter.drawEllipse(mapFromGlobal(QCursor::pos()),RECT_WIDTH / 2,RECT_HEIGHT / 2);

}
