/*************************************************************************
    > File Name: caro.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Thu Apr  6 18:02:58 2023
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

#ifndef __CARO_H__
#define __CARO_H__

#include "rules.h"

#include<iostream>
using namespace std;

const int WIN_SHAPS[][7] = {
    {0, 1, 1, 1, 1, 1, 0},
    {2, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 2},
    {0, 2, 2, 2, 2, 2, 0},
    {1, 2, 2, 2, 2, 2, 0},
    {0, 2, 2, 2, 2, 2, 1}};

class Caro final : public rules
{
public:
    bool checkWin(Board *board) override;

private:
    bool findShap(Board *board, const pair<int, int> &p_drt);
};

#endif
