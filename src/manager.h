/*************************************************************************
    > File Name: manager.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 10:32:27 2022
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

#ifndef MANAGER_H
#define MANAGER_H

#include "observer.h"
#include "subject.h"
#include "player.h"
#include "EngineLoader.h"

#include <QString>
#include <QProcess>

#include<iostream>
using namespace std;

class Manager final : public Observer
{
public:
    Manager(Subject *pSubject) : m_pSubject(pSubject)
    {
        this->m_pSubject->Attach(this);
    }
    ~Manager()
    {
        this->m_pSubject->Detach(this);
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
    void Update(int) override;
    bool AttachEngines(const Player *p1, const Player *p2);
    bool DetachEngines();
private:
    Subject *m_pSubject;
    int m_state;

    EngineLoader *m_engine_1;
    EngineLoader *m_engine_2;
};

#endif

