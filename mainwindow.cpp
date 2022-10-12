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
#include <QMessageBox>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    gWidget = new QWidget(this);
    this->setCentralWidget(gWidget);
    gridLayout = new QGridLayout();

#ifdef USE_DEFAULT_MENU_BAR
    this->pMenuBar = this->menuBar();
#else
    this->pMenuBar = new QMenuBar(this);
#endif

    this->pMenuGame = new QMenu("Game", this);
    this->pMenuSetting = new QMenu("Setting", this);
    this->pMenuPlayer = new QMenu("Player", this);
    this->pMenuAbout = new QMenu("About", this);
    this->pActionBoardSize = new QAction("Board Size", this);
    this->pActionStart = new QAction("Start", this);
    this->pActionEnd = new QAction("End", this);
    this->pMenuSetting->addAction(this->pActionBoardSize);
    this->pMenuGame->addAction(this->pActionStart);
    this->pMenuGame->addAction(this->pActionEnd);
    this->pMenuBar->addMenu(this->pMenuGame);
    this->pMenuBar->addMenu(this->pMenuSetting);
    this->pMenuBar->addMenu(this->pMenuPlayer);
    this->pMenuBar->addMenu(this->pMenuAbout);

#ifndef USE_DEFAULT_MENU_BAR
    setMenuBar(this->pMenuBar);
#endif

    this->gWidget->setLayout(this->gridLayout);

    this->mBoard = new Board();
    this->mBoard->setBSize(15);

    resize(this->mBoard->getBSize() * RECT_WIDTH, this->mBoard->getBSize() * RECT_HEIGHT + this->pMenuBar->height());

    mIsBlackTurn = true;

    this->freeStyleGomoku = new FreeStyleGomoku();
}

MainWindow::~MainWindow()
{
    if (nullptr != this->freeStyleGomoku)
    {
        delete  this->freeStyleGomoku;
        this->freeStyleGomoku = nullptr;
    }
    if (nullptr != this->mBoard)
    {
        delete this->mBoard;
        this->mBoard = nullptr;
    }
    if (nullptr != this->pActionStart)
    {
        delete this->pActionStart;
        this->pActionStart = nullptr;
    }
    if (nullptr != this->pActionEnd)
    {
        delete this->pActionEnd;
        this->pActionEnd = nullptr;
    }
    if (nullptr != this->pActionBoardSize)
    {
        delete this->pActionBoardSize;
        this->pActionBoardSize = nullptr;
    }
    if (nullptr != this->pMenuGame)
    {
        delete this->pMenuGame;
        this->pMenuGame = nullptr;
    }
    if (nullptr != this->pMenuPlayer)
    {
        delete this->pMenuPlayer;
        this->pMenuPlayer = nullptr;
    }
    if (nullptr != this->pMenuSetting)
    {
        delete this->pMenuSetting;
        this->pMenuSetting = nullptr;
    }
    if (nullptr != this->pMenuAbout)
    {
        delete this->pMenuAbout;
        this->pMenuAbout = nullptr;
    }
    if (nullptr != this->pMenuBar)
    {
        delete this->pMenuBar;
        this->pMenuBar = nullptr;
    }
    if (nullptr != this->gridLayout)
    {
        delete this->gridLayout;
        this->gridLayout = nullptr;
    }
    if (nullptr != this->gWidget)
    {
        delete this->gWidget;
        this->gWidget = nullptr;
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    DrawChessboard();
    DrawItems();
    DrawItemWithMouse();

    update();
}

void MainWindow::DrawChessboard()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(QColor(Qt::black),2));

    for(int i = 0; i < this->mBoard->getBSize() - 1; ++i)
    {
        for (int j = 0; j < this->mBoard->getBSize() - 1; ++j)
            painter.drawRect( (i+0.5)*RECT_WIDTH,(j+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
    }
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));

    for (int i = 0; i < this->mBoard->getVRecord().size(); i++)
    {
        QPoint p;
        if (this->mBoard->getVRecord().at(i).second == BLACK)
        {
            painter.setBrush(Qt::black);
        }
        else
        {
            painter.setBrush(Qt::white);
        }
        p.setX(this->mBoard->coord2idx(this->mBoard->getVRecord().at(i).first).first);
        p.setY(this->mBoard->coord2idx(this->mBoard->getVRecord().at(i).first).second);
        DrawChessAtPoint(painter, p);
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

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    QPoint pt;
    pt.setX( (e->pos().x() ) / RECT_WIDTH);
    pt.setY( (e->pos().y() ) / RECT_HEIGHT);

    pair<int, int> p_idx(pt.x(), pt.y());

    bool b_succ = false;
    if (this->mBoard->getVRecord().size() < this->mBoard->getMaxRecordSize())
    {
        if (mIsBlackTurn)
        {
            b_succ = this->mBoard->placeStone(p_idx, BLACK);
        }
        else
        {
            b_succ = this->mBoard->placeStone(p_idx, WHITE);
        }
    }

    //if connect five
    if (this->freeStyleGomoku->checkWin(this->mBoard))
    {
        if (this->mIsBlackTurn)
            QMessageBox::information(this, "game over!", "Black win!");
        else
            QMessageBox::information(this, "game over!", "White win!");
        this->mBoard->clearBoard();
        return ;
    }

    if (b_succ)
        mIsBlackTurn = !mIsBlackTurn;
}

