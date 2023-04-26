/*************************************************************************
    > File Name: EngineLoader.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Jan  7 10:56:44 2023
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
    if (nullptr != this->mProcess)
    {
        if (this->mProcess->isOpen())
        {
            this->mProcess->close();
            this->mProcess->waitForFinished();
        }
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
        this->mProcess->waitForStarted();
    }
}

qint64 EngineLoader::sendCommand(const char* s_cmd, unsigned int iLen)
{
    qDebug() << s_cmd;
    qint64 i_w = 0;
    if (this->mProcess->isWritable())
    {
        i_w = this->mProcess->write(s_cmd, iLen);
        this->mProcess->waitForBytesWritten();
        if (i_w < 0)    return i_w;
        else
        {
            const char szEnd[2] = {0x0d, 0x0a};
            i_w += this->mProcess->write(szEnd, 2);
            this->mProcess->waitForBytesWritten();
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

    vector<string> v_strOut;
    if (sOutStr.find('\r') != string::npos && sOutStr.find('\n') != string::npos )
    {
        v_strOut = split(sOutStr, "\r\n");
    }
    else if (sOutStr.find('\r') != string::npos && sOutStr.find('\n') == string::npos )
    {
        v_strOut = split(sOutStr, "\r");
    }
    else if (sOutStr.find('\r') == string::npos && sOutStr.find('\n') != string::npos )
    {
        v_strOut = split(sOutStr, "\n");
    }
    vector<string>::iterator iter;
    for (iter = v_strOut.begin(); iter != v_strOut.end(); ++iter)
    {
        this->response_parse(*iter);
    }
}

vector<string> EngineLoader::split(const string &str, const string &pattern)
{
    string::size_type pos;
    vector<string> result;
    string strs = str + pattern;
    unsigned int size = strs.size();
    for (unsigned int i = 0; i < size; ++i) {
        pos = strs.find(pattern, i);
        if (pos < size)
        {
            string s = strs.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() -1;
        }
    }
    return result;
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

// string EngineLoader::format_string(string& res) {
//     //delete 0x0d,0x0a
//     res.erase(remove(res.begin(), res.end(), '\r'), res.end());
//     res.erase(remove(res.begin(), res.end(), '\n'), res.end());
//     return res;
// }

// const string EngineLoader::response_filter(const string &str_res)
// {
//     string s_get, s_tmp;
//     s_get.clear();
//     s_tmp.clear();
//     s_tmp = str_res;
//     if (s_tmp.empty())
//         return "";
//     if (s_tmp.find_first_of("OK") == 0)
//         s_get = this->format_string(s_tmp);
//     else if (s_tmp.find_first_of("ERROR ") == 0)
//         s_get = this->format_string(s_tmp);
//     else if (s_tmp.find_first_of("UNKNOWN ") == 0)
//         s_get = this->format_string(s_tmp);
//     else if (s_tmp.find_first_of("MESSAGE ") == 0)
//         s_get = this->format_string(s_tmp);
//     else if (s_tmp.find_first_of("DEBUG ") == 0)
//         s_get = this->format_string(s_tmp);
//     else if ((s_tmp.find_first_of(',') == 2 || s_tmp.find_first_of(',') == 1) && this->split(s_tmp, ',').size() == 2)
//         s_get = this->format_string(s_tmp);
//     return s_get;
// }

void EngineLoader::response_parse(const string &str)
{
    qDebug() << str.c_str();
    string s_tmp = str;
    if (s_tmp.empty())
        return;
    if (s_tmp.find_first_of("OK") == 0)
        emit responsed_ok();
    else if (s_tmp.find_first_of("ERROR ") == 0)
        emit responsed_error();
    else if (s_tmp.find_first_of("UNKNOWN ") == 0)
        emit responsed_unknown();
    else if (s_tmp.find_first_of("SWAP") == 0)
        emit responsed_swap();
    else if ((3 <= this->split(s_tmp, " ").size())
     && (2 == s_tmp.find_first_of(',') || 1 == s_tmp.find_first_of(','))
      && (this->split(s_tmp, " ").at(1).find_first_of(',') != string::npos)
      && (this->split(s_tmp, " ").at(2).find_first_of(',') != string::npos))
    {
        vector<string> s_pos = this->split(s_tmp, " ");
        int x[3] = {-1, -1, -1}, y[3] = {-1, -1, -1};
        for (size_t i = 0; i < 3; i++)
        {
            s_pos[i].erase(remove(s_pos[i].begin(), s_pos[i].end(), ' '), s_pos[i].end());
            vector<int> i_pos = this->split(s_pos[i], ',');
            if (i_pos.size() == 2)
            {
                if (i_pos.front() >= 0)
                    x[i] = i_pos.front();
                if (i_pos.back() >= 0)
                    y[i] = i_pos.back();
            }
            else
                break;
        }
        if (x[0] >= 0 && x[1] >= 0 && x[2] >= 0 && y[0] >= 0 && y[1] >= 0 && y[2] >= 0)
            emit responsed_3_pos(x[0], y[0], x[1], y[1], x[2], y[2]);
    }
    else if ((2 <= this->split(s_tmp, " ").size())
     && (2 == s_tmp.find_first_of(',') || 1 == s_tmp.find_first_of(','))
      && (this->split(s_tmp, " ").at(1).find_first_of(',') != string::npos))
    {
        vector<string> s_pos = this->split(s_tmp, " ");
        int x[2] = {-1, -1}, y[2] = {-1, -1};
        for (size_t i = 0; i < 2; i++)
        {
            s_pos[i].erase(remove(s_pos[i].begin(), s_pos[i].end(), ' '), s_pos[i].end());
            vector<int> i_pos = this->split(s_pos[i], ',');
            if (i_pos.size() == 2)
            {
                if (i_pos.front() >= 0)
                    x[i] = i_pos.front();
                if (i_pos.back() >= 0)
                    y[i] = i_pos.back();
            }
            else
                break;
        }
        if (x[0] >= 0 && x[1] >= 0 && y[0] >= 0 && y[1] >= 0)
            emit responsed_2_pos(x[0], y[0], x[1], y[1]);
    }
    else if ((s_tmp.find_first_of(',') == 2 || s_tmp.find_first_of(',') == 1)
     && (this->split(s_tmp, ',').size() == 2))
    {
        vector<int> vPos = this->split(s_tmp, ',');
        emit responsed_pos(vPos[0], vPos[1]);
    }
    else if (s_tmp.find("name") != string::npos)
    {
        vector<string> vInfo = this->split(s_tmp, ",");
        for (vector<string>::iterator iter = vInfo.begin(); iter != vInfo.end(); ++iter)
        {
            if (iter->find("name") != string::npos && iter->find("=") != string::npos)
            {
                vector<string> vKV = this->split(*iter, "=");
                string vName = vKV.at(1);
                size_t start = vName.find_first_of('\"') != string::npos ? (vName.find_first_of('\"') + 1) : 0;
                size_t end = vName.find_first_of('\"', start) != string::npos ? (vName.find_first_of('\"', start)) : vName.size();
                if ((end > start) && (vName.size() >= (end - start)))
                    emit(responsed_name(QString::fromStdString(vName.substr(start, end - start))));
            }
        }
    }
}
