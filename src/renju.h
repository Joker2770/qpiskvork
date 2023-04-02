/*************************************************************************
    > File Name: renju.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Apr  1 14:49:21 2023
 ************************************************************************/

/**
    qpiskvork is another gomoku manager for adapting to Windows and Linux systems.
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

#include<iostream>
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

// 0 - pos empty, 1 - pos black, 2 - pos white
const int A4_SHAPS[][5] = {
    {0, 1, 1, 1, 1},
    {1, 0, 1, 1, 1},
    {1, 1, 0, 1, 1},
    {1, 1, 1, 0, 1},
    {1, 1, 1, 1, 0}};
// A3 is that add one stone could be A4
const int A3_SHAPS[][6] = {
    {0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 1, 0},
    {0, 1, 1, 0, 1, 0}};

class Renju final : public rules
{
public:
    bool isIllegal(Board *board);
    bool checkWin(Board *board) override;
    int getRenjuState();
private:
    bool isOverLine(Board *board);
    //after overline, include 44*
    bool isDoubleFour(Board *board);
    //after double-four, only 43
    bool isFourThree(Board *board);
    //after four-three, only 4 and no 3
    bool isFour(Board *board);
    //after four, include 433*, 33*
    bool isDoubleThree(Board *board);
    //after double-three, only 3
    bool isThree(Board *board);
    int countA4(Board *board, const pair<int, int>& p_drt);
    int countA3(Board *board, const pair<int, int>& p_drt);
    int countStoneInFour(Board *board, const pair<int, int>& p_drt);
    int countNearStone(Board *board, const pair<int, int>& p_drt);

    int m_renju_state;
};

#endif
