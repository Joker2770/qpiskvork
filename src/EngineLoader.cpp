/*************************************************************************
    > File Name: EngineLoader.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Jan  7 10:56:44 2023
 ************************************************************************/

/**
    qpiskvork is another gomoku manager for adapting to Windows and Linux systems.
    Copyright (C) 2023  Jintao Yang <yjt950840@outlook.com>

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

#include "EngineLoader.h"

#include <QDebug>

#include <iostream>
using namespace std;

EngineLoader::EngineLoader()
{
    this->mCmder = new Commander();
    this->mProcess = new QProcess();
    this->mProcess->setReadChannel(QProcess::StandardOutput);
    this->mProgram.clear();

    connect(this->mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadData()));
}
EngineLoader::~EngineLoader()
{
    this->mProcess->close();
    if (nullptr != this->mProcess)
    {
        delete this->mProcess;
        this->mProcess = nullptr;
    }
    if (nullptr != this->mCmder)
    {
        delete this->mCmder;
        this->mCmder = nullptr;
    }
}

bool EngineLoader::setProgram(const QString &sProgram)
{
    this->mProgram = sProgram;
    return !this->mProgram.isEmpty();
}

void EngineLoader::startProgram()
{
    if (!this->mProgram.isEmpty())
    {
        this->mProcess->setProgram(this->mProgram);
        this->mProcess->start();
    }
}

void EngineLoader::sendCommand(const char* s_cmd)
{
    this->mProcess->write(s_cmd, strlen(s_cmd));
    const char szEnd[2] = {0x0d, 0x0a};
    this->mProcess->write(szEnd, 2);
}

void EngineLoader::onReadData()
{
    qDebug() << this->mProcess->readAllStandardOutput();
}
