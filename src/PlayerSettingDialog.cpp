/*************************************************************************
    > File Name: PlayerSettingDialog.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Feb  5 15:18:57 2023
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

#include "PlayerSettingDialog.h"
#include <iostream>
using namespace std;

PlayerSettingDialog::PlayerSettingDialog(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint|Qt::Drawer)
{
    this->gl = new QGridLayout(this);
    this->gl_p1 = new QGridLayout();
    this->gl_p2 = new QGridLayout();
    this->gb_p1 = new QGroupBox();
    this->gb_p2 = new QGroupBox();
    this->btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    this->gl->setSpacing(5);
    this->gl_p1->setSpacing(5);
    this->gl_p2->setSpacing(5);
    this->gb_p1->setTitle("Player1");
    this->gb_p2->setTitle("Player2");

    //gb_p1->setLayout(&gl_p1);
    //gb_p2->setLayout(&gl_p2);
    this->gl->addWidget(this->gb_p1, 0, 0);
    this->gl->addWidget(this->gb_p2, 1, 0);
    this->gl->addWidget(this->btn_box, 2, 0);

    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Player Setting");
    this->setModal(true);
    this->resize(450, 300);

    connect(btn_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

PlayerSettingDialog::~PlayerSettingDialog()
{
    if (nullptr != this->btn_box)
    {
        delete this->btn_box;
        this->btn_box = nullptr;
    }
    if (nullptr != this->gl)
    {
        delete this->gl;
        this->gl = nullptr;
    }
    if (nullptr != this->gl_p1)
    {
        delete this->gl_p1;
        this->gl_p1 = nullptr;
    }
    if (nullptr != this->gl_p2)
    {
        delete this->gl_p2;
        this->gl_p2 = nullptr;
    }
    if (nullptr != this->gb_p1)
    {
        delete this->gb_p1;
        this->gb_p1 = nullptr;
    }
    if (nullptr != this->gb_p2)
    {
        delete this->gb_p2;
        this->gb_p2 = nullptr;
    }
}


