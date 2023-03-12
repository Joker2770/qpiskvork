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
#include <QInputDialog>
#include <QActionGroup>
#include <QDialog>
#include <QSharedPointer>

#include "board.h"
#include "FreeStyleGomoku.h"
#include "StandardGomoku.h"
#include "EngineLoader.h"
#include "PlayerSettingDialog.h"

#define RECT_WIDTH		50
#define RECT_HEIGHT		50

typedef enum game_state
{
    IDLE,
    PLAYING,
    PAUSing
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
    QInputDialog *pDialogBoardSize;
    QActionGroup *pRuleActionGroup;

public slots:
    void OnActionStart();
    void OnActionPause();
    void OnActionContinue();
    void OnActionClearBoard();
    void OnActionTakeBack();
    void OnActionBoardSize();
    void OnActionPlayerSetting();
    void OnActionVer();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    QSharedPointer<PlayerSettingDialog> m_player_setting;
    void DrawChessboard();
    void DrawItems();

    void DrawChessAtPoint(QPainter& painter,QPoint& pt);

private:
    GAME_STATE mState;
    Board *mBoard;
    bool mIsBlackTurn;
    bool m_bPause;

    FreeStyleGomoku *m_freeStyleGomoku;
    StandardGomoku *m_standardGomoku;

    EngineLoader *m_engine_1;
    EngineLoader *m_engine_2;
};
#endif // MAINWINDOW_H
