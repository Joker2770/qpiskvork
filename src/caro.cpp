/*************************************************************************
    > File Name: caro.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Thu Apr  6 18:03:05 2023
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

#include "caro.h"

#include <algorithm>
#include<iostream>
using namespace std;

bool Caro::findShap(Board *board,  const pair<int, int>& p_drt)
{
    if (board->getVRecord().size() >= 9)
    {
        vector<int> vColor;

        pair<int, int> p_last_move = board->getVRecord().back();
        pair<int, int> p_idx = board->coord2idx(p_last_move.first);
        pair<int, int> p_drt_idx(p_idx.first + p_drt.first, p_idx.second + p_drt.second);

        // push back current stone color
        if (STONECOLOR::BLACK == board->getIdxStoneColor(p_idx))
            vColor.push_back(1);
        else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_idx))
            vColor.push_back(2);
        else
            return false;

        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first += p_drt.first;
            p_drt_idx.second += p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 5 || abs(p_idx.second - p_drt_idx.second) > 5)
                break;
        }

        reverse(vColor.begin(), vColor.end());
        p_drt_idx.first = p_idx.first - p_drt.first;
        p_drt_idx.second = p_idx.second - p_drt.second;
        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first -= p_drt.first;
            p_drt_idx.second -= p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 5 || abs(p_idx.second - p_drt_idx.second) > 5)
                break;
        }

        // for (size_t i = 0; i < vColor.size(); i++)
        // {
        //     cout << vColor[i] ;
        // }
        // cout <<endl;

        if (vColor.size() >= 7)
        {
            for (size_t j = 0; j <= vColor.size() - 7; ++j)
            {
                for (size_t i = 0; i < 20; ++i)
                {
                    if (((WIN_SHAPES[i][0]) == (vColor[j])) &&
                        ((WIN_SHAPES[i][1]) == (vColor[j + 1])) &&
                        ((WIN_SHAPES[i][2]) == (vColor[j + 2])) &&
                        ((WIN_SHAPES[i][3]) == (vColor[j + 3])) &&
                        ((WIN_SHAPES[i][4]) == (vColor[j + 4])) &&
                        ((WIN_SHAPES[i][5]) == (vColor[j + 5])) &&
                        ((WIN_SHAPES[i][6]) == (vColor[j + 6])))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Caro::checkWin(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    bool b_up = findShap(board, p_drt_up);
    bool b_left = findShap(board, p_drt_left);
    bool b_leftup = findShap(board, p_drt_leftup);
    bool b_leftdown = findShap(board, p_drt_leftdown);

    if (b_up || b_left || b_leftup || b_leftdown)
        return true;

    return false;
}
