/*************************************************************************
    > File Name: board.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 18:05:28 2022
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

#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include <vector>
#include <utility>
#include <list>
#include<iostream>
using namespace std;

#define MAXBSIZE 25

typedef enum stone_color
{
    BLACK = 1,
    WHITE = 2
} STONECOLOR;

class Board final : public Subject
{
public:
    Board();
    ~Board();

    pair<int, int> coord2idx(const int iCoord);
    int idx2Coord(const pair<int, int> idx);
    bool isPosEmpty(const pair<int, int> idx);
    bool isPosOutOfBoard(const pair<int, int> idx);

    void Attach(Observer *pObserver);
    void Detach(Observer *pObserver);
    void Notify();
    void SetState(int state){
        m_iState = state;
    }

    int GetState(){
        return m_iState;
    }

    int BSize;

    //first - coord, second - color.
    vector<pair<int, int>> vRecord;
    int iMaxRecordSize;

private:
    std::list<Observer *> m_ObserverList;
    int m_iState;
};

#endif
