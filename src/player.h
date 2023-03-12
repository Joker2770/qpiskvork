/*************************************************************************
    > File Name: player.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Oct  2 14:02:21 2022
 ************************************************************************/

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

#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"

#include <QString>

#include<iostream>
using namespace std;

class Player
{
public:
    Player();
    ~Player();

    bool m_isComputer;
    bool m_isMyTurn;
    STONECOLOR m_currColor;
    QString m_sPath;

private:

};

#endif
