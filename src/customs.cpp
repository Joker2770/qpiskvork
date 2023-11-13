/*************************************************************************
    > File Name: customs.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年11月13日 星期一 19时19分14秒
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

#include "customs.h"

Customs::Customs(const QString &organization)
{
    this->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, organization);
}

Customs::~Customs()
{
    if (nullptr != this->settings)
    {
        delete this->settings;
        this->settings = nullptr;
    }
}

void Customs::setCfgValue(const QString &group, const QString &key, const QVariant &value)
{
    this->settings->beginGroup(group);
    this->settings->setValue(key, value);
    this->settings->endGroup();
}

void Customs::getCfgValue(const QString &group, const QString &key, QString &value)
{
    this->settings->beginGroup(group);
    value = this->settings->value(key).toString();
    this->settings->endGroup();
}
