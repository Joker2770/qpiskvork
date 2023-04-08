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
    this->pActionSkin = new QAction("Skin", this);
    this->pActionStart = new QAction("Start", this);
    this->pActionPause = new QAction("Pause", this);
    this->pActionContinue = new QAction("Continue", this);
    this->pActionEnd = new QAction("End", this);
    this->pActionClear = new QAction("Clear", this);
    this->pActionTakeBack = new QAction("Take Back", this);
    this->pActionFreeStyleGomoku = new QAction("Free-style Gomoku", this);
    this->pActionStandardGomoku = new QAction("Standard Gomoku", this);
    this->pActionContinuous = new QAction("Continuous", this);
    this->pActionRenju = new QAction("Renju", this);
    this->pActionCaro = new QAction("Caro", this);
    this->pActionPlayerSetting = new QAction("Setting", this);
    this->pActionVer = new QAction("Ver Info", this);
    this->pActionFeedback = new QAction("Feedback", this);
    this->pActionLicense = new QAction("License", this);
    this->pActionStart->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    this->pActionPause->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    this->pActionContinue->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    this->pActionEnd->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    this->pActionClear->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    this->pActionTakeBack->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    this->pActionBoardSize->setShortcut(QKeySequence(Qt::Key_B));
    this->pActionTimeoutMatch->setShortcut(QKeySequence(Qt::Key_M));
    this->pActionTimeoutTurn->setShortcut(QKeySequence(Qt::Key_T));
    this->pActionMaxMemory->setShortcut(QKeySequence(Qt::Key_O));
    this->pActionSkin->setShortcut(QKeySequence(Qt::Key_K));
    this->pActionFreeStyleGomoku->setShortcut(QKeySequence(Qt::Key_F));
    this->pActionStandardGomoku->setShortcut(QKeySequence(Qt::Key_S));
    this->pActionContinuous->setShortcut(QKeySequence(Qt::Key_N));
    this->pActionRenju->setShortcut(QKeySequence(Qt::Key_R));
    this->pActionCaro->setShortcut(QKeySequence(Qt::Key_C));
    this->pActionPlayerSetting->setShortcut(QKeySequence(Qt::Key_P));
    this->pMenuSetting->addAction(this->pActionBoardSize);
    this->pMenuSetting->addAction(this->pActionTimeoutMatch);
    this->pMenuSetting->addAction(this->pActionTimeoutTurn);
    this->pMenuSetting->addAction(this->pActionMaxMemory);
    this->pMenuSetting->addAction(this->pActionSkin);
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
    this->pRuleActionGroup->setExclusive(false);
    this->pActionFreeStyleGomoku->setCheckable(true);
    this->pActionStandardGomoku->setCheckable(true);
    this->pActionContinuous->setCheckable(true);
    this->pActionRenju->setCheckable(true);
    this->pActionCaro->setCheckable(true);
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionFreeStyleGomoku));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionStandardGomoku));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionContinuous));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionRenju));
    this->pMenuSetting->addAction(this->pRuleActionGroup->addAction(this->pActionCaro));
    this->pActionFreeStyleGomoku->setChecked(true);

#ifndef USE_DEFAULT_MENU_BAR
    setMenuBar(this->pMenuBar);
#endif

    this->setWindowIcon(QIcon(":/icon/icon.jpg"));

    this->mBoard = new Board();
    // pair<int, int> pBSize(15, 15);
    // this->mBoard->setBSize(pBSize);

    this->m_p1_name.clear();
    this->m_p2_name.clear();

    this->resize((this->mBoard->getBSize().first + 2) * RECT_WIDTH, (this->mBoard->getBSize().second + 3) * RECT_HEIGHT + this->pMenuBar->height());
    this->setWindowFlags(this->windowFlags()&~Qt::WindowMaximizeButtonHint);

    this->m_bBoard = false;
    this->m_bSkin = true;

    QPixmap pm;
    pm.load(":/skins/HGarden2.bmp");
    if (!pm.isNull())
    {
        for (size_t i = 0; i < 5; i++)
            this->m_images.push_back(pm.copy((int)(i * (pm.width()) * 0.2), 0, (int)((pm.width()) * 0.2), pm.height()).scaled(RECT_WIDTH, RECT_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    if (this->m_images.size() != 5)
        this->m_bSkin = false;

    this->m_freeStyleGomoku = new FreeStyleGomoku();
    this->m_standardGomoku = new StandardGomoku();
    this->m_renju = new Renju();
    this->m_caro = new Caro();

    this->m_manager = new Manager(this->mBoard);

    this->mState = GAME_STATE::IDLE;
    this->m_Rule = GAME_RULE::FREESTYLEGOMOKU;

    this->m_bOK_P1 = false;
    this->m_bOK_P2 = false;
    this->m_T1 = new Timer();
    this->m_T2 = new Timer();
    this->m_timeout_match = 15 * 60 * 1000;
    this->m_timeout_turn = 30 * 1000;
    this->m_max_memory = 1024 * 1024 * 1024;
    this->m_time_left_p1 = this->m_timeout_match;
    this->m_time_left_p2 = this->m_timeout_match;

    this->m_manager->m_p1->m_color = STONECOLOR::BLACK;
    this->m_manager->m_p2->m_color = STONECOLOR::WHITE;
    this->m_manager->m_p1->m_sPath = this->m_player_setting->getP1Path();
    this->m_manager->m_p2->m_sPath = this->m_player_setting->getP2Path();
    this->m_manager->m_p1->m_isComputer = !(this->m_player_setting->isP1Human());
    this->m_manager->m_p2->m_isComputer = !(this->m_player_setting->isP2Human());
    // qDebug() << this->m_manager->m_p1->m_sPath;
    // qDebug() << this->m_manager->m_p2->m_sPath;
    this->m_manager->m_p1->m_isMyTurn = true;
    this->m_manager->m_p2->m_isMyTurn = false;

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
    connect(this->pActionSkin, SIGNAL(triggered()), this, SLOT(OnActionSkin()));
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
    if (nullptr != this->m_caro)
    {
        delete this->m_caro;
        this->m_caro = nullptr;
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
    if (nullptr != this->pActionSkin)
    {
        delete this->pActionSkin;
        this->pActionSkin = nullptr;
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
    if (nullptr != this->pActionContinuous)
    {
        delete this->pActionContinuous;
        this->pActionContinuous = nullptr;
    }
    if (nullptr != this->pActionRenju)
    {
        delete this->pActionRenju;
        this->pActionRenju = nullptr;
    }
    if (nullptr != this->pActionCaro)
    {
        delete this->pActionCaro;
        this->pActionCaro = nullptr;
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

    DrawChessboard();
    DrawTimeLeft();
    DrawPlayerState();
    DrawPlayerStone();
    DrawPlayerName();
    DrawIndication();
    DrawItems();
    DrawMark();

    update();
}

void MainWindow::DrawChessboard()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(QColor(Qt::black), 2));

    for (int i = 0; i < this->mBoard->getBSize().first; ++i)
    {
        for (int j = 0; j < this->mBoard->getBSize().second; ++j)
        {
            if (this->m_bSkin && !this->m_images.at(0).isNull())
                painter.drawPixmap((i + 1) * RECT_WIDTH, (j + 2) * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT, this->m_images.at(0));
            else
                painter.drawRect((i + 1) * RECT_WIDTH, (j + 2) * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT);
        }
    }
}

void MainWindow::DrawMark()
{
    if (this->mBoard->getVRecord().size() > 0)
    {
        QPainter painter(this);
        QPoint pMark;
        painter.setBrush(Qt::red);
        painter.setRenderHints(QPainter::Antialiasing, true);
        painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

        pair<int, int> last_move_pos = this->mBoard->coord2idx(this->mBoard->getVRecord().back().first);
        pMark.setX(last_move_pos.first + 1);
        pMark.setY(last_move_pos.second + 2);

        QPoint ptCenter((pMark.x() + 0.5) * RECT_WIDTH, (pMark.y() + 0.5) * RECT_HEIGHT);

        int idx = 3;
        if (this->mBoard->getVRecord().back().second == STONECOLOR::BLACK)
            idx = 3;
        else
            idx = 4;


        if (this->m_bSkin && !this->m_images.at(idx).isNull())
            painter.drawPixmap(pMark.x() * RECT_WIDTH, pMark.y() * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT, this->m_images.at(idx));
        else
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }
}

void MainWindow::DrawItems()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

    for (size_t i = 0; i < this->mBoard->getVRecord().size(); i++)
    {
        QPoint p;
        p.setX(this->mBoard->coord2idx(this->mBoard->getVRecord().at(i).first).first + 1);
        p.setY(this->mBoard->coord2idx(this->mBoard->getVRecord().at(i).first).second + 2);
        QPoint ptCenter((p.x() + 0.5) * RECT_WIDTH, (p.y() + 0.5) * RECT_HEIGHT);

        int idx = 1;
        if (this->mBoard->getVRecord().at(i).second == STONECOLOR::BLACK)
        {
            painter.setBrush(Qt::black);
            idx = 1;
        }
        else
        {
            painter.setBrush(Qt::white);
            idx = 2;
        }

        if (this->m_bSkin && !this->m_images.at(idx).isNull())
            painter.drawPixmap(p.x() * RECT_WIDTH, p.y() * RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT, this->m_images.at(idx));
        else
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
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

    if (0 == this->m_time_left_p1)
    {
        painter.setPen(QPen(QColor(Qt::red), 2));
        painter.drawText(50, (int)(RECT_HEIGHT * 0.8), 150, 50, Qt::AlignLeft, "TIMEOUT");
    }
    else if (this->m_timeout_match > this->m_T1->getElapsed())
    {
        this->m_time_left_p1 = this->m_timeout_match - this->m_T1->getElapsed();
        painter.setPen(QPen(QColor(Qt::black), 2));
        painter.drawText(50, (int)(RECT_HEIGHT * 0.8), 150, 50, Qt::AlignLeft, QString::fromStdString(to_string(this->m_time_left_p1) + "ms"));
    }
    else
    {
        this->m_time_left_p1 = 0;
        this->OnActionEnd();
    }

    if (0 == this->m_time_left_p2)
    {
        painter.setPen(QPen(QColor(Qt::red), 2));
        painter.drawText(this->geometry().width() - 200, (int)(RECT_HEIGHT * 0.8), 150, 50, Qt::AlignRight, "TIMEOUT");
    }
    else if (this->m_timeout_match > this->m_T2->getElapsed())
    {
        this->m_time_left_p2 = this->m_timeout_match - this->m_T2->getElapsed();
        painter.setPen(QPen(QColor(Qt::black), 2));
        painter.drawText(this->geometry().width() - 200, (int)(RECT_HEIGHT * 0.8), 150, 50, Qt::AlignRight, QString::fromStdString(to_string(this->m_time_left_p2) + "ms"));
    }
    else
    {
        this->m_time_left_p2 = 0;
        this->OnActionEnd();
    }
}

void MainWindow::DrawPlayerState()
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

    QPoint pPos;
    pair<int, int> pos_1((int)(RECT_WIDTH * 0.8), RECT_HEIGHT);
    pair<int, int> pos_2(this->geometry().width() - (int)(RECT_WIDTH * 0.8), RECT_HEIGHT);

    if ((GAME_STATE::PLAYING == this->mState) && (this->m_bOK_P1 || !(this->m_manager->m_p1->m_isComputer)))
    {
        if (GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS) || this->m_manager->m_p1->m_isMyTurn)
            painter.setBrush(Qt::green);
        else
            painter.setBrush(Qt::red);

        pPos.setX(pos_1.first);
        pPos.setY(pos_1.second);

        QPoint ptCenter(pPos.x(), pPos.y());
        painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }
    else
    {
        painter.setBrush(Qt::gray);
        pPos.setX(pos_1.first);
        pPos.setY(pos_1.second);

        QPoint ptCenter(pPos.x(), pPos.y());
        painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }

    if ((GAME_STATE::PLAYING == this->mState) && (this->m_bOK_P2 || !(this->m_manager->m_p2->m_isComputer)))
    {
        if (GAME_RULE::CONTINUOUS != (this->m_Rule & GAME_RULE::CONTINUOUS) && this->m_manager->m_p2->m_isMyTurn)
            painter.setBrush(Qt::green);
        else
            painter.setBrush(Qt::red);

        pPos.setX(pos_2.first);
        pPos.setY(pos_2.second);

        QPoint ptCenter(pPos.x(), pPos.y());
        painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }
    else
    {
        painter.setBrush(Qt::gray);
        pPos.setX(pos_2.first);
        pPos.setY(pos_2.second);

        QPoint ptCenter(pPos.x(), pPos.y());
        painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.25), (int)(RECT_HEIGHT * 0.25));
    }
}

void MainWindow::DrawPlayerStone()
{
    QPainter painter(this);
    painter.setPen(QPen(QColor(Qt::transparent)));
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

    if (STONECOLOR::BLACK == this->m_manager->m_p1->m_color)
    {
        if (this->m_bSkin && !this->m_images[1].isNull())
            painter.drawPixmap((int)(0.25 * RECT_WIDTH), this->geometry().height() - (int)(1.25 * RECT_HEIGHT), RECT_WIDTH, RECT_HEIGHT, this->m_images.at(1));
        else
        {
            painter.setBrush(Qt::black);
            QPoint ptCenter((int)(0.75 * RECT_WIDTH), this->geometry().height() - (int)(0.75 * RECT_HEIGHT));
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
        }
    }
    else
    {
        if (this->m_bSkin && !this->m_images[2].isNull())
            painter.drawPixmap((int)(0.25 * RECT_WIDTH), this->geometry().height() - (int)(1.25 * RECT_HEIGHT), RECT_WIDTH, RECT_HEIGHT, this->m_images.at(2));
        else
        {
            painter.setBrush(Qt::white);
            QPoint ptCenter((int)(0.75 * RECT_WIDTH), this->geometry().height() - (int)(0.75 * RECT_HEIGHT));
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
        }
    }

    if (STONECOLOR::WHITE == this->m_manager->m_p2->m_color)
    {
        if (this->m_bSkin && !this->m_images[2].isNull())
            painter.drawPixmap(this->geometry().width() - (int)(1.25 * RECT_WIDTH), this->geometry().height() - (int)(1.25 * RECT_HEIGHT), RECT_WIDTH, RECT_HEIGHT, this->m_images.at(2));
        else
        {
            painter.setBrush(Qt::white);
            QPoint ptCenter(this->geometry().width() - (int)(0.75 * RECT_WIDTH), this->geometry().height() - (int)(0.75 * RECT_HEIGHT));
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
        }
    }
    else
    {
        if (this->m_bSkin && !this->m_images[1].isNull())
            painter.drawPixmap(this->geometry().width() - (int)(1.25 * RECT_WIDTH), this->geometry().height() - (int)(1.25 * RECT_HEIGHT), RECT_WIDTH, RECT_HEIGHT, this->m_images.at(1));
        else
        {
            painter.setBrush(Qt::black);
            QPoint ptCenter(this->geometry().width() - (int)(0.75 * RECT_WIDTH), this->geometry().height() - (int)(0.75 * RECT_HEIGHT));
            painter.drawEllipse(ptCenter, (int)(RECT_WIDTH * 0.5), (int)(RECT_HEIGHT * 0.5));
        }
    }
}

void MainWindow::DrawPlayerName()
{
    QFont font;
    font.setPixelSize(15);
    font.setUnderline(true);
    //font.setItalic(true);
    font.setBold(true);

    QPainter painter(this);
    painter.setFont(font);
    painter.setPen(QPen(QColor(Qt::black), 2));

    if (!this->m_p1_name.isEmpty())
        painter.drawText(50, this->geometry().height() - 30, 150, 50, Qt::AlignLeft, this->m_p1_name);
    else if (this->m_manager->m_p1->m_isComputer)
        painter.drawText(50, this->geometry().height() - 30, 150, 50, Qt::AlignLeft, "AI");
    else
        painter.drawText(50, this->geometry().height() - 30, 150, 50, Qt::AlignLeft, "Human");

    if (!this->m_p2_name.isEmpty())
        painter.drawText(this->geometry().width() - 200, this->geometry().height() - 30, 150, 50, Qt::AlignRight, this->m_p2_name);
    else if (this->m_manager->m_p2->m_isComputer)
        painter.drawText(this->geometry().width() - 200, this->geometry().height() - 30, 150, 50, Qt::AlignRight, "AI");
    else
        painter.drawText(this->geometry().width() - 200, this->geometry().height() - 30, 150, 50, Qt::AlignRight, "Human");
}

void MainWindow::DrawIndication()
{
    if (nullptr != this->mBoard)
    {
        QFont font;
        font.setPixelSize(10);
        font.setItalic(true);
        // font.setBold(true);

        QPainter painter(this);
        painter.setFont(font);
        painter.setPen(QPen(QColor(Qt::black), 2));

        for (size_t i = 0; i < this->mBoard->getBSize().first; i++)
            painter.drawText((i + 1) * RECT_WIDTH + 5, (int)(RECT_HEIGHT * 1.5), 20, 20, Qt::AlignRight, QString::fromStdString(to_string(i)));

        for (size_t i = 0; i < this->mBoard->getBSize().second; i++)
            painter.drawText((int)(RECT_WIDTH * 0.5), (i + 2) * RECT_HEIGHT + 5, 20, 20, Qt::AlignBottom, QString::fromStdString(to_string(i)));
    }
}

vector<pair<pair<int,int>, int>> MainWindow::record_expand(const vector<pair<int, int>> vRecord, bool bContinuous)
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
        if (bContinuous)
            vElement.second = 3;
        else
            vElement.second = iter->second;
        vRecExpendTmp.push_back(vElement);
    }

    return vRecExpendTmp;
}

void MainWindow::mousePressEvent(QMouseEvent * e)
{
    if (this->mState == GAME_STATE::PLAYING)
    {
        if ((GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS)) || (this->m_manager->m_p1->m_isMyTurn && this->m_manager->m_p1->m_isComputer) || (this->m_manager->m_p2->m_isMyTurn && this->m_manager->m_p2->m_isComputer))
            return;

        QPoint pt;
        pt.setX( (e->pos().x() ) / RECT_WIDTH);
        pt.setY( (e->pos().y() ) / RECT_HEIGHT);

        if (pt.y() < 2 || pt.x() < 1)
            return;

        pair<int, int> p_idx(pt.x() - 1, pt.y() - 2);

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
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord());
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
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord());
                        this->m_manager->sendBoard(vRecExpendTmp);
                        this->m_bBoard = false;
                    }
                    else
                    {
                        this->m_manager->turn_2_p2(p_idx.first, p_idx.second);
                    }
                }

                if (this->mBoard->GetState() == BOARDSTATUS::BOARDFULL)
                {
                    this->OnActionEnd();
                    QMessageBox::information(this, "Game Over", "Draw!");
                    return;
                }
            }
            else
                return;
        }
        else
        {
            this->OnActionEnd();
            QMessageBox::information(this, "Game Over", "Draw!");
            return;
        }

        //if connect five
        bool isWin = false;
        int i_win = 0;
        isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
        if (GAME_RULE::STANDARDGOMOKU == (this->m_Rule & GAME_RULE::STANDARDGOMOKU))
        {
            if (this->m_standardGomoku->checkWin(this->mBoard))
                i_win |= GAME_RULE::STANDARDGOMOKU;
            else
                isWin = false;
        }
        if (GAME_RULE::RENJU == (this->m_Rule &GAME_RULE::RENJU))
        {
            if (this->m_renju->checkWin(this->mBoard))
                i_win |= GAME_RULE::RENJU;
            else
                isWin = false;
        }
        if (GAME_RULE::CARO == (this->m_Rule & GAME_RULE::CARO))
        {
            if (this->m_caro->checkWin(this->mBoard))
                i_win |= GAME_RULE::CARO;
            else
                isWin = false;
        }

        if (0 != i_win)
        {
            if ((this->m_Rule & i_win) == this->m_Rule)
                isWin = true;
            else
                isWin = false;
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
        else if (0x04 == (this->m_Rule & 0x04))
        {
            if (!this->m_renju->isLegal(this->mBoard))
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

        bool bStart = false;
        bool bAttach = false;
        //continuous game
        if (GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS))
        {
            if (!this->m_manager->m_p1->m_isComputer || this->m_manager->m_p1->m_sPath.isEmpty())
            {
                QMessageBox::information(this, "Error!", "Engine is necessary for continuous game.\nPlease check the setting of player 1!");
                return;
            }

            bAttach = this->m_manager->AttachEngines();
            qDebug() << "AttachFlag: " << bAttach;
            if (bAttach)
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
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
                this->m_manager->sendAbout();

                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
                this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(this->m_Rule).c_str());
            }
            else
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                }

                this->m_manager->DetachEngines();
                QMessageBox::information(this, "Error!", "Failied to start game!");
                return;
            }

            this->m_manager->infoMatch_p1(INFO_KEY::TIME_LEFT, to_string(this->m_time_left_p1).c_str());
            this->m_T1->start();
        }
        else // is not continuous game
        {
            bAttach = this->m_manager->AttachEngines();
            qDebug() << "AttachFlag: " << bAttach;
            if (bAttach)
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                }
                if (nullptr != this->m_manager->m_engine_2)
                {
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                    connect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
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
                this->m_manager->sendAbout();

                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
                this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(this->m_Rule).c_str());

                this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                this->m_manager->infoMatch_p2(INFO_KEY::GAME_TYPE, "0");
                this->m_manager->infoMatch_p2(INFO_KEY::RULE, to_string(this->m_Rule).c_str());
            }
            else
            {
                if (nullptr != this->m_manager->m_engine_1)
                {
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                }
                if (nullptr != this->m_manager->m_engine_2)
                {
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                    disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
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
        }

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
                if (GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS))
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                else
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
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

        this->m_bOK_P1 = false;
        this->m_bOK_P2 = false;
        this->m_p1_name.clear();
        this->m_p2_name.clear();
        this->mState = GAME_STATE::PAUSING;
    }
}

void MainWindow::OnActionContinue()
{
    if (this->mState == GAME_STATE::PAUSING || this->mState == IDLE)
    {
        if (nullptr != this->m_manager)
        {
            bool bAttach = false;
            bool bStart = false;
            // continuous game
            if (GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS))
            {
                if (!this->m_manager->m_p1->m_isComputer || this->m_manager->m_p1->m_sPath.isEmpty())
                {
                    QMessageBox::information(this, "Error!", "Engine is necessary for continuous game.\nPlease check the setting of player 1!");
                    return;
                }

                bAttach = this->m_manager->AttachEngines();
                qDebug() << "AttachFlag: " << bAttach;
                if (bAttach)
                {
                    if (nullptr != this->m_manager->m_engine_1)
                    {
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
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
                    this->m_manager->sendAbout();

                    this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
                    this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(this->m_Rule).c_str());

                }
                else
                {
                    if (nullptr != this->m_manager->m_engine_1)
                    {
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                    }
                    this->m_manager->DetachEngines();
                    QMessageBox::information(this, "Error!", "Failied to start game!");
                    return;
                }

                vector<pair<pair<int, int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord(), true);

                this->mBoard->Notify();
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
                this->m_manager->sendBoard(vRecExpendTmp, true);
            }
            else    // is not continuous game
            {
                bAttach = this->m_manager->AttachEngines();
                qDebug() << "AttachFlag: " << bAttach;
                if (bAttach)
                {
                    if (nullptr != this->m_manager->m_engine_1)
                    {
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                        connect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                    }
                    if (nullptr != this->m_manager->m_engine_2)
                    {
                        connect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                        connect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                        connect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
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
                    this->m_manager->sendAbout();

                    this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                    this->m_manager->infoMatch_p1(INFO_KEY::GAME_TYPE, "0");
                    this->m_manager->infoMatch_p1(INFO_KEY::RULE, to_string(this->m_Rule).c_str());

                    this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_MATCH, to_string(this->m_timeout_match).c_str());
                    this->m_manager->infoMatch_p2(INFO_KEY::TIMEOUT_TURN, to_string(this->m_timeout_turn).c_str());
                    this->m_manager->infoMatch_p2(INFO_KEY::MAX_MEMORY, to_string(this->m_max_memory).c_str());
                    this->m_manager->infoMatch_p2(INFO_KEY::GAME_TYPE, "0");
                    this->m_manager->infoMatch_p2(INFO_KEY::RULE, to_string(this->m_Rule).c_str());
                }
                else
                {
                    if (nullptr != this->m_manager->m_engine_1)
                    {
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                        disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
                    }
                    if (nullptr != this->m_manager->m_engine_2)
                    {
                        disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                        disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                        disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
                        disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_error()), this, SLOT(OnP2ResponseError()));
                        disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_unknown()), this, SLOT(OnP2ResponseUnknown()));
                    }
                    this->m_manager->DetachEngines();
                    QMessageBox::information(this, "Error!", "Failied to start game!");
                    return;
                }

                vector<pair<pair<int, int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord());

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
                this->m_bBoard = true;
            }
        }

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
                if (GAME_RULE::CONTINUOUS == (this->m_Rule & GAME_RULE::CONTINUOUS))
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnContinuousPos(int, int)));
                else
                    disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP1PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_name(QString)), this, SLOT(OnP1ResponseName(QString)));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_ok()), this, SLOT(OnP1ResponseOk()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_error()), this, SLOT(OnP1ResponseError()));
                disconnect(this->m_manager->m_engine_1, SIGNAL(responsed_unknown()), this, SLOT(OnP1ResponseUnknown()));
            }
            if (nullptr != this->m_manager->m_engine_2)
            {
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_pos(int, int)), this, SLOT(OnP2PlaceStone(int, int)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_name(QString)), this, SLOT(OnP2ResponseName(QString)));
                disconnect(this->m_manager->m_engine_2, SIGNAL(responsed_ok()), this, SLOT(OnP2ResponseOk()));
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

        this->m_bOK_P1 = false;
        this->m_bOK_P2 = false;
        this->m_p1_name.clear();
        this->m_p2_name.clear();
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
                resize((this->mBoard->getBSize().first + 2) * RECT_WIDTH, (this->mBoard->getBSize().second + 3) * RECT_HEIGHT + this->pMenuBar->height());

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

void MainWindow::OnActionSkin()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        bool ok = false;
        QStringList s_items;
        s_items << "none" << "bold" << "fontanGomo" << "gems" << "gomoku" << "HGarden" << "HGarden2" << "light" << "pisq" << "rain" << "star" << "whitex" << "wood" << "yellow";
        QString s_get = QInputDialog::getItem(this, "Skin", "Please choose skin:", s_items, 0, false,
                                            &ok, Qt::MSWindowsFixedSizeDialogHint);
        if (ok)
        {
            if (QString::compare(s_get, "none") == 0)
            {
                this->m_bSkin = false;
                this->m_images.clear();
            }
            else
            {
                bool bLoad = false;
                QPixmap pm;
                this->m_images.clear();

                if (QString::compare(s_get, "bold") == 0)
                    bLoad = pm.load(":/skins/bold.bmp");
                else if (QString::compare(s_get, "fontanGomo") == 0)
                    bLoad = pm.load(":/skins/fontanGomo.bmp");
                else if (QString::compare(s_get, "gems") == 0)
                    bLoad = pm.load(":/skins/gems.bmp");
                else if (QString::compare(s_get, "gomoku") == 0)
                    bLoad = pm.load(":/skins/gomoku.bmp");
                else if (QString::compare(s_get, "HGarden") == 0)
                    bLoad = pm.load(":/skins/HGarden.bmp");
                else if (QString::compare(s_get, "HGarden2") == 0)
                    bLoad = pm.load(":/skins/HGarden2.bmp");
                else if (QString::compare(s_get, "light") == 0)
                    bLoad = pm.load(":/skins/light.bmp");
                else if (QString::compare(s_get, "pisq") == 0)
                    bLoad = pm.load(":/skins/pisq.bmp");
                else if (QString::compare(s_get, "rain") == 0)
                    bLoad = pm.load(":/skins/rain.bmp");
                else if (QString::compare(s_get, "star") == 0)
                    bLoad = pm.load(":/skins/star.bmp");
                else if (QString::compare(s_get, "whitex") == 0)
                    bLoad = pm.load(":/skins/whitex.bmp");
                else if (QString::compare(s_get, "wood") == 0)
                    bLoad = pm.load(":/skins/wood.bmp");
                else if (QString::compare(s_get, "yellow") == 0)
                    bLoad = pm.load(":/skins/yellow.bmp");

                if (!pm.isNull() && bLoad)
                {
                    for (size_t i = 0; i < 5; i++)
                        this->m_images.push_back(pm.copy((int)(i * (pm.width()) * 0.2), 0, (int)((pm.width()) * 0.2), pm.height()).scaled(RECT_WIDTH, RECT_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                    if (this->m_images.size() != 5)
                        this->m_bSkin = false;
                    else
                        this->m_bSkin = true;
                }
            }
        }
    }
}

void MainWindow::On_ClickedRuleActionGroup(QAction *pAction)
{
    if (this->mState == GAME_STATE::OVER || this->mState == GAME_STATE::IDLE)
    {
        if (pAction->text().compare(this->pActionFreeStyleGomoku->text())==0)
        {
            if (this->pActionFreeStyleGomoku->isChecked())
            {
                qDebug() << "Choose Free-Style gomoku!";
                this->m_Rule |= GAME_RULE::FREESTYLEGOMOKU;
            }
            else
            {
                qDebug() << "Cancel Free-Style gomoku!";
                this->m_Rule &= (~(GAME_RULE::FREESTYLEGOMOKU));
            }
        }
        else if (pAction->text().compare(this->pActionStandardGomoku->text()) == 0)
        {
            if (this->pActionStandardGomoku->isChecked())
            {
                qDebug() << "Choose standard gomoku!";
                this->m_Rule |= GAME_RULE::STANDARDGOMOKU;
            }
            else
            {
                qDebug() << "Cancel standard gomoku!";
                this->m_Rule &= (~(GAME_RULE::STANDARDGOMOKU));
            }
        }
        else if (pAction->text().compare(this->pActionContinuous->text()) == 0)
        {
            if (this->pActionContinuous->isChecked())
            {
                qDebug() << "Choose continuous!";
                this->m_Rule |= GAME_RULE::CONTINUOUS;
                QMessageBox::information(this, "Tips", "It will select the engine of player 1 to begin continuous game!\n1, no turn;\n2, self play continuously.\nPlease check the setting of player 1.");
            }
            else
            {
                qDebug() << "Cancel continuous!";
                this->m_Rule &= (~(GAME_RULE::CONTINUOUS));
            }
        }
        else if (pAction->text().compare(this->pActionRenju->text())==0)
        {
            if (this->pActionRenju->isChecked())
            {
                qDebug() << "Choose renju!";
                this->m_Rule |= GAME_RULE::RENJU;
            }
            else
            {
                qDebug() << "Cancel renju!";
                this->m_Rule &= (~(GAME_RULE::RENJU));
            }
        }
        else if (pAction->text().compare(this->pActionCaro->text())==0)
        {
            if (this->pActionCaro->isChecked())
            {
                qDebug() << "Choose caro!";
                this->m_Rule |= GAME_RULE::CARO;
            }
            else
            {
                qDebug() << "Cancel caro!";
                this->m_Rule &= (~(GAME_RULE::CARO));
            }
        }
        else
        {
            qDebug() << "Choose Free-Style gomoku!";
            this->m_Rule = GAME_RULE::FREESTYLEGOMOKU;
        }
        qDebug() << "m_Rule: " << this->m_Rule;
    }
}

void MainWindow::OnActionPlayerSetting()
{
    if (this->mState != GAME_STATE::PLAYING)
    {
        this->m_player_setting->exec();

        if (nullptr != this->m_manager)
        {
            this->m_manager->m_p1->m_sPath = this->m_player_setting->getP1Path();
            this->m_manager->m_p2->m_sPath = this->m_player_setting->getP2Path();
            this->m_manager->m_p1->m_isComputer = !(this->m_player_setting->isP1Human());
            this->m_manager->m_p2->m_isComputer = !(this->m_player_setting->isP2Human());
            qDebug() << "p1: " << this->m_manager->m_p1->m_sPath;
            qDebug() << "p2: " << this->m_manager->m_p2->m_sPath;
        }
    }
}

void MainWindow::OnActionVer()
{
    const QString strVerNum = "Ver Num: 0.4.11\n";
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
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord());
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

            if (this->mBoard->GetState() == BOARDSTATUS::BOARDFULL)
            {
                this->OnActionEnd();
                QMessageBox::information(this, "Game Over", "Draw!");
                return;
            }
        }
        else
        {
            this->OnActionEnd();
            QMessageBox::information(this, "Game Over", "Draw!");
            return;
        }

        // if connect five
        bool isWin = false;
        int i_win = 0;
        isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
        if (GAME_RULE::STANDARDGOMOKU == (this->m_Rule & GAME_RULE::STANDARDGOMOKU))
        {
            if (this->m_standardGomoku->checkWin(this->mBoard))
                i_win |= GAME_RULE::STANDARDGOMOKU;
            else
                isWin = false;
        }
        if (GAME_RULE::RENJU == (this->m_Rule &GAME_RULE::RENJU))
        {
            if (this->m_renju->checkWin(this->mBoard))
                i_win |= GAME_RULE::RENJU;
            else
                isWin = false;
        }
        if (GAME_RULE::CARO == (this->m_Rule & GAME_RULE::CARO))
        {
            if (this->m_caro->checkWin(this->mBoard))
                i_win |= GAME_RULE::CARO;
            else
                isWin = false;
        }

        if (0 != i_win)
        {
            if ((this->m_Rule & i_win) == this->m_Rule)
                isWin = true;
            else
                isWin = false;
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
        else if (0x04 == (this->m_Rule & 0x04))
        {
            if (!this->m_renju->isLegal(this->mBoard))
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
                        vector<pair<pair<int,int>, int>> vRecExpendTmp = this->record_expand(this->mBoard->getVRecord());
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

            if (this->mBoard->GetState() == BOARDSTATUS::BOARDFULL)
            {
                this->OnActionEnd();
                QMessageBox::information(this, "Game Over", "Draw!");
                return;
            }
        }
        else
        {
            this->OnActionEnd();
            QMessageBox::information(this, "Game Over", "Draw!");
            return;
        }

        // if connect five
        bool isWin = false;
        int i_win = 0;
        isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
        if (GAME_RULE::STANDARDGOMOKU == (this->m_Rule & GAME_RULE::STANDARDGOMOKU))
        {
            if (this->m_standardGomoku->checkWin(this->mBoard))
                i_win |= GAME_RULE::STANDARDGOMOKU;
            else
                isWin = false;
        }
        if (GAME_RULE::RENJU == (this->m_Rule &GAME_RULE::RENJU))
        {
            if (this->m_renju->checkWin(this->mBoard))
                i_win |= GAME_RULE::RENJU;
            else
                isWin = false;
        }
        if (GAME_RULE::CARO == (this->m_Rule & GAME_RULE::CARO))
        {
            if (this->m_caro->checkWin(this->mBoard))
                i_win |= GAME_RULE::CARO;
            else
                isWin = false;
        }

        if (0 != i_win)
        {
            if ((this->m_Rule & i_win) == this->m_Rule)
                isWin = true;
            else
                isWin = false;
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
        else if (0x04 == (this->m_Rule & 0x04))
        {
            if (!this->m_renju->isLegal(this->mBoard))
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

void MainWindow::OnContinuousPos(int x, int y)
{
    if (this->mState == GAME_STATE::PLAYING)
    {
        pair<int, int> p_idx(x, y);

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
                this->OnActionEnd();
                QMessageBox::information(this, "Game Error", "Might be illegal move from player 1!");
                return;
            }

            if (this->mBoard->GetState() == BOARDSTATUS::BOARDFULL)
            {
                this->OnActionEnd();
                QMessageBox::information(this, "Game Over", "Draw!");
                return;
            }
        }
        else
        {
            this->OnActionEnd();
            QMessageBox::information(this, "Game Over", "Draw!");
            return;
        }

        // if connect five
        bool isWin = false;
        int i_win = 0;
        isWin = this->m_freeStyleGomoku->checkWin(this->mBoard);
        if (GAME_RULE::STANDARDGOMOKU == (this->m_Rule & GAME_RULE::STANDARDGOMOKU))
        {
            if (this->m_standardGomoku->checkWin(this->mBoard))
                i_win |= GAME_RULE::STANDARDGOMOKU;
            else
                isWin = false;
        }
        if (GAME_RULE::RENJU == (this->m_Rule &GAME_RULE::RENJU))
        {
            if (this->m_renju->checkWin(this->mBoard))
                i_win |= GAME_RULE::RENJU;
            else
                isWin = false;
        }
        if (GAME_RULE::CARO == (this->m_Rule & GAME_RULE::CARO))
        {
            if (this->m_caro->checkWin(this->mBoard))
                i_win |= GAME_RULE::CARO;
            else
                isWin = false;
        }

        if (0 != i_win)
        {
            if ((this->m_Rule & i_win) == this->m_Rule)
                isWin = true;
            else
                isWin = false;
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
        else if (0x04 == (this->m_Rule & 0x04))
        {
            if (!this->m_renju->isLegal(this->mBoard))
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

void MainWindow::OnP1ResponseName(const QString &name)
{
    qDebug() << "...................";
    this->m_p1_name = name;
    qDebug() << "p1 name: " << this->m_p1_name;
    qDebug() << "...................";
}

void MainWindow::OnP1ResponseOk()
{
    this->m_bOK_P1 = true;
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

void MainWindow::OnP2ResponseName(const QString &name)
{
    qDebug() << "...................";
    this->m_p2_name = name;
    qDebug() << "p2 name: " << this->m_p2_name;
    qDebug() << "...................";
}

void MainWindow::OnP2ResponseOk()
{
    this->m_bOK_P2 = true;
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

