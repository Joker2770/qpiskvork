/*************************************************************************
    > File Name: customs.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年11月13日 星期一 19时18分55秒
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

#ifndef __CUSTOMS_H__
#define __CUSTOMS_H__

#include <QSettings>

#include <iostream>
using namespace std;

class Customs
{
public:
    Customs(const QString &organization);
    ~Customs();

    void setCfgValue(const QString &group, const QString &key, const QVariant &value);
    void getCfgValue(const QString &group, const QString &key, QString &value);

private:
    QSettings *settings;
};

#endif
