/**
    qpiskvork is another gomoku manager for adapting to Windows and Linux systems.
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

#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "EngineLoader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_player_setting(new PlayerSettingDialog(this))
{
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
    this->pActionPause = new QAction("Pause", this);
    this->pActionContinue = new QAction("Continue", this);
    this->pActionEnd = new QAction("End", this);
    this->pActionClear = new QAction("Clear", this);
    this->pActionTakeBack = new QAction("Take Back", this);
    this->pActionFreeStyleGomoku = new QAction("Free-style Gomoku", this);
    this->pActionStandardGomoku = new QAction("Standard Gomoku", this);
    this->pActionPlayerSetting = new QAction("Setting", this);
    this->pActionVer = new QAction("Ver Info", this);
    this->pDialogBoardSize = new QInputDialog(this);
    this->pMenuSetting->addAction(this->pActionBoardSize);
    this->pMenuGame->addAction(this->pActionStart);
    this->pMenuGame->addAction(this->pActionPause);
    this->pMenuGame->addAction(this->pActionContinue);
    this->pMenuGame->addAction(this->pActionEnd);
    this->pMenuGame->addAction(this->pActionClear);
    this->pMenuGame->addAction(this->pActionTakeBack);
    this->pMenuPlayer->addAction(this->pActionPlayerSetting);
    this->pMenuAbout->addAction(this->pActionVer);
    this->pMenuBar->addMenu(this->pMenuGame);
    this->pMenuBar->addMenu(this->pMenuSetting);
    this->pMenuBar->addMenu(this->pMenuPlayer);
    this->pMenuBar->addMenu(this->pMenuAbout);

    this->pMenuSetting->addSeparator();
    this->pRuleActionGroup = new QActionGroup(this);
    this->pRuleActionGroup->setExclusive(true);
    this->pActionFreeStyleGomoku->setCheckable(true);
    this->pActionStandardGomoku->setCheckable(true);
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionFreeStyleGomoku));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionStandardGomoku));
    this->pActionFreeStyleGomoku->setChecked(true);

#ifndef USE_DEFAULT_MENU_BAR
    setMenuBar(this->pMenuBar);
#endif

    this->mBoard = new Board();
    // pair<int, int> pBSize(15, 15);
    // this->mBoard->setBSize(pBSize);

    resize(this->mBoard->getBSize().first * RECT_WIDTH, (this->mBoard->getBSize().second + 1) * RECT_HEIGHT + this->pMenuBar->height());

    this->m_bPause = true;

    this->m_freeStyleGomoku = new FreeStyleGomoku();
    this->m_standardGomoku = new StandardGomoku();

    this->m_p1 = new Player();
    this->m_p2 = new Player();

    this->m_manager = new Manager(this->mBoard);

    connect(this->pActionStart, SIGNAL(triggered()), this, SLOT(OnActionStart()));
    connect(this->pActionPause, SIGNAL(triggered()), this, SLOT(OnActionPause()));
    connect(this->pActionContinue, SIGNAL(triggered()), this, SLOT(OnActionContinue()));
    connect(this->pActionEnd, SIGNAL(triggered()), this, SLOT(OnActionEnd()));
    connect(this->pActionClear, SIGNAL(triggered()), this, SLOT(OnActionClearBoard()));
    connect(this->pActionTakeBack, SIGNAL(triggered()), this, SLOT(OnActionTakeBack()));
    connect(this->pActionBoardSize, SIGNAL(triggered()), this, SLOT(OnActionBoardSize()));
    connect(this->pActionPlayerSetting, SIGNAL(triggered()), this, SLOT(OnActionPlayerSetting()));
    connect(this->pActionVer, SIGNAL(triggered()), this, SLOT(OnActionVer()));
}

MainWindow::~MainWindow()
{
    if (nullptr != this->m_manager)
    {
        delete this->m_manager;
        this->m_manager = nullptr;
    }
    if (nullptr != this->m_p1)
    {
        delete this->m_p1;
        this->m_p1 = nullptr;
    }
    if (nullptr != this->m_p2)
    {
        delete this->m_p2;
        this->m_p2 = nullptr;
    }
    if (nullptr != this->m_freeStyleGomoku)
    {
        delete  this->m_freeStyleGomoku;
        this->m_freeStyleGomoku = nullptr;
    }
    if (nullptr != this->m_standardGomoku)
    {
        delete this->m_standardGomoku;
        this->m_standardGomoku = nullptr;
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
    if (nullptr != this->pActionPause)
    {
        delete this->pActionPause;
        this->pActionPause = nullptr;
    }
    if (nullptr != this->pActionContinue)
    {
        delete this->pActionContinue;
        this->pActionContinue = nullptr;
    }
    if (nullptr != this->pActionEnd)
    {
        delete this->pActionEnd;
        this->pActionEnd = nullptr;
    }
    if (nullptr != this->pActionClear)
    {
        delete this->pActionClear;
        this->pActionClear = nullptr;
    }
    if (nullptr != this->pActionTakeBack)
    {
        delete this->pActionTakeBack;
        this->pActionTakeBack = nullptr;
    }
    if (nullptr != this->pActionBoardSize)
    {
        delete this->pActionBoardSize;
        this->pActionBoardSize = nullptr;
    }
    if (nullptr != this->pActionFreeStyleGomoku)
    {
        delete this->pActionFreeStyleGomoku;
        this->pActionFreeStyleGomoku = nullptr;
    }
    if (nullptr != this->pActionStandardGomoku)
    {
        delete this->pActionStandardGomoku;
        this->pActionStandardGomoku = nullptr;
    }
    if (nullptr != this->pActionPlayerSetting)
    {
        delete this->pActionPlayerSetting;
        this->pActionPlayerSetting = nullptr;
    }
    if (nullptr != this->pActionVer)
    {
        delete this->pActionVer;
        this->pActionVer = nullptr;
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
    if (nullptr != this->pDialogBoardSize)
    {
        delete this->pDialogBoardSize;
        this->pDialogBoardSize = nullptr;
    }
    if (nullptr != this->pRuleActionGroup)
    {
        delete this->pRuleActionGroup;
        this->pRuleActionGroup = nullptr;
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    DrawChessboard();
    DrawItems();

    update();
}

void MainWindow::DrawChessboard()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(QColor(Qt::black),2));

    for(int i = 0; i < this->mBoard->getBSize().first - 1; ++i)
    {
        for (int j = 1; j < this->mBoard->getBSize().second; ++j)
            painter.drawRect( (i+0.5)*RECT_WIDTH,(j+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);
    }
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));

    for (size_t i = 0; i < this->mBoard->getVRecord().size(); i++)
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
        p.setY(this->mBoard->coord2idx(this->mBoard->getVRecord().at(i).first).second + 1);
        DrawChessAtPoint(painter, p);
    }
}

void MainWindow::DrawChessAtPoint(QPainter& painter,QPoint& pt)
{
    if (!this->m_bPause)
    {
        //painter.drawRect( (pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);

        QPoint ptCenter((pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT);
        painter.drawEllipse(ptCenter,RECT_WIDTH / 2,RECT_HEIGHT / 2);
    }
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    if (!this->m_bPause)
    {
        QPoint pt;
        pt.setX( (e->pos().x() ) / RECT_WIDTH);
        pt.setY( (e->pos().y() ) / RECT_HEIGHT);

        if (pt.y() < 1)
            return;

        pair<int, int> p_idx(pt.x(), pt.y() - 1);

        if (this->mBoard->GetState() != BOARDFULL)
        {
            if ((this->mBoard->GetState() == BOARDEMPTY) || (this->mBoard->GetState() == BLACKNEXT))
            {
                this->mBoard->placeStone(p_idx, BLACK);
            }
            else if (this->mBoard->GetState() == WHITENEXT)
            {
                this->mBoard->placeStone(p_idx, WHITE);
            }
        }

        //if connect five
        bool isWin = false;
        if (this->pActionFreeStyleGomoku->isChecked())
        {
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
        }
        else if (this->pActionStandardGomoku->isChecked())
        {
            isWin = this->m_standardGomoku->checkWin(this->mBoard);
        }
        else
        {
            /* code */
        }

        if (isWin)
        {
            if (this->mBoard->getVRecord().back().second == BLACK)
                QMessageBox::information(this, "game over!", "Black win!");
            else
                QMessageBox::information(this, "game over!", "White win!");
            this->mBoard->clearBoard();
            return ;
        }
    }
}

void MainWindow::OnActionStart()
{
    this->m_bPause = false;
    this->m_p1->m_sPath = this->m_player_setting->getP1Path();
    this->m_p2->m_sPath = this->m_player_setting->getP2Path();
    this->m_p1->m_isComputer = !(this->m_player_setting->isP1Human());
    this->m_p2->m_isComputer = !(this->m_player_setting->isP2Human());
    qDebug() << this->m_p1->m_sPath;
    qDebug() << this->m_p2->m_sPath;
    this->m_p1->m_isMyTurn = true;
    this->m_p2->m_isMyTurn = false;

    bool bAttach = this->m_manager->AttachEngines(this->m_p1, this->m_p2);
    qDebug() << "AttachFlag: " << bAttach;

    this->mState = GAME_STATE::PLAYING;
}

void MainWindow::OnActionPause()
{
    this->m_bPause = true;
    this->mState = GAME_STATE::PAUSING;
}

void MainWindow::OnActionContinue()
{
    this->m_bPause = false;
    this->mState = GAME_STATE::PLAYING;
}

void MainWindow::OnActionEnd()
{
    if (nullptr != this->m_manager)
    {
        bool bDetach = this->m_manager->DetachEngines();
        qDebug() << "DetachFlag: " << bDetach;
    }

    this->mState = GAME_STATE::IDLE;
}

void MainWindow::OnActionClearBoard()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        this->mBoard->clearBoard();
    }
}

void MainWindow::OnActionTakeBack()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool b_succ = this->mBoard->takeBackStone();
        if (!b_succ)
            QMessageBox::information(this, "Error!", "Failied to take back!");
    }
}

void MainWindow::OnActionBoardSize()
{
    if (this->mState == GAME_STATE::IDLE)
    {
        bool getInfo = false;
        QString down = pDialogBoardSize->getText(this, "Board Size", "Please input board size:", QLineEdit::Normal, "15", &getInfo,
                                                 Qt::WindowFlags(), Qt::ImhNone);
        if (getInfo)
        {
            bool ok = false;
            int iTmp = down.toInt(&ok);
            pair<int, int> pTmp(iTmp, iTmp);
            if (ok)
            {
                if (this->mBoard->setBSize(pTmp))
                    resize(this->mBoard->getBSize().first * RECT_WIDTH, (this->mBoard->getBSize().second + 1) * RECT_HEIGHT + this->pMenuBar->height());
            }
        }
    }
}

void MainWindow::OnActionPlayerSetting()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        this->m_player_setting->exec();
    }
}

void MainWindow::OnActionVer()
{
    const QString strVerNum = "Ver Num: 0.0.01";
    const QString strAll = strVerNum + "\n" + "build at " + __TIMESTAMP__;
    QMessageBox::about(this, "Version", strAll);
}
