/*************************************************************************
    > File Name: renju.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Sat Apr  1 14:49:21 2023
 ************************************************************************/

/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
    Copyright (C) 2023  Jintao Yang <yjt950840@outlook.com>

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

#ifndef __RENJU_H__
#define __RENJU_H__

#include "rules.h"

#include <iostream>
using namespace std;

typedef enum pattern
{
    ROW,
    UNBROKEN_ROW,
    OVERLINE,
    FIVE_IN_A_ROW,
    FOUR,
    STRAIGHT_FOUR,
    THREE,
    DOUBLE_FOUR,
    DOUBLE_THREE
} PATTERN;

class Renju final : public rules
{
public:
    Renju();

    bool checkWin(Board *board) override;
    // after checkWin
    bool isLegal(Board *board);
    // RIF 9.2 for one single intersection: the forbidden pattern that a black
    // move on (x, y) would make, PATTERN::ROW when black is allowed to play
    // there (a stone that attains exactly five in a row is a win and never a
    // forbidden move).  Unlike isLegal() this does not depend on the last move
    // of the board and it leaves m_renju_state untouched.
    int getForbiddenPatternAt(Board *board, int x, int y);
    // Every empty intersection where black would make a forbidden move (RIF
    // 9.2), with the pattern that would be made there.  Meant to be asked after
    // every move of a game, so the candidates are pruned to the neighbourhood
    // of the black stones: a three or a four spans at most five intersections.
    void collectForbiddenPoints(Board *board, vector<pair<pair<int, int>, int>> &vPoints);
    int getRenjuState();

private:
    // RIF 9.2 - the moves that are forbidden for black only.  Every judgment is
    // made for the stone that has just been played.
    bool isOverLine(Board *board);    // 9.2.a
    bool isDoubleFour(Board *board);  // 9.2.b
    bool isDoubleThree(Board *board); // 9.2.c, the exceptions of 9.3 applied

    int m_renju_state;
};

#endif
