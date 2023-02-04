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
#include <QInputDialog>
#include <QActionGroup>
#include <QDialog>

#include "board.h"
#include "FreeStyleGomoku.h"
#include "StandardGomoku.h"
#include "EngineLoader.h"

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
    QMenu *pMenuGame;
    QMenu *pMenuSetting;
    QMenu *pMenuPlayer;
    QMenu *pMenuAbout;
    QAction *pActionBoardSize;
    QAction *pActionStart;
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
    void OnActionClearBoard();
    void OnActionTakeBack();
    void OnActionBoardSize();
    void OnActionPlayerSetting();
    void OnActionVer();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

private:
    void DrawChessboard();
    void DrawItems();
    void DrawItemWithMouse();

    void DrawChessAtPoint(QPainter& painter,QPoint& pt);

private:
    Board *mBoard;
    bool mIsBlackTurn;

    FreeStyleGomoku *m_freeStyleGomoku;
    StandardGomoku *m_standardGomoku;

    EngineLoader *m_engine_1;
    EngineLoader *m_engine_2;
};
#endif // MAINWINDOW_H
