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
#include <iostream>
using namespace std;

Board::Board() : BSize(15), iMaxRecordSize(15 * 15)
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

bool Board::isPosEmpty(const pair<int, int> idx)
{
    if (this->vRecord.empty())
        return true;
    int i_coord = 0;
    i_coord = idx.second * this->BSize + idx.first;
    for (unsigned int i = 0; i < this->vRecord.size(); ++i)
    {
        if (this->vRecord.at(i).first == i_coord)
            return false;
    }
    return true;
}

bool Board::isPosOutOfBoard(const pair<int, int> idx)
{
    int i_tmpCoord = this->idx2Coord(idx);
    if (0 <= i_tmpCoord && i_tmpCoord < this->iMaxRecordSize)
        return false;
    else
        return true;
}

bool Board::setBSize(int i_size)
{
    this->clearBoard();
    this->BSize = i_size;
    this->iMaxRecordSize = this->BSize*this->BSize;
    return true;
}

int Board::getBSize()
{
    return this->BSize;
}

int Board::getMaxRecordSize()
{
    return this->BSize * this->BSize;
}

vector<pair<int, int>> &Board::getVRecord()
{
    return this->vRecord;
}

bool Board::placeStone(const pair<int, int> idx, STONECOLOR color)
{
    if (this->isPosEmpty(idx) && !this->isPosOutOfBoard(idx))
    {
        pair<int, int> p_coord(idx2Coord(idx), color);
        this->vRecord.push_back(p_coord);
        return true;
    }
    else
        return false;
}

bool Board::takeBackStone()
{
    int i_size_1 = this->vRecord.size();
    if (!this->vRecord.empty())
        this->vRecord.pop_back();
    int i_size_2 = this->vRecord.size();
    if (i_size_1 > i_size_2)
        return true;
    else
        return false;
}

void Board::clearBoard()
{
    vector<pair<int, int>>::iterator iter = this->vRecord.begin();
    for (; iter != this->vRecord.end();)
    {
        iter = this->vRecord.erase(iter);
    }
}

void Board::Attach(Observer *pObserver)
{
    m_ObserverList.push_back(pObserver);
}

void Board::Detach(Observer *pObserver)
{
    m_ObserverList.remove(pObserver);
}

void Board::Notify()
{
    std::list<Observer *>::iterator it = m_ObserverList.begin();
    while (it != m_ObserverList.end())
    {
        (*it)->Update(m_iState);
        ++it;
    }
}
