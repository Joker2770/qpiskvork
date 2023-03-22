/*************************************************************************
    > File Name: manager.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 10:32:33 2022
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

#include "manager.h"
#include<iostream>
using namespace std;

void Manager::Update(int i_state)
{
    this->m_state = i_state;
}

bool Manager::AttachEngines(const Player *p1, const Player *p2)
{
    if (p1->m_isComputer)
    {
        this->m_engine_1 = new EngineLoader();
        this->m_engine_1->setProgram(p1->m_sPath);
        this->m_engine_1->startProgram();
    }
    if (p2->m_isComputer)
    {
        this->m_engine_2 = new EngineLoader();
        this->m_engine_2->setProgram(p2->m_sPath);
        this->m_engine_2->startProgram();
    }

    bool isAttach = false;

    return isAttach;
}

bool Manager::DetachEngines()
{
    if (nullptr != this->m_engine_1)
    {
        delete this->m_engine_1;
        this->m_engine_1 = nullptr;
    }
    if (nullptr != this->m_engine_2)
    {
        delete this->m_engine_2;
        this->m_engine_2 = nullptr;
    }

    bool isDetach = false;

    return isDetach;
}

