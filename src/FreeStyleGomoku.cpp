/*************************************************************************
    > File Name: FreeStyleGomoku.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Mon Oct 10 12:54:50 2022
 ************************************************************************/

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

#include<iostream>
#include "FreeStyleGomoku.h"
using namespace std;

int FreeStyleGomoku::countNearStone(Board *board, const pair<int, int>& p_drt)
{
    int i_count = 0;
    if (board->GetState() == BOARDEMPTY)    return 0;

    pair<int, int> p_last_move = board->getVRecord().back();
    pair<int, int> p_idx = board->coord2idx(p_last_move.first);
    pair<int, int> p_drt_idx(p_idx.first + p_drt.first, p_idx.second + p_drt.second);

    while (!board->isPosOutOfBoard(p_drt_idx) && !board->isPosEmpty(p_drt_idx))
    {
        for (size_t i = 0; i < board->getVRecord().size(); ++i)
        {
            if (board->getVRecord().at(i).first == board->idx2Coord(p_drt_idx))
            {
                if (board->getVRecord().at(i).second == p_last_move.second)
                {
                    i_count++;
                    break;
                }
                else
                    return i_count;
            }
        }
        p_drt_idx.first += p_drt.first;
        p_drt_idx.second += p_drt.second;
        if (abs(p_idx.first - p_drt_idx.first) > 5 || abs(p_idx.second - p_drt_idx.second) > 5)
            break;
    }

    return i_count;
}

bool FreeStyleGomoku::checkWin(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_down(0, 1), p_drt_left(-1, 0), p_drt_right(1, 0)
            , p_drt_leftup(-1, -1), p_drt_rightdown(1, 1), p_drt_rightup(1, -1), p_drt_leftdown(-1, 1);
    int i_up = countNearStone(board, p_drt_up);
    int i_down = countNearStone(board, p_drt_down);
    int i_left = countNearStone(board, p_drt_left);
    int i_right = countNearStone(board, p_drt_right);
    int i_leftup = countNearStone(board, p_drt_leftup);
    int i_rightdown = countNearStone(board, p_drt_rightdown);
    int i_leftdown = countNearStone(board, p_drt_leftdown);
    int i_rightup = countNearStone(board, p_drt_rightup);

    if (i_up + i_down >= 4 || i_left + i_right >= 4 || i_leftup + i_rightdown >= 4 || i_leftdown + i_rightup >= 4)
        return true;

    return false;
}

