/*************************************************************************
    > File Name: timer.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Oct  2 18:14:08 2022
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

#include "timer.h"

Timer::Timer()
{
    //Initialize the variables
    isRunning = false;
    elapsed = 0;
}

Timer::~Timer()
{
    isRunning = false;
    elapsed = 0;
}

void Timer::start()
{
    if (!isRunning)
    {
        startTime = chrono::system_clock::now();
        isRunning = true;
    }
}

void Timer::stop()
{
    pause();
    elapsed = 0;
}

void Timer::pause()
{
    if (isRunning)
    {
        stopTime = chrono::system_clock::now();
        elapsed += chrono::duration_cast<chrono::milliseconds>(stopTime - startTime).count();
        isRunning = false;
    }
}

void Timer::resume()
{
    if(!isRunning)
    {
        startTime = chrono::system_clock::now();
        isRunning = true;
    }
}

long long Timer::getElapsed()
{
    if (isRunning)
    {
        chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
        return elapsed + chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
    }
    else
    {
        return elapsed;
    }
}
