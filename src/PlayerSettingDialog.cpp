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
    this->p1_hbl_1 = new QHBoxLayout();
    this->p1_hbl_2 = new QHBoxLayout();
    this->p2_hbl_1 = new QHBoxLayout();
    this->p2_hbl_2 = new QHBoxLayout();
    this->p1_rb_1 = new QRadioButton();
    this->p1_rb_2 = new QRadioButton();
    this->p2_rb_1 = new QRadioButton();
    this->p2_rb_2 = new QRadioButton();
    this->le_p1 = new QLineEdit();
    this->le_p2 = new QLineEdit();
    this->btn_p1 = new QPushButton();
    this->btn_p2 = new QPushButton();

    this->gl->setSpacing(5);
    this->gl_p1->setSpacing(5);
    this->gl_p2->setSpacing(5);
    this->p1_hbl_1->setSpacing(5);
    this->p1_hbl_2->setSpacing(5);
    this->p2_hbl_1->setSpacing(5);
    this->p2_hbl_2->setSpacing(5);
    this->gb_p1->setTitle("Player1");
    this->gb_p2->setTitle("Player2");
    this->p1_rb_1->setText("Human");
    this->p1_rb_2->setText("AI");
    this->p2_rb_1->setText("Human");
    this->p2_rb_2->setText("AI");
    this->p1_rb_1->setChecked(true);
    this->p2_rb_1->setChecked(true);
    this->btn_p1->setText("open");
    this->btn_p2->setText("open");
    this->p1_hbl_1->addWidget(p1_rb_1);
    this->p1_hbl_1->addWidget(p1_rb_2);
    this->p1_hbl_2->addWidget(le_p1);
    this->p1_hbl_2->addWidget(btn_p1);
    this->p2_hbl_1->addWidget(p2_rb_1);
    this->p2_hbl_1->addWidget(p2_rb_2);
    this->p2_hbl_2->addWidget(le_p2);
    this->p2_hbl_2->addWidget(btn_p2);
    this->gl_p1->addLayout(this->p1_hbl_1, 0, 0);
    this->gl_p1->addLayout(this->p1_hbl_2, 1, 0);
    this->gl_p2->addLayout(this->p2_hbl_1, 0, 0);
    this->gl_p2->addLayout(this->p2_hbl_2, 1, 0);
    this->gb_p1->setLayout(gl_p1);
    this->gb_p2->setLayout(gl_p2);
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
    if (nullptr != this->btn_p1)
    {
        delete this->btn_p1;
        this->btn_p1 = nullptr;
    }
    if (nullptr != this->btn_p2)
    {
        delete this->btn_p2;
        this->btn_p2 = nullptr;
    }
    if (nullptr != this->le_p1)
    {
        delete this->le_p1;
        this->le_p1 = nullptr;
    }
    if (nullptr != this->le_p2)
    {
        delete this->le_p2;
        this->le_p2 = nullptr;
    }
    if (nullptr != this->p1_rb_1)
    {
        delete this->p1_rb_1;
        this->p1_rb_1 = nullptr;
    }
    if (nullptr != this->p1_rb_2)
    {
        delete this->p1_rb_2;
        this->p1_rb_2 = nullptr;
    }
    if (nullptr != this->p2_rb_1)
    {
        delete this->p2_rb_1;
        this->p2_rb_1 = nullptr;
    }
    if (nullptr != this->p2_rb_2)
    {
        delete this->p2_rb_2;
        this->p2_rb_2 = nullptr;
    }
    if (nullptr != this->p1_hbl_1)
    {
        delete this->p1_hbl_1;
        this->p1_hbl_1 = nullptr;
    }
    if (nullptr != this->p1_hbl_2)
    {
        delete this->p1_hbl_2;
        this->p1_hbl_2 = nullptr;
    }
    if (nullptr != this->p2_hbl_1)
    {
        delete this->p2_hbl_1;
        this->p2_hbl_1 = nullptr;
    }
    if (nullptr != this->p2_hbl_2)
    {
        delete this->p2_hbl_2;
        this->p2_hbl_2 = nullptr;
    }
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


