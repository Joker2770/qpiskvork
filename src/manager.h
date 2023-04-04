/*************************************************************************
    > File Name: manager.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 10:32:27 2022
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

#ifndef MANAGER_H
#define MANAGER_H

#include "observer.h"
#include "subject.h"
#include "player.h"
#include "EngineLoader.h"
#include "commander.h"

#include <QString>
#include <QProcess>

#include <vector>
#include<iostream>
using namespace std;

class Manager final : public Observer
{
public:
    Manager(Subject *pSubject);
    Manager(const Manager &);
    virtual ~Manager();
    void Update(int) override;
    bool AttachEngines();
    bool DetachEngines();
    bool startMatch(int i_size);
    bool infoMatch_p1(INFO_KEY key, const char* szValue);
    bool infoMatch_p2(INFO_KEY key, const char* szValue);
    void beginMatch();
    void turn_2_p1(int i_x, int i_y);
    void turn_2_p2(int i_x, int i_y);
    void sendBoard(vector<pair<pair<int, int>, int>> vRecord);
    void sendAbout();
    void endMatch();

public:
    Player *m_p1;
    Player *m_p2;

    Commander *m_cmd;

    EngineLoader *m_engine_1;
    EngineLoader *m_engine_2;

private:
    Subject *m_pSubject;
    int m_state;
};

#endif

