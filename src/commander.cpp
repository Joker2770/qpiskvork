/*************************************************************************
    > File Name: commander.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Jan 17 10:13:25 2023
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

#include "commander.h"
#include "commands.h"

#include <string>
#include <iostream>
using namespace std;

Commander::Commander()
{

}

Commander::~Commander()
{

}

const string Commander::begin_2_send()
{
    string sBegin = BEGIN;
    sBegin.append("\r\n");

    return sBegin;
}

const string Commander::start_2_send(int i_size)
{
    string sStart = START;
    sStart.append("\r\n");

    return sStart;
}

const string Commander::move_2_send(int x, int y)
{
    string turn_move = TURN;
    turn_move.append(to_string(x));
    turn_move.append(",");
    turn_move.append(to_string(y));
    turn_move.append("\r\n");

    return turn_move;
}

const string Commander::end_2_send()
{
    string sEnd = END;
    sEnd.append("\r\n");

    return sEnd;
}

const string Commander::about_2_send()
{
    string sAbout = ABOUT;
    sAbout.append("\r\n");

    return sAbout;
}
