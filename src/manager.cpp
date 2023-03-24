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


Manager::Manager(Subject *pSubject) : m_pSubject(pSubject)
{
    this->m_pSubject->Attach(this);
    this->m_engine_1 = nullptr;
    this->m_engine_2 = nullptr;
    this->m_cmd = new Commander();
}

Manager::~Manager()
{
    this->m_pSubject->Detach(this);

    if (nullptr != this->m_cmd)
    {
        delete this->m_cmd;
        this->m_cmd = nullptr;
    }
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
}

void Manager::Update(int i_state)
{
    this->m_state = i_state;
}

bool Manager::AttachEngines(const Player *p1, const Player *p2)
{
    bool isAttach_1 = false;
    bool isAttach_2 = false;

    if (p1->m_isComputer)
    {
        this->m_engine_1 = new EngineLoader();
        if (this->m_engine_1->setProgram(p1->m_sPath))
        {
            this->m_engine_1->startProgram();
            isAttach_1 = true;
        }
    }
    else
        isAttach_1 = true;

    if (p2->m_isComputer)
    {
        this->m_engine_2 = new EngineLoader();
        if (this->m_engine_2->setProgram(p2->m_sPath))
        {
            this->m_engine_2->startProgram();
            isAttach_2 = true;
        }
    }
    else
        isAttach_2 = true;

    return (isAttach_1 && isAttach_2);
}

bool Manager::DetachEngines()
{
    bool isDetach_1 = false;
    bool isDetach_2 = false;

    if (nullptr != this->m_engine_1)
    {
        delete this->m_engine_1;
        this->m_engine_1 = nullptr;
        isDetach_1 = true;
    }
    if (nullptr != this->m_engine_2)
    {
        delete this->m_engine_2;
        this->m_engine_2 = nullptr;
        isDetach_2 = true;
    }

    return (isDetach_1 || isDetach_2);
}

bool Manager::startMatch(const Player *p1)
{
    bool bStart = false;
    qint64 i_write = 0;
    if (nullptr != p1 && nullptr != this->m_engine_1)
    {
        if (p1->m_isComputer && p1->m_isMyTurn && !(p1->m_sPath.isEmpty()))
        {
            i_write = this->m_engine_1->sendCommand(this->m_cmd->begin_2_send().c_str());
        }
    }
    if (i_write > 0)
    {
        bStart = true;
    }

    return bStart;
}
