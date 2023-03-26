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

#include <sstream>
#include <iostream>
using namespace std;

EngineLoader::EngineLoader()
{
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

qint64 EngineLoader::sendCommand(const char* s_cmd)
{
    qint64 i_w = 0;
    if (this->mProcess->isWritable())
    {
        i_w = this->mProcess->write(s_cmd, strlen(s_cmd));
        if (i_w < 0)    return i_w;
        else
        {
            const char szEnd[2] = {0x0d, 0x0a};
            i_w += this->mProcess->write(szEnd, 2);
        }
    }
    return i_w;
}

void EngineLoader::onReadData()
{
    string sOutStr;
    sOutStr.clear();
    QByteArray baOut = nullptr;
    baOut = this->mProcess->readAllStandardOutput();
    qDebug() << baOut;
    sOutStr = baOut.toStdString();

    vector<string> v_strOut = split(sOutStr, "\r\n");
    vector<string>::iterator iter;
    for (iter = v_strOut.begin(); iter != v_strOut.end(); ++iter)
    {
        this->response_parse(this->response_filter(*iter));
    }
}

vector<string> EngineLoader::split(const string &str, const string &pattern)
{
    vector<string> res;
    if(str == "")
        return res;
    string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }

    return res;
}

vector<int> EngineLoader::split(const string &str, char sep)
{
    vector<int> tokens;

    int i;
    stringstream ss(str);
    while (ss >> i) {
        tokens.push_back(i);
        if (ss.peek() == sep) {
            ss.ignore();
        }
    }

    return tokens;
}

string EngineLoader::format_string(string& res) {
    //delete 0x0d,0x0a
    res.erase(remove(res.begin(), res.end(), '\r'), res.end());
    res.erase(remove(res.begin(), res.end(), '\n'), res.end());
    return res;
}

const string EngineLoader::response_filter(const string &str_res)
{
    string s_get, s_tmp;
    s_get.clear();
    s_tmp.clear();
    s_tmp = str_res;
    if (s_tmp.empty())
        return "";
    if (s_tmp.find_first_of("OK") == 0)
        s_get = this->format_string(s_tmp);
    else if (s_tmp.find_first_of("ERROR ") == 0)
        s_get = this->format_string(s_tmp);
    else if (s_tmp.find_first_of("UNKNOWN ") == 0)
        s_get = this->format_string(s_tmp);
    else if (s_tmp.find_first_of("MESSAGE ") == 0)
        s_get = this->format_string(s_tmp);
    else if (s_tmp.find_first_of("DEBUG ") == 0)
        s_get = this->format_string(s_tmp);
    else if ((s_tmp.find_first_of(',') == 2 || s_tmp.find_first_of(',') == 1) && this->split(s_tmp, ',').size() == 2)
        s_get = this->format_string(s_tmp);
    return s_get;
}

void EngineLoader::response_parse(const string &str)
{
    string s_tmp = str;
    if (s_tmp.empty())
        return;
    if (s_tmp.find_first_of("OK") == 0)
        emit responsed_ok();
    else if (s_tmp.find_first_of("ERROR ") == 0)
        emit responsed_error();
    else if (s_tmp.find_first_of("UNKNOWN ") == 0)
        emit responsed_unknown();
    else if ((s_tmp.find_first_of(',') == 2 || s_tmp.find_first_of(',') == 1) && this->split(s_tmp, ',').size() == 2)
    {
        vector<int> vPos = this->split(s_tmp, ',');
        emit responsed_pos(vPos[0], vPos[1]);
    }
}
