/*************************************************************************
    > File Name: commands.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: 2022年10月20日 星期四 09时09分01秒
 ************************************************************************/

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

#include<iostream>
using namespace std;

// Mandatory commands
const char[] START = "START";
const char[] TURN = "TURN";
const char[] BEGIN = "BEGIN";
const char[] BOARD = "BOARD";
const char[] INFO = "INFO";
const char[] END = "END";
const char[] ABOUT = "ABOUT";

// Optional commands
const char[] RECTSTART = "RECTSTART";
const char[] RESTART = "RESTART";
const char[] TAKEBACK = "TAKEBACK";
const char[] PLAY = "PLAY";