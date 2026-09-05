/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
    Copyright (C) 2022-2026 Jintao Yang <yjt950840@outlook.com>

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
#include <QCloseEvent>
#include <QTimer>

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

// 棋盘绘制布局常量：棋盘绘制中使用的各种偏移量统一在此管理，调整布局时只需修改这里。
namespace BoardLayout
{
    // ---- 棋盘网格整体布局（单位：格）----
    constexpr int LEFT_MARGIN_CELLS = 1;   // 棋盘左侧留白格数
    constexpr int RIGHT_MARGIN_CELLS = 1;  // 棋盘右侧留白格数
    constexpr int TOP_MARGIN_CELLS = 2;    // 棋盘顶部留白格数（第 1 格计时/状态，第 2 格 X 轴标注）
    constexpr int BOTTOM_MARGIN_CELLS = 1; // 棋盘底部留白格数（底部为玩家棋子/名字信息区）
    constexpr double CELL_CENTER = 0.5;    // 格内中心偏移（半格）

    // ---- 棋子与标记尺寸（占格宽/格高的比例）----
    constexpr double STONE_RADIUS_RATIO = 0.46;      // 棋子半径
    constexpr double MARK_RADIUS_RATIO = 0.25;      // 最后一手标记半径
    constexpr double STATE_DOT_RADIUS_RATIO = 0.25; // 玩家状态圆点半径
    constexpr double STATE_DOT_X_RATIO = 0.8;       // 玩家状态圆点横向位置

    // ---- 坐标轴标注 ----
    constexpr double Y_AXIS_LABEL_X_RATIO = 0.4;                             // Y 轴标注横向位置（占格宽比例）
    constexpr double X_AXIS_LABEL_Y_RATIO = TOP_MARGIN_CELLS - CELL_CENTER;  // X 轴标注纵向位置（占格高比例，居中于标注行）
    constexpr int AXIS_LABEL_PAD = 5;                                        // 标注文字与格线的像素间距

    // ---- 底部玩家信息区 ----
    constexpr double PLAYER_STONE_CORNER_X_RATIO = 0.25;      // 左侧棋子贴图左上角横向位置（占格宽比例）
    constexpr double PLAYER_STONE_CORNER_Y_RATIO = 1.25;      // 棋子贴图左上角距窗口底边（占格高比例）
    constexpr double PLAYER_STONE_CENTER_X_RATIO = 0.75;      // 棋子中心横向位置（占格宽比例）
    constexpr double PLAYER_STONE_CENTER_Y_RATIO = 0.75;      // 棋子中心距窗口底边（占格高比例）
    constexpr double PLAYER_STONE_RIGHT_MARGIN_RATIO = 1.25;  // 右侧棋子贴图距窗口右边缘（占格宽比例）
    constexpr int TEXT_MARGIN = 15;       // 文字距棋盘左右两侧的像素间距
    constexpr int TEXT_RIGHT_SPACE = 170; // 右侧文字距窗口右边缘的像素间距
    constexpr int NAME_TEXT_BOTTOM = 30;  // 玩家名文字距窗口底边的像素距离

    // ---- 计时文字 ----
    constexpr double TIME_TEXT_Y_RATIO = 0.8; // 剩余时间文字纵向位置（占格高比例）
    constexpr int TIME_TEXT_LEFT_X = 50;      // 左侧 "TIMEOUT" 文字 x 坐标
    constexpr int TIME_TEXT_RIGHT_X = 200;    // 右侧 "TIMEOUT" 文字距窗口右边缘的像素间距
}

namespace TimeControl
{
    constexpr int OVERTIME_TURN_COUNT = 3;
    constexpr int OVERTIME_FLASH_INTERVAL_MS = 500;
}

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
    ":/skins/yellow.bmp"};

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
    QMenu *pSubMenuOfLanguage;
    QAction *pActionBoardSize;
    QAction *pActionTimeoutMatch;
    QAction *pActionTimeoutTurn;
    QAction *pActionMaxMemory;
    QAction *pActionOvertime;
    QAction *pActionSkin;
    QAction *pActionLanguage;
    QAction *pActionLangZHCN;
    QAction *pActionLangENUK;
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
    QAction *pActionToggleOpenMind;
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
    QActionGroup *pLanguageActionGroup;

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
    void OnActionOvertime();
    void OnActionGridSize();
    void OnActionSkin();
    void On_ClickedRuleActionGroup(QAction *);
    void On_ClickedLanguageActionGroup(QAction *);
    void OnActionPlayerSetting();
    void OnActionNumOfMove();
    void OnActionToggleOpenMind();
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
    void OnP1Thinking(const QString &sData);
    void OnP2Thinking(const QString &sData);
    void beginSwap2Board();
    void onRepaintTimerTimeout();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *event);

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
    void DrawOpenMind();
    void updateOpenMindData(const QString &sData);
    // 更新一名玩家的计时状态（常规阶段/加时阶段）；若加时机会耗尽则返回 true
    bool updatePlayerClock(Timer *t, long long &timeLeft, long long &turnStartElapsed, int &overtimeUsed);
    // 刷新指定玩家的计时状态并发送 info time_left（常规阶段=总预算、加时阶段=剩余机会总量*timeout_turn-当前机会已用）；若该玩家超时则返回 false
    bool sendTimeLeft(int player);

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
    QTimer *m_repaintTimer;
    Customs *m_customs;
    vector<QPixmap> m_images;
    QString m_p1_name;
    QString m_p2_name;
    long long m_timeout_match;
    long long m_timeout_turn;
    long long m_max_memory;
    bool m_bOvertime;
    long long m_time_left_p1;
    long long m_time_left_p2;
    long long m_turn_start_elapsed_p1; // 加时阶段当前步时起点（累计毫秒），-1 表示尚未进入加时
    long long m_turn_start_elapsed_p2;
    int m_overtime_used_p1;            // 已消耗的加时步时机会数
    int m_overtime_used_p2;
    bool m_prev_p1_turn;               // 上一帧是否为 P1 行棋（用于检测落子切换）
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
    bool m_bOpenMind;
    bool m_bOK_P1;
    bool m_bOK_P2;
    vector<pair<pair<int, int>, double>> m_openMindData;
};
#endif // MAINWINDOW_H
