/*************************************************************************
    > File Name: EngineLoader.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Jan  7 10:56:36 2023
 ************************************************************************/

/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
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

#ifndef __ENGINELOADER_H__
#define __ENGINELOADER_H__

#include <QProcess>
#include <QObject>

#include <iostream>
using namespace std;

class EngineLoader : public QObject
{
    Q_OBJECT
public:
    EngineLoader();
    EngineLoader(const EngineLoader &);
    virtual ~EngineLoader();

    bool setProgram(const QString &sProgram);
    void startProgram();
    qint64 sendCommand(const char* sCmd, unsigned int iLen);

    vector<string> split(const string &str, const string &pattern);
    vector<int> split(const string &str, char sep);
    string format_string(string &str);
    const string response_filter(const string &sResp);
    void response_parse(const string &sResp);

signals:
    void responsed_ok();
    void responsed_error();
    void responsed_unknown();
    void responsed_name(const QString &name);
    void responsed_swap();
    void responsed_pos(int x, int y);
    void responsed_2_pos(int x_1, int y_1, int x_2, int y_2);
    void responsed_3_pos(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);

private slots:
    void onReadData();

private:
    QString mProgram;
    QProcess *mProcess;
};

#endif
