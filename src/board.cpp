/*************************************************************************
    > File Name: board.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Fri Sep 30 18:05:37 2022
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

#include "board.h"
#include <iostream>
using namespace std;

Board::Board() : i_width(15), i_height(15), iMaxRecordSize(15 * 15)
{
    this->vRecord.clear();
    this->m_iState = BOARDSTATUS::BOARDEMPTY;
}

Board::~Board()
{
}

pair<int, int> Board::coord2idx(int i_coord)
{
    pair<int, int> idx;
    idx.first = i_coord % this->i_width;
    idx.second = i_coord / this->i_width;
    return idx;
}

int Board::idx2Coord(const pair<int, int> idx)
{
    int i_coord = 0;
    i_coord = idx.second * this->i_width + idx.first;
    return i_coord;
}

bool Board::isPosEmpty(const pair<int, int> idx)
{
    if (this->vRecord.empty())
        return true;
    int i_coord = idx2Coord(idx);
    for (unsigned int i = 0; i < this->vRecord.size(); ++i)
    {
        if (this->vRecord.at(i).first == i_coord)
            return false;
    }
    return true;
}

bool Board::isPosOutOfBoard(const pair<int, int> idx)
{
    if (0 <= idx.first && idx.first < this->i_width && 0 <= idx.second && idx.second < this->i_height)
        return false;
    else
        return true;
}

bool Board::setBSize(const pair<int, int> wh)
{
    if (8 <= wh.first && wh.first <= MAXBSIZE && 8 <= wh.second && wh.second <= MAXBSIZE)
    {
        this->clearBoard();
        this->i_width = wh.first;
        this->i_height = wh.second;
        this->iMaxRecordSize = this->i_width * this->i_height;
        return true;
    }
    else
        return false;
}

pair<int, int> Board::getBSize()
{
    pair<int, int> pTmp(this->i_width, this->i_height);
    return pTmp;
}

int Board::getMaxRecordSize()
{
    return this->i_width * this->i_height;
}

vector<pair<int, int>> &Board::getVRecord()
{
    return this->vRecord;
}

int Board::getIdxStoneColor(const pair<int, int> idx)
{
    int i_color = -1;
    if (!this->isPosOutOfBoard(idx))
    {
        i_color = 0;
        vector<pair<int, int>>::iterator iter;
        for (iter = this->vRecord.begin(); iter != this->vRecord.end(); ++iter)
        {
            // found
            if (this->idx2Coord(idx) == iter->first)
            {
                i_color = iter->second;
                break;
            }
        }
    }

    return i_color;
}

bool Board::placeStone(const pair<int, int> idx, STONECOLOR color)
{
    if (this->isPosEmpty(idx) && !this->isPosOutOfBoard(idx))
    {
        pair<int, int> p_coord(idx2Coord(idx), color);
        this->vRecord.push_back(p_coord);

        if (this->vRecord.size() >= this->iMaxRecordSize)
            this->m_iState = BOARDSTATUS::BOARDFULL;
        else if (color == STONECOLOR::WHITE)
            this->m_iState = BOARDSTATUS::BLACKNEXT;
        else if (color == STONECOLOR::BLACK)
            this->m_iState = BOARDSTATUS::WHITENEXT;

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
    {
        if (0 == this->vRecord.size())
            this->m_iState = BOARDSTATUS::BOARDEMPTY;
        else
        {
            if (this->vRecord.back().second == STONECOLOR::WHITE)
                this->m_iState = BOARDSTATUS::BLACKNEXT;
            else if (this->vRecord.back().second == STONECOLOR::BLACK)
                this->m_iState = BOARDSTATUS::WHITENEXT;
        }
        return true;
    }
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
    this->m_iState = BOARDSTATUS::BOARDEMPTY;
}

void Board::Attach(Observer *pObserver)
{
    m_vObserver.push_back(pObserver);
}

void Board::Detach(Observer *pObserver)
{
    for (auto it = m_vObserver.begin(); it != m_vObserver.end(); ++it)
    {
        if (*it == pObserver)
        {
            m_vObserver.erase(it);
            break;
        }
    }
}

void Board::Notify()
{
    // std::list<Observer *>::iterator it = m_ObserverList.begin();
    // while (it != m_vObserver.end())
    // {
    //     (*it)->Update(m_iState);
    //     ++it;
    // }
    for (auto observer : m_vObserver)
    {
        observer->Update(m_iState);
    }
}
