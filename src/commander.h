/*************************************************************************
    > File Name: commander.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Jan 17 10:13:19 2023
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

#ifndef __COMMANDER_H__
#define __COMMANDER_H__

#include "commands.h"
#include <iostream>
using namespace std;

class Commander
{
public:
    Commander();
    ~Commander();

    const string info_2_send(INFO_KEY info_key, const char* argv);
    const string begin_2_send(void);
    const string start_2_send(int i_size);
    const string move_2_send(int x, int y);
    const string end_2_send(void);
    const string about_2_send(void);

    const string response_parse(const string &sResp);
};

#endif
