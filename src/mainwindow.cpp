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

#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>

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
    this->pActionTimeoutMatch = new QAction("Match Timeout", this);
    this->pActionTimeoutTurn = new QAction("Turn Timeout", this);
    this->pActionMaxMemory = new QAction("Max Memory", this);
    this->pActionStart = new QAction("Start", this);
    this->pActionPause = new QAction("Pause", this);
    this->pActionContinue = new QAction("Continue", this);
    this->pActionEnd = new QAction("End", this);
    this->pActionClear = new QAction("Clear", this);
    this->pActionTakeBack = new QAction("Take Back", this);
    this->pActionFreeStyleGomoku = new QAction("Free-style Gomoku", this);
    this->pActionStandardGomoku = new QAction("Standard Gomoku", this);
    this->pActionRenju = new QAction("Renju", this);
    this->pActionPlayerSetting = new QAction("Setting", this);
    this->pActionVer = new QAction("Ver Info", this);
    this->pActionFeedback = new QAction("Feedback", this);
    this->pActionLicense = new QAction("License", this);
    this->pMenuSetting->addAction(this->pActionBoardSize);
    this->pMenuSetting->addAction(this->pActionTimeoutMatch);
    this->pMenuSetting->addAction(this->pActionTimeoutTurn);
    this->pMenuSetting->addAction(this->pActionMaxMemory);
    this->pMenuGame->addAction(this->pActionStart);
    this->pMenuGame->addAction(this->pActionPause);
    this->pMenuGame->addAction(this->pActionContinue);
    this->pMenuGame->addAction(this->pActionEnd);
    this->pMenuGame->addAction(this->pActionClear);
    this->pMenuGame->addAction(this->pActionTakeBack);
    this->pMenuPlayer->addAction(this->pActionPlayerSetting);
    this->pMenuAbout->addAction(this->pActionVer);
    this->pMenuAbout->addAction(this->pActionFeedback);
    this->pMenuAbout->addAction(this->pActionLicense);
    this->pMenuBar->addMenu(this->pMenuGame);
    this->pMenuBar->addMenu(this->pMenuSetting);
    this->pMenuBar->addMenu(this->pMenuPlayer);
    this->pMenuBar->addMenu(this->pMenuAbout);

    this->pMenuSetting->addSeparator();
    this->pRuleActionGroup = new QActionGroup(this);
    this->pRuleActionGroup->setExclusive(true);
    this->pActionFreeStyleGomoku->setCheckable(true);
    this->pActionStandardGomoku->setCheckable(true);
    this->pActionRenju->setCheckable(true);
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionFreeStyleGomoku));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionStandardGomoku));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionRenju));
    this->pActionFreeStyleGomoku->setChecked(true);

#ifndef USE_DEFAULT_MENU_BAR
    setMenuBar(this->pMenuBar);
#endif

    this->setWindowIcon(QIcon(":/icon/icon.jpg"));

    this->mBoard = new Board();
    // pair<int, int> pBSize(15, 15);
    // this->mBoard->setBSize(pBSize);

    this->resize(this->mBoard->getBSize().first * RECT_WIDTH, (this->mBoard->getBSize().second + 1) * RECT_HEIGHT + this->pMenuBar->height());
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);

    this->m_bBoard = false;

    this->m_freeStyleGomoku = new FreeStyleGomoku();
    this->m_standardGomoku = new StandardGomoku();
    this->m_renju = new Renju();

    this->m_manager = new Manager(this->mBoard);

    this->mState = GAME_STATE::IDLE;
    this->mRule = GAME_RULE::FREESTYLEGOMOKU;

    this->m_T1 = new Timer();
    this->m_T2 = new Timer();
    this->m_timeout_match = 15*60*1000;
    this->m_timeout_turn = 30*1000;
    this->m_max_memory = 1024*1024*1024;
    this->m_time_left_p1 = 15*60*1000;
    this->m_time_left_p2 = 15*60*1000;

    connect(this->pActionStart, SIGNAL(triggered()), this, SLOT(OnActionStart()));
    connect(this->pActionPause, SIGNAL(triggered()), this, SLOT(OnActionPause()));
    connect(this->pActionContinue, SIGNAL(triggered()), this, SLOT(OnActionContinue()));
    connect(this->pActionEnd, SIGNAL(triggered()), this, SLOT(OnActionEnd()));
    connect(this->pActionClear, SIGNAL(triggered()), this, SLOT(OnActionClearBoard()));
    connect(this->pActionTakeBack, SIGNAL(triggered()), this, SLOT(OnActionTakeBack()));
    connect(this->pActionBoardSize, SIGNAL(triggered()), this, SLOT(OnActionBoardSize()));
    connect(this->pActionTimeoutMatch, SIGNAL(triggered()), this, SLOT(OnActionTimeoutMatch()));
    connect(this->pActionTimeoutTurn, SIGNAL(triggered()), this, SLOT(OnActionTimeoutTurn()));
    connect(this->pActionMaxMemory, SIGNAL(triggered()), this, SLOT(OnActionMaxMemory()));
    connect(this->pRuleActionGroup,SIGNAL(triggered(QAction *)),this,SLOT(On_ClickedRuleActionGroup(QAction*)));
    connect(this->pActionPlayerSetting, SIGNAL(triggered()), this, SLOT(OnActionPlayerSetting()));
    connect(this->pActionVer, SIGNAL(triggered()), this, SLOT(OnActionVer()));
    connect(this->pActionFeedback, SIGNAL(triggered()), this, SLOT(OnActionFeedback()));
    connect(this->pActionLicense, SIGNAL(triggered()), this, SLOT(OnActionLicense()));
}

MainWindow::~MainWindow()
{
    if (nullptr != this->m_T1)
    {
        delete this->m_T1;
        this->m_T1 = nullptr;
    }
    if (nullptr != this->m_T2)
    {
        delete this->m_T2;
        this->m_T2 = nullptr;
    }
    if (nullptr != this->m_manager)
    {
        delete this->m_manager;
        this->m_manager = nullptr;
    }
    if (nullptr != this->m_renju)
    {
        delete this->m_renju;
        this->m_renju = nullptr;
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
    if (nullptr != this->pActionTimeoutMatch)
    {
        delete this->pActionTimeoutMatch;
        this->pActionTimeoutMatch = nullptr;
    }
    if (nullptr != this->pActionTimeoutTurn)
    {
        delete this->pActionTimeoutTurn;
        this->pActionTimeoutTurn = nullptr;
    }
    if (nullptr != this->pActionMaxMemory)
    {
        delete this->pActionMaxMemory;
        this->pActionMaxMemory = nullptr;
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
    if (nullptr != this->pActionRenju)
    {
        delete this->pActionRenju;
        this->pActionRenju = nullptr;
    }
    if (nullptr != this->pActionPlayerSetting)
    {
        delete this->pActionPlayerSetting;
        this->pActionPlayerSetting = nullptr;
    }
    if (nullptr != this->pActionLicense)
    {
        delete this->pActionLicense;
        this->pActionLicense = nullptr;
    }
    if (nullptr != this->pActionFeedback)
    {
        delete this->pActionFeedback;
        this->pActionFeedback = nullptr;
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
    if (nullptr != this->pRuleActionGroup)
    {
        delete this->pRuleActionGroup;
        this->pRuleActionGroup = nullptr;
    }
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    DrawTimeLeft();
    DrawChessboard();
    DrawItems();
    DrawMark();

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

void MainWindow::DrawChessAtPoint(QPainter &painter, QPoint &pt)
{
    // painter.drawRect( (pt.x()+0.5)*RECT_WIDTH,(pt.y()+0.5)*RECT_HEIGHT,RECT_WIDTH,RECT_HEIGHT);

    QPoint ptCenter((pt.x() + 0.5) * RECT_WIDTH, (pt.y() + 0.5) * RECT_HEIGHT);
    painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
}

void MainWindow::DrawMark()
{
    if (this->mBoard->getVRecord().size() > 0)
    {
        QPainter painter(this);
        QPoint pMark;
        painter.setBrush(Qt::red);
        pair<int, int> last_move_pos = this->mBoard->coord2idx(this->mBoard->getVRecord().back().first);
        pMark.setX(last_move_pos.first);
        pMark.setY(last_move_pos.second + 1);

        QPoint ptCenter((pMark.x() + 0.5) * RECT_WIDTH, (pMark.y() + 0.5) * RECT_HEIGHT);
        painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));

    for (size_t i = 0; i < this->mBoard->getVRecord().size(); i++)
    {
        QPoint p;
        if (this->mBoard->getVRecord().at(i).second == STONECOLOR::BLACK)
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

void MainWindow::DrawTimeLeft()
{
    QFont font;
    font.setPixelSize(15);
    font.setUnderline(true);
    font.setItalic(true);
    font.setBold(true);

    QPainter painter(this);
    painter.setFont(font);
    painter.setPen(QPen(QColor(Qt::black), 2));

    if (0 == this->m_time_left_p1)
    {
    }
    else if (this->m_timeout_match > this->m_T1->getElapsed())
    {
        this->m_time_left_p1 = this->m_timeout_match - this->m_T1->getElapsed();
    }
    else
    {
        this->m_time_left_p1 = 0;
        this->OnActionEnd();
    }

    if (0 == this->m_time_left_p2)
    {
    }
    else if (this->m_timeout_match > this->m_T2->getElapsed())
    {
        this->m_time_left_p2 = this->m_timeout_match - this->m_T2->getElapsed();
    }
    else
    {
        this->m_time_left_p2 = 0;
        this->OnActionEnd();
    }

    if (0 != this->m_time_left_p1)
        painter.drawText(50, 25, 200, 50, Qt::AlignLeft, QString::fromStdString(to_string(this->m_time_left_p1) + "ms"));
    else
        painter.drawText(50, 25, 200, 50, Qt::AlignLeft, "TIMEOUT");

    if (0 != this->m_time_left_p2)
        painter.drawText(this->geometry().width() - 250, 25, 200, 50, Qt::AlignRight, QString::fromStdString(to_string(this->m_time_left_p2) + "ms"));
    else
        painter.drawText(this->geometry().width() - 250, 25, 200, 50, Qt::AlignRight, "TIMEOUT");
}

vector<pair<pair<int,int>, int>> MainWindow::record_expend(vector<pair<int, int>> vRecord)
{
    vector<pair<pair<int,int>, int>> vRecExpendTmp;
    vRecExpendTmp.clear();
    vector<pair<int, int>> vRecZoom = vRecord;
    vector<pair<int, int>>::iterator iter;
    for (iter = vRecZoom.begin(); iter != vRecZoom.end(); ++iter)
    {
        pair<int, int> pTmpPos;
        pair<pair<int, int>, int> vElement;

        pTmpPos = this->mBoard->coord2idx(iter->first);
        vElement.first = pTmpPos;
        vElement.second = iter->second;
        vRecExpendTmp.push_back(vElement);
    }

    return vRecExpendTmp;
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    if (this->mState == GAME_STATE::PLAYING)
    {
        if ((this->m_manager->m_p1->m_isMyTurn && this->m_manager->m_p1->m_isComputer) || (this->m_manager->m_p2->m_isMyTurn && this->m_manager->m_p2->m_isComputer))
            return;

        QPoint pt;
        pt.setX( (e->pos().x() ) / RECT_WIDTH);
        pt.setY( (e->pos().y() ) / RECT_HEIGHT);

        if (pt.y() < 1)
            return;

        pair<int, int> p_idx(pt.x(), pt.y() - 1);

        if (this->mBoard->GetState() != BOARDSTATUS::BOARDFULL)
        {
            bool bSucceed = false;
            if ((this->mBoard->GetState() == BOARDSTATUS::BOARDEMPTY) || (this->mBoard->GetState() == BOARDSTATUS::BLACKNEXT))
            {
                bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::BLACK);
            }
            else if (this->mBoard->GetState() == BOARDSTATUS::WHITENEXT)
            {
                bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::WHITE);
            }
            if (bSucceed)
            {
                this->mBoard->Notify();

                if (this->m_manager->m_p1->m_isMyTurn)
                {
                    this->m_T2->pause();
                    this->m_T1->resume();
                    if (this->m_time_left_p1 > 0)
                    {
                        this->m_manager->infoMatch_p1(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p1).c_str());
                    }
                    else
                    {
                        this->OnActionEnd();
                        QMessageBox::information(this, "Game Over", "Player 1 timeout!");
                        return;
                    }
                    if (this->m_bBoard)
                    {
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expend(this->mBoard->getVRecord());
                        this->m_manager->sendBoard(vRecExpendTmp);
                        this->m_bBoard = false;
                    }
                    else
                    {
                        this->m_manager->turn_2_p1(p_idx.first, p_idx.second);
                    }
                }
                else if (this->m_manager->m_p2->m_isMyTurn)
                {
                    this->m_T1->pause();
                    this->m_T2->resume();
                    if (this->m_time_left_p2 > 0)
                    {
                        this->m_manager->infoMatch_p2(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p2).c_str());
                    }
                    else
                    {
                        this->OnActionEnd();
                        QMessageBox::information(this, "Game Over", "Player 2 timeout!");
                        return;
                    }
                    if (this->m_bBoard)
                    {
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expend(this->mBoard->getVRecord());
                        this->m_manager->sendBoard(vRecExpendTmp);
                        this->m_bBoard = false;
                    }
                    else
                    {
                        this->m_manager->turn_2_p2(p_idx.first, p_idx.second);
                    }
                }
            }
            else
                return;
        }

        //if connect five
        bool isWin = false;
        switch (this->mRule)
        {
        case GAME_RULE::FREESTYLEGOMOKU:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::STANDARDGOMOKU:
            isWin = this->m_standardGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::RENJU:
            isWin = this->m_renju->checkWin(this->mBoard);
            break;
        default:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        }

        if (isWin)
        {
            this->OnActionEnd();
            this->mState = GAME_STATE::OVER;
            this->pRuleActionGroup->setEnabled(true);
            if (this->mBoard->getVRecord().back().second == STONECOLOR::BLACK)
                QMessageBox::information(this, "game over!", "Black win!");
            else
                QMessageBox::information(this, "game over!", "White win!");
            //this->mBoard->clearBoard();
        }
        else if (this->mRule == GAME_RULE::RENJU)
        {
            if (!this->m_renju->isIllegal(this->mBoard))
            {
                this->OnActionEnd();
                this->mState = GAME_STATE::OVER;
                this->pRuleActionGroup->setEnabled(true);
                QString info = "Illegal move from BLACK! ";
                switch (this->m_renju->getRenjuState())
                {
                case PATTERN::OVERLINE:
                    info.append("OVERLINE");
                    break;
                case PATTERN::DOUBLE_FOUR:
                    info.append("DOUBLE_FOUR");
                    break;
                case PATTERN::DOUBLE_THREE:
                    info.append("DOUBLE_THREE");
                    break;
                default:
                    break;
                }
                QMessageBox::information(this, "game over!", info);
            }
        }
    }
}

void MainWindow::OnActionStart()
{
    if (GAME_STATE::PLAYING != this->mState)
    {
        this->mBoard->clearBoard();

        if (nullptr != this->m_T1)
            this->m_T1->stop();
        if (nullptr != this->m_T2)
            this->m_T2->stop();

        this->m_time_left_p1 = this->m_timeout_match;
        this->m_time_left_p2 = this->m_timeout_match;

        this->m_manager->m_p1->m_color = STONECOLOR::BLACK;
        this->m_manager->m_p2->m_color = STONECOLOR::WHITE;
        this->m_manager->m_p1->m_sPath = this->m_player_setting->getP1Path();
        this->m_manager->m_p2->m_sPath = this->m_player_setting->getP2Path();
        this->m_manager->m_p1->m_isComputer = !(this->m_player_setting->isP1Human());
        this->m_manager->m_p2->m_isComputer = !(this->m_player_setting->isP2Human());
        qDebug() << this->m_manager->m_p1->m_sPath;
        qDebug() << this->m_manager->m_p2->m_sPath;
        this->m_manager->m_p1->m_isMyTurn = true;
        this->m_manager->m_p2->m_isMyTurn = false;

        bool bAttach = false;
        bAttach = this->m_manager->AttachEngines();
        qDebug() << "AttachFlag: " << bAttach;

        bool bStart = false;
        if (bAttach)
        {
            if (nullptr != this->m_manager->m_engine_1)
            {
                connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                connect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                connect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                connect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
            }

            bStart = this->m_manager->startMatch(this->mBoard->getBSize().first);
            qDebug() << "StartFlag: " << bStart;
        }
        else
        {
            this->m_manager->DetachEngines();
            QMessageBox::information(this, "Error!", "Failied to Attach Engine!");
            return;
        }

        if (bStart)
        {
            int i_rule_flag = 0;
            if (this->mRule == GAME_RULE::STANDARDGOMOKU)
            {
                i_rule_flag |= 0x01;
            }
            else if (this->mRule == GAME_RULE::RENJU)
            {
                i_rule_flag |= 0x04;
            }
            this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
            this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
            this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
            this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
            this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(i_rule_flag).c_str());

            this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
            this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
            this->m_manager->infoMatch_p2(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
            this->m_manager->infoMatch_p2(INFO_KEY::GAME_TYPE, "0");
            this->m_manager->infoMatch_p2(INFO_KEY::RULE, to_string(i_rule_flag).c_str());
        }
        else
        {
            if (nullptr != this->m_manager->m_engine_1)
            {
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
            }
            this->m_manager->DetachEngines();
            QMessageBox::information(this, "Error!", "Failied to start game!");
            return;
        }

        this->m_manager->infoMatch_p1(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p1).c_str());
        this->m_manager->infoMatch_p2(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p2).c_str());

        if (this->m_manager->m_p1->m_isMyTurn)
            this->m_T1->start();
        else
            this->m_T2->start();

        this->mBoard->Notify();
        this->m_manager->beginMatch();

        this->mState = GAME_STATE::PLAYING;
        this->pRuleActionGroup->setDisabled(true);
    }
}

void MainWindow::OnActionPause()
{
    if (GAME_STATE::PLAYING == this->mState)
    {
        if (nullptr != this->m_manager)
        {
            this->m_manager->endMatch();
            if (nullptr != this->m_manager->m_engine_1)
            {
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
            }
            bool bDetach = this->m_manager->DetachEngines();
            qDebug() << "DetachFlag: " << bDetach;
        }

        if (nullptr != this->m_T1)
        {
            this->m_T1->pause();
        }
        if (nullptr != this->m_T2)
        {
            this->m_T2->pause();
        }

        this->mState = GAME_STATE::PAUSING;
    }
}

void MainWindow::OnActionContinue()
{
    if (this->mState == GAME_STATE::PAUSING || this->mState == IDLE)
    {
        if (nullptr != this->m_manager)
        {
            this->m_manager->m_p1->m_color = STONECOLOR::BLACK;
            this->m_manager->m_p2->m_color = STONECOLOR::WHITE;
            this->m_manager->m_p1->m_sPath = this->m_player_setting->getP1Path();
            this->m_manager->m_p2->m_sPath = this->m_player_setting->getP2Path();
            this->m_manager->m_p1->m_isComputer = !(this->m_player_setting->isP1Human());
            this->m_manager->m_p2->m_isComputer = !(this->m_player_setting->isP2Human());
            qDebug() << this->m_manager->m_p1->m_sPath;
            qDebug() << this->m_manager->m_p2->m_sPath;

            bool bAttach = false;
            bAttach = this->m_manager->AttachEngines();
            qDebug() << "AttachFlag: " << bAttach;

            bool bStart = false;
            if (bAttach)
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                }
                if (nullptr != this->m_manager->m_engine_2)
                {
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
                }

                bStart = this->m_manager->startMatch(this->mBoard->getBSize().first);
                qDebug() << "StartFlag: " << bStart;
            }
            else
            {
                this->m_manager->DetachEngines();
                QMessageBox::information(this, "Error!", "Failied to Attach Engine!");
                return;
            }

            if (bStart)
            {
                int i_rule_flag = 0;
                if (this->mRule == GAME_RULE::STANDARDGOMOKU)
                {
                    i_rule_flag |= 0x01;
                }
                else if (this->mRule == GAME_RULE::RENJU)
                {
                    i_rule_flag |= 0x04;
                }
                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
                this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(i_rule_flag).c_str());

                this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::GAME_TYPE, "0");
                this->m_manager->infoMatch_p2(INFO_KEY::RULE, to_string(i_rule_flag).c_str());
            }
            else
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                }
                if (nullptr != this->m_manager->m_engine_2)
                {
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
                }
                this->m_manager->DetachEngines();
                QMessageBox::information(this, "Error!", "Failied to start game!");
                return;
            }

            vector<pair<pair<int,int>, int>> vRecExpendTmp;
            vector<pair<int, int>> vRecZoom = this->mBoard->getVRecord();
            vector<pair<int, int>>::iterator iter;
            for (iter = vRecZoom.begin(); iter != vRecZoom.end(); ++iter)
            {
                pair<int, int> pTmpPos;
                pair<pair<int, int>, int> vElement;

                pTmpPos = this->mBoard->coord2idx(iter->first);
                vElement.first = pTmpPos;
                vElement.second = iter->second;
                vRecExpendTmp.push_back(vElement);
            }

            this->mBoard->Notify();

            if (this->m_manager->m_p1->m_isMyTurn)
            {
                this->m_T2->pause();
                this->m_T1->resume();
                if (this->m_time_left_p1 > 0)
                {
                    this->m_manager->infoMatch_p1(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p1).c_str());
                }
                else
                {
                    this->OnActionEnd();
                    QMessageBox::information(this, "Game Over", "Player 1 timeout!");
                    return;
                }
            }
            else
            {
                this->m_T1->pause();
                this->m_T2->resume();
                if (this->m_time_left_p2 > 0)
                {
                    this->m_manager->infoMatch_p2(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p2).c_str());
                }
                else
                {
                    this->OnActionEnd();
                    QMessageBox::information(this, "Game Over", "Player 2 timeout!");
                    return;
                }
            }
            this->m_manager->sendBoard(vRecExpendTmp);
        }

        this->m_bBoard = true;
        this->mState = GAME_STATE::PLAYING;
        this->pRuleActionGroup->setDisabled(true);
    }
}

void MainWindow::OnActionEnd()
{
    if (GAME_STATE::PLAYING == this->mState)
    {
        if (nullptr != this->m_manager)
        {
            this->m_manager->endMatch();
            if (nullptr != this->m_manager->m_engine_1)
            {
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
            }
            bool bDetach = this->m_manager->DetachEngines();
            qDebug() << "DetachFlag: " << bDetach;
        }

        if (nullptr != this->m_T1)
        {
            this->m_T1->pause();
        }
        if (nullptr != this->m_T2)
        {
            this->m_T2->pause();
        }

        this->mState = GAME_STATE::PAUSING;
    }
}

void MainWindow::OnActionClearBoard()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        this->mBoard->clearBoard();
        this->mBoard->Notify();

        if (nullptr != this->m_T1)
            this->m_T1->stop();
        if (nullptr != this->m_T2)
            this->m_T2->stop();

        this->mState = GAME_STATE::IDLE;
        this->pRuleActionGroup->setEnabled(true);
    }
}

void MainWindow::OnActionTakeBack()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool b_succ = this->mBoard->takeBackStone();
        if (!b_succ)
        {
            this->OnActionEnd();
            QMessageBox::information(this, "Error!", "Failied to take back!");
            return;
        }
        else
        {
            this->mBoard->Notify();
            if (this->mState == GAME_STATE::OVER)
                this->mState = GAME_STATE::PAUSING;
        }
    }
}

void MainWindow::OnActionBoardSize()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool ok = false;
        int i_get = QInputDialog::getInt(this, "Board Size", "Please input board size:", 15, 8, 25,
                                            1, &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            int iTmp = i_get;
            pair<int, int> pTmp(iTmp, iTmp);
            if (this->mBoard->setBSize(pTmp))
                resize(this->mBoard->getBSize().first * RECT_WIDTH, (this->mBoard->getBSize().second + 1) * RECT_HEIGHT + this->pMenuBar->height());

            this->mBoard->Notify();

            if (nullptr != this->m_T1)
                this->m_T1->stop();
            if (nullptr != this->m_T2)
                this->m_T2->stop();

            this->mState = GAME_STATE::IDLE;
            this->pRuleActionGroup->setEnabled(true);
        }
    }
}

void MainWindow::OnActionTimeoutMatch()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool ok = false;
        int i_get = QInputDialog::getInt(this, "Match Timeout", "Please input Match-Timeout(ms):", 900000, 0, 86400000,
                                            1000, &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            if (i_get > 0)
            {
                this->m_timeout_match = (unsigned int)i_get;
                this->m_time_left_p1 = (unsigned int)i_get;
                this->m_time_left_p2 = (unsigned int)i_get;
            }
            else if (i_get == 0)
            {
                this->m_timeout_match = 2147483647;
                this->m_time_left_p1 = 2147483647;
                this->m_time_left_p2 = 2147483647;
            }

            if (nullptr != this->m_T1)
                this->m_T1->stop();
            if (nullptr != this->m_T2)
                this->m_T2->stop();
        }
    }
}

void MainWindow::OnActionTimeoutTurn()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool ok = false;
        int i_get = QInputDialog::getInt(this, "Turn Timeout", "Please input Turn-Timeout(ms):", 30000, 5000, 300000,
                                            1000, &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            if (i_get >= 0) this->m_timeout_turn = (unsigned int)i_get;
        }
    }
}

void MainWindow::OnActionMaxMemory()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool ok = false;
        int i_get = QInputDialog::getInt(this, "Max Memory", "Please input Max-Memory(byte):", 1024*1024*1024, 0, (int)((unsigned int)-1>>1),
                                            1024, &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            if (i_get >= 0) this->m_max_memory = (unsigned int)i_get;
        }
    }
}

void MainWindow::On_ClickedRuleActionGroup(QAction *pAction)
{
    if (this->mState == GAME_STATE::OVER || this->mState == GAME_STATE::IDLE)
    {
        if (pAction->text().compare(this->pActionFreeStyleGomoku->text())==0)
        {
            qDebug() << "Choose Free-Style gomoku!";
            this->mRule = GAME_RULE::FREESTYLEGOMOKU;
        }
        else if (pAction->text().compare(this->pActionStandardGomoku->text())==0)
        {
            qDebug() << "Choose standard gomoku!";
            this->mRule = GAME_RULE::STANDARDGOMOKU;
        }
        else if (pAction->text().compare(this->pActionRenju->text())==0)
        {
            qDebug() << "Choose renju!";
            this->mRule = GAME_RULE::RENJU;
        }
        else
        {
            qDebug() << "Choose Free-Style gomoku!";
            this->mRule = GAME_RULE::FREESTYLEGOMOKU;
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
    const QString strVerNum = "Ver Num: 0.2.01\n";
    QString strBuildTime = "Build at ";
    strBuildTime.append(__TIMESTAMP__);
    strBuildTime.append("\n");
    const QString strAll = strVerNum + strBuildTime;
    QMessageBox::about(this, "Version", strAll);
}

void MainWindow::OnActionFeedback()
{
    const QString strAll = R"(
        <a href='https://github.com/Joker2770/qpiskvork.git'>https://github.com/Joker2770/qpiskvork.git</a><br>
        <a href='https://gitee.com/Joker2770/qpiskvork.git'>https://gitee.com/Joker2770/qpiskvork.git</a>
        )";
    QMessageBox::about(this, "Feedback", strAll);
}

void MainWindow::OnActionLicense()
{
    QString strAll = nullptr;
    strAll.append("qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.\n");
    strAll.append("Copyright (C) 2022-2023  Jintao Yang <yjt950840@outlook.com>\n\n");
    strAll.append("This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.\n\n");
    strAll.append("This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\n");
    strAll.append("You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.\n");
    QMessageBox::about(this, "License", strAll);
}

void MainWindow::OnP1PlaceStone(int x, int y)
{
    if (this->mState == GAME_STATE::PLAYING)
    {
        if (!this->m_manager->m_p1->m_isMyTurn || !this->m_manager->m_p1->m_isComputer)
            return;

        pair<int, int> p_idx(x, y);

        if (this->mBoard->GetState() != BOARDSTATUS::BOARDFULL)
        {
            bool bSucceed = false;
            if ((this->mBoard->GetState() == BOARDSTATUS::BOARDEMPTY) || (this->mBoard->GetState() == BOARDSTATUS::BLACKNEXT))
            {
                if (this->m_manager->m_p1->m_color == STONECOLOR::BLACK)
                {
                    bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::BLACK);
                }
            }
            else if (this->mBoard->GetState() == BOARDSTATUS::WHITENEXT)
            {
                if (this->m_manager->m_p1->m_color == STONECOLOR::WHITE)
                {
                    bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::WHITE);
                }
            }
            if (bSucceed)
            {
                this->mBoard->Notify();

                if (this->m_manager->m_p2->m_isMyTurn)
                {
                    this->m_T1->pause();
                    this->m_T2->resume();
                    if (this->m_time_left_p2 > 0)
                    {
                        this->m_manager->infoMatch_p2(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p2).c_str());
                    }
                    else
                    {
                        this->OnActionEnd();
                        QMessageBox::information(this, "Game Over", "Player 2 timeout!");
                        return;
                    }

                    if (this->m_bBoard)
                    {
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expend(this->mBoard->getVRecord());
                        this->m_manager->sendBoard(vRecExpendTmp);
                        this->m_bBoard = false;
                    }
                    else
                    {
                        this->m_manager->turn_2_p2(p_idx.first, p_idx.second);
                    }
                }
            }
            else
            {
                this->OnActionEnd();
                QMessageBox::information(this, "Game Error", "Might be illegal move from player 1!");
                return;
            }
        }

        //if connect five
        bool isWin = false;
        switch (this->mRule)
        {
        case GAME_RULE::FREESTYLEGOMOKU:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::STANDARDGOMOKU:
            isWin = this->m_standardGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::RENJU:
            isWin = this->m_renju->checkWin(this->mBoard);
            break;
        default:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        }

        if (isWin)
        {
            this->OnActionEnd();
            this->mState = GAME_STATE::OVER;
            this->pRuleActionGroup->setEnabled(true);
            if (this->mBoard->getVRecord().back().second == STONECOLOR::BLACK)
                QMessageBox::information(this, "game over!", "Black win!");
            else
                QMessageBox::information(this, "game over!", "White win!");
            //this->mBoard->clearBoard();
        }
        else if (this->mRule == GAME_RULE::RENJU)
        {
            if (!this->m_renju->isIllegal(this->mBoard))
            {
                this->OnActionEnd();
                this->mState = GAME_STATE::OVER;
                this->pRuleActionGroup->setEnabled(true);
                QString info = "Illegal move from BLACK! ";
                switch (this->m_renju->getRenjuState())
                {
                case PATTERN::OVERLINE:
                    info.append("OVERLINE");
                    break;
                case PATTERN::DOUBLE_FOUR:
                    info.append("DOUBLE_FOUR");
                    break;
                case PATTERN::DOUBLE_THREE:
                    info.append("DOUBLE_THREE");
                    break;
                default:
                    break;
                }
                QMessageBox::information(this, "game over!", info);
            }
        }
    }
}

void MainWindow::OnP2PlaceStone(int x, int y)
{
    if (this->mState == GAME_STATE::PLAYING)
    {
        if (!this->m_manager->m_p2->m_isMyTurn || !this->m_manager->m_p2->m_isComputer)
            return;

        pair<int, int> p_idx(x, y);

        if (this->mBoard->GetState() != BOARDSTATUS::BOARDFULL)
        {
            bool bSucceed = false;
            if ((this->mBoard->GetState() == BOARDSTATUS::BOARDEMPTY) || (this->mBoard->GetState() == BOARDSTATUS::BLACKNEXT))
            {
                if (this->m_manager->m_p2->m_color == STONECOLOR::BLACK)
                {
                    bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::BLACK);
                }
            }
            else if (this->mBoard->GetState() == BOARDSTATUS::WHITENEXT)
            {
                if (this->m_manager->m_p2->m_color == STONECOLOR::WHITE)
                {
                    bSucceed = this->mBoard->placeStone(p_idx, STONECOLOR::WHITE);
                }
            }
            if (bSucceed)
            {
                this->mBoard->Notify();

                if (this->m_manager->m_p1->m_isMyTurn)
                {
                    this->m_T2->pause();
                    this->m_T1->resume();
                    if (this->m_time_left_p1 > 0)
                    {
                        this->m_manager->infoMatch_p1(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p1).c_str());
                    }
                    else
                    {
                        this->OnActionEnd();
                        QMessageBox::information(this, "Game Over", "Player 1 timeout!");
                        return;
                    }

                    if (this->m_bBoard)
                    {
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expend(this->mBoard->getVRecord());
                        this->m_manager->sendBoard(vRecExpendTmp);
                        this->m_bBoard = false;
                    }
                    else
                    {
                        this->m_manager->turn_2_p1(p_idx.first, p_idx.second);
                    }
                }
            }
            else
            {
                this->OnActionEnd();
                QMessageBox::information(this, "Game Error", "Might be illegal move from player 2!");
                return;
            }
        }

        //if connect five
        bool isWin = false;
        switch (this->mRule)
        {
        case GAME_RULE::FREESTYLEGOMOKU:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::STANDARDGOMOKU:
            isWin = this->m_standardGomoku->checkWin(this->mBoard);
            break;
        case GAME_RULE::RENJU:
            isWin = this->m_renju->checkWin(this->mBoard);
            break;
        default:
            isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
            break;
        }

        if (isWin)
        {
            this->OnActionEnd();
            this->mState = GAME_STATE::OVER;
            this->pRuleActionGroup->setEnabled(true);
            if (this->mBoard->getVRecord().back().second == STONECOLOR::BLACK)
                QMessageBox::information(this, "game over!", "Black win!");
            else
                QMessageBox::information(this, "game over!", "White win!");
            //this->mBoard->clearBoard();
        }
        else if (this->mRule == GAME_RULE::RENJU)
        {
            if (!this->m_renju->isIllegal(this->mBoard))
            {
                this->OnActionEnd();
                this->mState = GAME_STATE::OVER;
                this->pRuleActionGroup->setEnabled(true);
                QString info = "Illegal move from BLACK! ";
                switch (this->m_renju->getRenjuState())
                {
                case PATTERN::OVERLINE:
                    info.append("OVERLINE");
                    break;
                case PATTERN::DOUBLE_FOUR:
                    info.append("DOUBLE_FOUR");
                    break;
                case PATTERN::DOUBLE_THREE:
                    info.append("DOUBLE_THREE");
                    break;
                default:
                    break;
                }
                QMessageBox::information(this, "game over!", info);
            }
        }
    }
}

void MainWindow::OnP1ResponseError()
{
    this->OnActionEnd();
    QMessageBox::information(this, "game over!", "Player 1 responsed ERROR!");
}

void MainWindow::OnP1ResponseUnknown()
{
    this->OnActionEnd();
    QMessageBox::information(this, "game over!", "Player 1 responsed UNKNOWN!");
}

void MainWindow::OnP2ResponseError()
{
    this->OnActionEnd();
    QMessageBox::information(this, "game over!", "Player 2 responsed ERROR!");
}

void MainWindow::OnP2ResponseUnknown()
{
    this->OnActionEnd();
    QMessageBox::information(this, "game over!", "Player 2 responsed UNKNOWN!");
}

