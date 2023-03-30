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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMenuBar>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QMap>
#include <QActionGroup>
#include <QDialog>
#include <QSharedPointer>

#include "board.h"
#include "FreeStyleGomoku.h"
#include "StandardGomoku.h"
#include "manager.h"
#include "PlayerSettingDialog.h"
#include "timer.h"

#define RECT_WIDTH		40
#define RECT_HEIGHT		40

typedef enum game_state
{
    IDLE,
    PLAYING,
    PAUSING
} GAME_STATE;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QMenuBar *pMenuBar;
    QMenu *pMenuGame;
    QMenu *pMenuSetting;
    QMenu *pMenuPlayer;
    QMenu *pMenuAbout;
    QAction *pActionBoardSize;
    QAction *pActionTimeoutMatch;
    QAction *pActionTimeoutTurn;
    QAction *pActionMaxMemory;
    QAction *pActionStart;
    QAction *pActionPause;
    QAction *pActionContinue;
    QAction *pActionEnd;
    QAction *pActionClear;
    QAction *pActionTakeBack;
    QAction *pActionFreeStyleGomoku;
    QAction *pActionStandardGomoku;
    QAction *pActionPlayerSetting;
    QAction *pActionVer;
    QAction *pActionFeedback;
    QActionGroup *pRuleActionGroup;

public slots:
    void OnActionStart();
    void OnActionPause();
    void OnActionContinue();
    void OnActionEnd();
    void OnActionClearBoard();
    void OnActionTakeBack();
    void OnActionBoardSize();
    void OnActionTimeoutMatch();
    void OnActionTimeoutTurn();
    void OnActionMaxMemory();
    void OnActionPlayerSetting();
    void OnActionVer();
    void OnActionFeedback();
    void OnP1PlaceStone(int x, int y);
    void OnP2PlaceStone(int x, int y);
    void OnP1ResponseError();
    void OnP1ResponseUnknown();
    void OnP2ResponseError();
    void OnP2ResponseUnknown();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    QSharedPointer<PlayerSettingDialog> m_player_setting;
    void DrawChessboard();
    void DrawItems();
    void DrawTimeLeft();
    void DrawChessAtPoint(QPainter& painter,QPoint& pt);

    vector<pair<pair<int, int>, int>> record_expend(vector<pair<int, int>> vRecord);

private:
    GAME_STATE mState;
    Board *mBoard;
    bool m_bBoard;

    FreeStyleGomoku *m_freeStyleGomoku;
    StandardGomoku *m_standardGomoku;

    Manager *m_manager;

    Timer *m_T1;
    Timer *m_T2;
    long long m_timeout_match;
    long long m_timeout_turn;
    long long m_max_memory;
    long long m_time_left_p1;
    long long m_time_left_p2;
};
#endif // MAINWINDOW_H
