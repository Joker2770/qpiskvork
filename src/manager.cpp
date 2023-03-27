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
#include "board.h"

#include <QDebug>

#include<iostream>
using namespace std;


Manager::Manager(Subject *pSubject) : m_pSubject(pSubject)
{
    this->m_pSubject->Attach(this);
    this->m_engine_1 = nullptr;
    this->m_engine_2 = nullptr;
    this->m_cmd = nullptr;

    this->m_p1 = new Player();
    this->m_p2 = new Player();
}

Manager::~Manager()
{
    this->m_pSubject->Detach(this);

    if (nullptr != this->m_p1)
    {
        delete this->m_p1;
        this->m_p1 = nullptr;
    }
    if (nullptr != this->m_p2)
    {
        delete this->m_p2;
        this->m_p2 = nullptr;
    }
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
    if (BOARDSTATUS::BOARDEMPTY == this->m_state)
    {
        if (this->m_p1->m_color == STONECOLOR::BLACK)
        {
            this->m_p1->m_isMyTurn = true;
            this->m_p2->m_isMyTurn = false;
            qDebug() << "P1 turn";
        }
        else
        {
            this->m_p1->m_isMyTurn = false;
            this->m_p2->m_isMyTurn = true;
            qDebug() << "P2 turn";
        }
    }
    else if (BOARDSTATUS::BLACKNEXT == this->m_state)
    {
        if (this->m_p1->m_color == STONECOLOR::BLACK)
        {
            this->m_p1->m_isMyTurn = true;
            this->m_p2->m_isMyTurn = false;
            qDebug() << "P1 turn";
        }
        else
        {
            this->m_p1->m_isMyTurn = false;
            this->m_p2->m_isMyTurn = true;
            qDebug() << "P2 turn";
        }
    }
    else if (BOARDSTATUS::WHITENEXT == this->m_state)
    {
        if (this->m_p2->m_color == STONECOLOR::WHITE)
        {
            this->m_p1->m_isMyTurn = false;
            this->m_p2->m_isMyTurn = true;
            qDebug() << "P2 turn";
        }
        else
        {
            this->m_p1->m_isMyTurn = true;
            this->m_p2->m_isMyTurn = false;
            qDebug() << "P1 turn";
        }
    }
}

bool Manager::AttachEngines()
{
    bool isAttach_1 = false;
    bool isAttach_2 = false;

    this->m_cmd = new Commander();

    if (this->m_p1->m_isComputer)
    {
        this->m_engine_1 = new EngineLoader();
        if (this->m_engine_1->setProgram(this->m_p1->m_sPath))
        {
            this->m_engine_1->startProgram();
            isAttach_1 = true;
        }
    }
    else
        isAttach_1 = true;

    if (this->m_p2->m_isComputer)
    {
        this->m_engine_2 = new EngineLoader();
        if (this->m_engine_2->setProgram(this->m_p2->m_sPath))
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

    if (nullptr != this->m_cmd)
    {
        delete this->m_cmd;
        this->m_cmd = nullptr;
    }

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

bool Manager::startMatch(int i_size)
{
    bool bStart_1 = false;
    bool bStart_2 = false;

    if (nullptr != this->m_p1 && nullptr != this->m_engine_1)
    {
        if (this->m_p1->m_isComputer && !(this->m_p1->m_sPath.isEmpty()))
        {
            qint64 i_write_1 = 0;
            i_write_1 = this->m_engine_1->sendCommand(this->m_cmd->start_2_send(i_size).c_str());
            qDebug() << "i_write_1: " << i_write_1;
            if (i_write_1 > 0)
            {
                bStart_1 = true;
            }
        }
    }
    else
    {
        bStart_1 = true;
    }

    if (nullptr != this->m_p2 && nullptr != this->m_engine_2)
    {
        if (this->m_p2->m_isComputer && !(this->m_p2->m_sPath.isEmpty()))
        {
            qint64 i_write_2 = 0;
            i_write_2 = this->m_engine_2->sendCommand(this->m_cmd->start_2_send(i_size).c_str());
            qDebug() << "i_write_2: " << i_write_2;
            if (i_write_2 > 0)
            {
                bStart_2 = true;
            }
        }
    }
    else
    {
        bStart_2 = true;
    }

    return (bStart_1 && bStart_2);
}

void Manager::beginMatch()
{
    qint64 i_write = 0;

    if (this->m_p1->m_isComputer && nullptr != this->m_engine_1)
    {
        i_write = this->m_engine_1->sendCommand(this->m_cmd->begin_2_send().c_str());
        if (i_write <= 0) qDebug() << "Failed to send begin to engine_1!";
    }
}

bool Manager::infoMatch_p1(INFO_KEY key, const char* szValue)
{
    bool bSend = false;
    qint64 i_write = 0;

    if (this->m_p1->m_isComputer && nullptr != this->m_engine_1)
    {
        i_write = this->m_engine_1->sendCommand(this->m_cmd->info_2_send(key, szValue).c_str());
        if (i_write <= 0) qDebug() << "Failed to send info to engine_1!";
        else bSend = true;
    }

    return bSend;
}

bool Manager::infoMatch_p2(INFO_KEY key, const char* szValue)
{
    bool bSend = false;
    qint64 i_write = 0;

    if (this->m_p2->m_isComputer && nullptr != this->m_engine_2)
    {
        i_write = this->m_engine_2->sendCommand(this->m_cmd->info_2_send(key, szValue).c_str());
        if (i_write <= 0) qDebug() << "Failed to send info to engine_1!";
        else bSend = true;
    }

    return bSend;
}

void Manager::turn_2_p1(int iX, int iY)
{
    qint64 i_write = 0;

    if (this->m_p1->m_isMyTurn && this->m_p1->m_isComputer && nullptr != this->m_engine_1)
    {
        i_write = this->m_engine_1->sendCommand(this->m_cmd->move_2_send(iX, iY).c_str());
        if (i_write <= 0) qDebug() << "Failed to send turn pos to engine_1!";
    }
}

void Manager::turn_2_p2(int iX, int iY)
{
    qint64 i_write = 0;

    if (this->m_p2->m_isMyTurn && this->m_p2->m_isComputer && nullptr != this->m_engine_2)
    {
        i_write = this->m_engine_2->sendCommand(this->m_cmd->move_2_send(iX, iY).c_str());
        if (i_write <= 0) qDebug() << "Failed to send turn pos to engine_2!";
    }
}

void Manager::sendBoard(vector<pair<pair<int, int>, int>> vRecord)
{
    qint64 i_write = 0;

    if (nullptr != this->m_engine_1)
    {
        i_write = this->m_engine_1->sendCommand(this->m_cmd->board_2_send(vRecord, this->m_p1->m_color).c_str());
        if (i_write <= 0) qDebug() << "Failed to send end to engine_1!";
    }

    if (nullptr != this->m_engine_2)
    {
        i_write = this->m_engine_2->sendCommand(this->m_cmd->board_2_send(vRecord, this->m_p2->m_color).c_str());
        if (i_write <= 0) qDebug() << "Failed to send end to engine_2!";
    }
}

void Manager::endMatch()
{
    qint64 i_write = 0;

    if (nullptr != this->m_engine_1)
    {
        i_write = this->m_engine_1->sendCommand(this->m_cmd->end_2_send().c_str());
        if (i_write <= 0) qDebug() << "Failed to send end to engine_1!";
    }

    if (nullptr != this->m_engine_2)
    {
        i_write = this->m_engine_2->sendCommand(this->m_cmd->end_2_send().c_str());
        if (i_write <= 0) qDebug() << "Failed to send end to engine_2!";
    }
}
