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
#include <QPixmap>

#include "board.h"
#include "FreeStyleGomoku.h"
#include "StandardGomoku.h"
#include "renju.h"
#include "caro.h"
#include "manager.h"
#include "PlayerSettingDialog.h"
#include "S2BResDialog.h"
#include "timer.h"
#include "customs.h"

typedef enum game_state
{
    IDLE,
    PLAYING,
    PAUSING,
    OVER
} GAME_STATE;

typedef enum game_rule
{
    FREESTYLEGOMOKU = 0,
    STANDARDGOMOKU = 1,
    CONTINUOUS = 2,
    RENJU = 4,
    CARO = 8
} GAME_RULE;

const QString g_szSkins[13] = {
    ":/skins/bold.bmp",
    ":/skins/fontanGomo.bmp",
    ":/skins/gems.bmp",
    ":/skins/gomoku.bmp",
    ":/skins/HGarden.bmp",
    ":/skins/HGarden2.bmp",
    ":/skins/light.bmp",
    ":/skins/pisq.bmp",
    ":/skins/rain.bmp",
    ":/skins/star.bmp",
    ":/skins/whitex.bmp",
    ":/skins/wood.bmp",
    ":/skins/yellow.bmp"
};

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
    QMenu *pMenuShow;
    QMenu *pMenuAbout;
    QAction *pActionBoardSize;
    QAction *pActionTimeoutMatch;
    QAction *pActionTimeoutTurn;
    QAction *pActionMaxMemory;
    QAction *pActionSkin;
    QAction *pActionSwap2Board;
    QAction *pActionStart;
    QAction *pActionPause;
    QAction *pActionContinue;
    QAction *pActionEnd;
    QAction *pActionClear;
    QAction *pActionTakeBack;
    QAction *pActionFreeStyleGomoku;
    QAction *pActionStandardGomoku;
    QAction *pActionContinuous;
    QAction *pActionRenju;
    QAction *pActionCaro;
    QAction *pActionNumOfMove;
    QAction *pActionXAxisLetter;
    QAction *pActionYAxisLetter;
    QAction *pActionXAxisStartFrom_1;
    QAction *pActionYAxisStartFrom_1;
    QAction *pActionTimeSecond;
    QAction *pActionGridSize;
    QAction *pActionPlayerSetting;
    QAction *pActionVer;
    QAction *pActionFeedback;
    QAction *pActionLicense;
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
    void OnActionGridSize();
    void OnActionSkin();
    void On_ClickedRuleActionGroup(QAction *);
    void OnActionPlayerSetting();
    void OnActionNumOfMove();
    void OnActionVer();
    void OnActionFeedback();
    void OnActionLicense();
    void OnP1PlaceStone(int x, int y);
    void OnP2PlaceStone(int x, int y);
    void OnContinuousPos(int x, int y);
    void OnP1Responsed2Pos(int x_1, int y_1, int x_2, int y_2);
    void OnP1Responsed3Pos(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);
    void OnP1ResponsedSwap();
    void OnP1ResponseName(const QString &name);
    void OnP1ResponseOk();
    void OnP1ResponseError();
    void OnP1ResponseUnknown();
    void OnP2Responsed2Pos(int x_1, int y_1, int x_2, int y_2);
    void OnP2Responsed3Pos(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);
    void OnP2ResponsedSwap();
    void OnP2ResponseName(const QString &name);
    void OnP2ResponseOk();
    void OnP2ResponseError();
    void OnP2ResponseUnknown();
    void beginSwap2Board();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    QSharedPointer<PlayerSettingDialog> m_player_setting;
    S2BResDialog *m_S2BRes_1, *m_S2BRes_2, *m_S2BRes_3;
    void DrawChessboard();
    void DrawItems();
    void DrawStepNum();
    void DrawMark();
    void DrawTimeLeft();
    void DrawPlayerState();
    void DrawPlayerStone();
    void DrawPlayerName();
    void DrawIndication();

    void connectP1Signals();
    void connectP2Signals();
    void disconnectP1Signals();
    void disconnectP2Signals();

    vector<pair<pair<int, int>, int>> record_expand(const vector<pair<int, int>> &vRecord, bool bContinuous = false);

private:
    Board *mBoard;
    Manager *m_manager;
    FreeStyleGomoku *m_freeStyleGomoku;
    StandardGomoku *m_standardGomoku;
    Renju *m_renju;
    Caro *m_caro;
    Timer *m_T1;
    Timer *m_T2;
    Customs *m_customs;
    vector<QPixmap> m_images;
    QString m_p1_name;
    QString m_p2_name;
    long long m_timeout_match;
    long long m_timeout_turn;
    long long m_max_memory;
    long long m_time_left_p1;
    long long m_time_left_p2;
    unsigned int m_cur_skin_idx;
    int m_Rule;
    int RECT_WIDTH;
    int RECT_HEIGHT;
    GAME_STATE mState;
    bool m_bBoard;
    bool m_bSwap2Board;
    bool m_bS2B_over;
    bool m_bSkin;
    bool m_bNumOfMove;
    bool m_bOK_P1;
    bool m_bOK_P2;
};
#endif // MAINWINDOW_H
