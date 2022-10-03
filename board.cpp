/*************************************************************************
    > File Name: board.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 18:05:37 2022
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

#include "board.h"
#include<iostream>
using namespace std;

Board::Board()
{
    this->vRecord.clear();
}

Board::~Board()
{

}

pair<int, int> Board::coord2idx(int i_coord)
{
    pair<int, int> idx;
    idx.first = i_coord % this->BSize;
    idx.second = i_coord / this->BSize;
    return idx;
}

int Board::idx2Coord(const pair<int, int> idx)
{
    int i_coord = 0;
    i_coord = idx.second * this->BSize + idx.first;
    return i_coord;
}

void Board::Attach(Observer *pObserver){
    m_ObserverList.push_back(pObserver);
}

void Board::Detach(Observer *pObserver){
    m_ObserverList.remove(pObserver);
}

void Board::Notify(){
    std::list<Observer *>::iterator it = m_ObserverList.begin();
    while (it != m_ObserverList.end()){
        (*it)->Update(m_iState);
        ++it;
    }
}
