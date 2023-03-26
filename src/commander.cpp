/*************************************************************************
    > File Name: commander.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Tue Jan 17 10:13:25 2023
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

#include "commander.h"
#include "commands.h"

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

Commander::Commander()
{

}

Commander::~Commander()
{

}

vector<int> Commander::split(const string &str, char sep)
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

string Commander::format_string(string& res) {
    //delete 0x0d,0x0a
    res.erase(remove(res.begin(), res.end(), '\r'), res.end());
    res.erase(remove(res.begin(), res.end(), '\n'), res.end());
    return res;
}

const string Commander::info_2_send(INFO_KEY info_key, const char* argv)
{
    string sInfo;
    sInfo.clear();
    sInfo.append(INFO);
    sInfo.append(" ");
    switch (info_key) {
    case TIMEOUT_TURN:
        sInfo.append(timeout_turn);
        break;
    case TIMEOUT_MATCH:
        sInfo.append(timeout_match);
        break;
    case MAX_MEMORY:
        sInfo.append(max_memory);
        break;
    case TIME_LEFT:
        sInfo.append(time_left);
        break;
    case GAME_TYPE:
        sInfo.append(game_type);
        break;
    case RULE:
        sInfo.append(rule);
        break;
    case EVALUATE:
        sInfo.append(evaluate);
        break;
    case FOLDER:
        sInfo.append(folder);
        break;
    default:
        sInfo.append(max_memory);
        break;
    }
    sInfo.append(" ");
    sInfo.append(argv);
    //sInfo.append("\r\n");

    return sInfo;
}

const string Commander::begin_2_send()
{
    string sBegin = BEGIN;
    //sBegin.append("\r\n");

    return sBegin;
}

const string Commander::start_2_send(int i_size)
{
    string sStart = START;
    sStart.append(" ");
    sStart.append(to_string(i_size));
    //sStart.append("\r\n");

    return sStart;
}

const string Commander::move_2_send(int x, int y)
{
    string turn_move = TURN;
    turn_move.append(" ");
    turn_move.append(to_string(x));
    turn_move.append(",");
    turn_move.append(to_string(y));
    //turn_move.append("\r\n");

    return turn_move;
}

const string Commander::end_2_send()
{
    string sEnd = END;
    //sEnd.append("\r\n");

    return sEnd;
}

const string Commander::about_2_send()
{
    string sAbout = ABOUT;
    //sAbout.append("\r\n");

    return sAbout;
}

const string Commander::response_filter(const string &str_res)
{
    string s_get, s_tmp;
    s_tmp = str_res;
    if (!(s_tmp.empty()) && s_tmp.find_first_of("OK") == 0)
        s_get = this->format_string(s_tmp);
    else if (!(s_tmp.empty()) && s_tmp.find_first_of("ERROR ") == 0)
        s_get = this->format_string(s_tmp);
    else if (!(s_tmp.empty()) && s_tmp.find_first_of("UNKNOWN ") == 0)
        s_get = this->format_string(s_tmp);
    else if (!(s_tmp.empty()) && s_tmp.find_first_of("MESSAGE ") == 0)
        s_get = this->format_string(s_tmp);
    else if (!(s_tmp.empty()) && s_tmp.find_first_of("DEBUG ") == 0)
        s_get = this->format_string(s_tmp);
    else if (!(s_tmp.empty()) && s_tmp.find_first_of(',') < 3 && s_tmp.find_first_of(',') > 0 && this->split(s_tmp, ',').size() == 2)
        s_get = this->format_string(s_tmp);
    return s_get;
}

void Commander::response_parse(const string &str)
{

}
