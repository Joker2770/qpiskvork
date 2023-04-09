/*************************************************************************
    > File Name: S2BResDialog.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Apr  9 09:55:26 2023
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

#include "S2BResDialog.h"

#include<iostream>
using namespace std;

// S2BResDialog::S2BResDialog(QWidget *parent) :
//     QDialog(parent,Qt::WindowCloseButtonHint|Qt::Drawer)
// {
// }

S2BResDialog::S2BResDialog(int i_mode, QWidget *parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    this->gl = new QGridLayout(this);
    this->vbl_l = new QVBoxLayout();
    this->vbl_r = new QVBoxLayout();
    this->hbl = new QHBoxLayout();
    this->rb_1 = new QRadioButton();
    this->rb_2 = new QRadioButton();
    this->rb_3 = new QRadioButton();
    this->lb_1 = new QLabel();
    this->lb_2 = new QLabel();
    this->lb_3 = new QLabel();
    this->lb_4 = new QLabel();
    this->lb_5 = new QLabel();
    this->lb_6 = new QLabel();
    this->lb_7 = new QLabel();
    this->le_1 = new QLineEdit();
    this->le_2 = new QLineEdit();
    this->le_3 = new QLineEdit();
    this->le_4 = new QLineEdit();
    this->le_5 = new QLineEdit();
    this->le_6 = new QLineEdit();
    this->bg = new QButtonGroup();
    this->dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    this->m_RegEV = new QRegExpValidator(QRegExp("[0-9]+$"));

    this->m_option = 0;
    this->m_bSwap = false;

    if (1 == i_mode)
    {
        this->lb_3->setText(",");
        this->lb_4->setText(",");
        this->lb_5->setText(",");
        this->lb_6->setText(" ");
        this->lb_7->setText(" ");
        this->le_1->setMaxLength(2);
        this->le_2->setMaxLength(2);
        this->le_3->setMaxLength(2);
        this->le_4->setMaxLength(2);
        this->le_5->setMaxLength(2);
        this->le_6->setMaxLength(2);
        this->le_1->setValidator(this->m_RegEV);
        this->le_2->setValidator(this->m_RegEV);
        this->le_3->setValidator(this->m_RegEV);
        this->le_4->setValidator(this->m_RegEV);
        this->le_5->setValidator(this->m_RegEV);
        this->le_6->setValidator(this->m_RegEV);
        this->hbl->addWidget(le_1);
        this->hbl->addWidget(lb_3);
        this->hbl->addWidget(le_2);
        this->hbl->addWidget(lb_6);
        this->hbl->addWidget(le_3);
        this->hbl->addWidget(lb_4);
        this->hbl->addWidget(le_4);
        this->hbl->addWidget(lb_7);
        this->hbl->addWidget(le_5);
        this->hbl->addWidget(lb_5);
        this->hbl->addWidget(le_6);
        this->gl->addLayout(hbl, 0, 0);
        this->gl->addWidget(dbb, 1, 0);

        this->setLayout(gl);
        this->setWindowTitle("Input 3 Move");
        this->resize(200, 100);
        this->setModal(true);

        this->m_mode = 1;
    }
    else if (2 == i_mode)
    {
        this->lb_1->setText("swap");
        this->lb_2->setText("go on");
        this->lb_3->setText(",");
        this->lb_4->setText(",");
        this->lb_5->setText(",");
        this->lb_6->setText(" ");
        this->lb_7->setText(" ");
        this->le_1->setMaxLength(2);
        this->le_2->setMaxLength(2);
        this->le_3->setMaxLength(2);
        this->le_4->setMaxLength(2);
        this->le_1->setValidator(this->m_RegEV);
        this->le_2->setValidator(this->m_RegEV);
        this->le_3->setValidator(this->m_RegEV);
        this->le_4->setValidator(this->m_RegEV);
        this->rb_1->setText("1");
        this->rb_2->setText("2");
        this->rb_3->setText("3");

        this->bg->setExclusive(true);
        this->bg->addButton(this->rb_1);
        this->bg->addButton(this->rb_2);
        this->bg->addButton(this->rb_3);
        this->rb_2->setChecked(true);
        this->m_option = 2;

        this->hbl->addWidget(this->le_1);
        this->hbl->addWidget(this->lb_3);
        this->hbl->addWidget(this->le_2);
        this->hbl->addWidget(this->lb_6);
        this->hbl->addWidget(this->le_3);
        this->hbl->addWidget(this->lb_4);
        this->hbl->addWidget(this->le_4);
        this->vbl_l->addWidget(this->rb_1);
        this->vbl_l->addWidget(this->rb_2);
        this->vbl_l->addWidget(this->rb_3);
        this->vbl_r->addWidget(this->lb_1);
        this->vbl_r->addWidget(this->lb_2);
        this->vbl_r->addLayout(this->hbl);
        this->gl->addLayout(this->vbl_l, 0, 0);
        this->gl->addLayout(this->vbl_r, 0, 1);
        this->gl->addWidget(this->dbb, 1, 1);

        this->setLayout(this->gl);
        this->setWindowTitle("Case 2");
        this->resize(250, 150);
        this->setModal(true);

        this->m_mode = 2;
    }
    else if (3 == i_mode)
    {
        this->lb_1->setText("swap");
        this->lb_2->setText("go on");
        this->rb_1->setText("1");
        this->rb_2->setText("2");

        this->bg->setExclusive(true);
        this->bg->addButton(this->rb_1);
        this->bg->addButton(this->rb_2);
        this->rb_2->setChecked(true);
        this->m_option = 2;

        this->vbl_l->addWidget(this->rb_1);
        this->vbl_l->addWidget(this->rb_2);
        this->vbl_r->addWidget(this->lb_1);
        this->vbl_r->addWidget(this->lb_2);
        this->gl->addLayout(this->vbl_l, 0, 0);
        this->gl->addLayout(this->vbl_r, 0, 1);
        this->gl->addWidget(this->dbb, 1, 1);

        this->setLayout(this->gl);
        this->setWindowTitle("Case 3");
        this->resize(200, 100);
        this->setModal(true);

        this->m_mode = 3;
    }

    connect(this->dbb, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(this->dbb, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(this->dbb, SIGNAL(accepted()), this, SLOT(OnDialogS2BResAccept()));
    connect(this->dbb, SIGNAL(rejected()), this, SLOT(OnDialogS2BResReject()));
    connect(this->rb_1, SIGNAL(clicked()), this, SLOT(OnRB1Triggered()));
    connect(this->rb_2, SIGNAL(clicked()), this, SLOT(OnRB2Triggered()));
    connect(this->rb_3, SIGNAL(clicked()), this, SLOT(OnRB3Triggered()));
}

S2BResDialog::~S2BResDialog()
{
    disconnect(this->dbb, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(this->dbb, &QDialogButtonBox::rejected, this, &QDialog::reject);
    disconnect(this->dbb, SIGNAL(accepted()), this, SLOT(OnDialogS2BResAccept()));
    disconnect(this->dbb, SIGNAL(rejected()), this, SLOT(OnDialogS2BResReject()));
    disconnect(this->rb_1, SIGNAL(clicked()), this, SLOT(OnRB1Triggered()));
    disconnect(this->rb_2, SIGNAL(clicked()), this, SLOT(OnRB2Triggered()));
    disconnect(this->rb_3, SIGNAL(clicked()), this, SLOT(OnRB3Triggered()));

    if (nullptr != this->m_RegEV)
    {
        delete this->m_RegEV;
        this->m_RegEV = nullptr;
    }
    if (nullptr != this->gl)
    {
        delete this->gl;
        this->gl = nullptr;
    }
    if (nullptr != this->vbl_l)
    {
        delete this->vbl_l;
        this->vbl_l = nullptr;
    }
    if (nullptr != this->vbl_r)
    {
        delete this->vbl_r;
        this->vbl_r = nullptr;
    }
    if (nullptr != this->hbl)
    {
        delete this->hbl;
        this->hbl = nullptr;
    }
    if (nullptr != this->rb_1)
    {
        delete this->rb_1;
        this->rb_1 = nullptr;
    }
    if (nullptr != this->rb_2)
    {
        delete this->rb_2;
        this->rb_2 = nullptr;
    }
    if (nullptr != this->rb_3)
    {
        delete this->rb_3;
        this->rb_3 = nullptr;
    }
    if (nullptr != this->lb_1)
    {
        delete this->lb_1;
        this->lb_1 = nullptr;
    }
    if (nullptr != this->lb_2)
    {
        delete this->lb_2;
        this->lb_2 = nullptr;
    }
    if (nullptr != this->lb_3)
    {
        delete this->lb_3;
        this->lb_3 = nullptr;
    }
    if (nullptr != this->lb_4)
    {
        delete this->lb_4;
        this->lb_4 = nullptr;
    }
    if (nullptr != this->lb_5)
    {
        delete this->lb_5;
        this->lb_5 = nullptr;
    }
    if (nullptr != this->lb_6)
    {
        delete this->lb_6;
        this->lb_6 = nullptr;
    }
    if (nullptr != this->lb_7)
    {
        delete this->lb_7;
        this->lb_7 = nullptr;
    }
    if (nullptr != this->le_1)
    {
        delete this->le_1;
        this->le_1 = nullptr;
    }
    if (nullptr != this->le_2)
    {
        delete this->le_2;
        this->le_2 = nullptr;
    }
    if (nullptr != this->le_3)
    {
        delete this->le_3;
        this->le_3 = nullptr;
    }
    if (nullptr != this->le_4)
    {
        delete this->le_4;
        this->le_4 = nullptr;
    }
    if (nullptr != this->le_5)
    {
        delete this->le_5;
        this->le_5 = nullptr;
    }
    if (nullptr != this->le_6)
    {
        delete this->le_6;
        this->le_6 = nullptr;
    }
    if (nullptr != this->bg)
    {
        delete this->bg;
        this->bg = nullptr;
    }
    if (nullptr != this->dbb)
    {
        delete this->dbb;
        this->dbb = nullptr;
    }
}

vector<pair<int, int>> S2BResDialog::getVPos()
{
    return this->m_vPos;
}

bool S2BResDialog::isSwap()
{
    return this->m_bSwap;
}

void S2BResDialog::OnDialogS2BResAccept()
{
    this->m_vPos.clear();
    if (1 == m_mode)
    {
        if (!(this->le_1->text().isEmpty()) && !(this->le_2->text().isEmpty()))
        {
            pair<int, int> p_1(this->le_1->text().toInt(), this->le_2->text().toInt());
            this->m_vPos.push_back(p_1);
        }
        if (!(this->le_3->text().isEmpty()) && !(this->le_4->text().isEmpty()))
        {
            pair<int, int> p_2(this->le_3->text().toInt(), this->le_4->text().toInt());
            this->m_vPos.push_back(p_2);
        }
        if (!(this->le_5->text().isEmpty()) && !(this->le_6->text().isEmpty()))
        {
            pair<int, int> p_3(this->le_5->text().toInt(), this->le_6->text().toInt());
            this->m_vPos.push_back(p_3);
        }
    }
    else if (2 == m_mode)
    {
        if (!(this->le_1->text().isEmpty()) && !(this->le_2->text().isEmpty()))
        {
            pair<int, int> p_1(this->le_1->text().toInt(), this->le_2->text().toInt());
            this->m_vPos.push_back(p_1);
        }
        if (!(this->le_3->text().isEmpty()) && !(this->le_4->text().isEmpty()))
        {
            pair<int, int> p_2(this->le_3->text().toInt(), this->le_4->text().toInt());
            this->m_vPos.push_back(p_2);
        }
    }
    else
    {}
}

void S2BResDialog::OnDialogS2BResReject()
{
    this->m_option = 0;
    this->m_bSwap = false;
}

void S2BResDialog::OnRB1Triggered()
{
    if (this->rb_1->isChecked())
        this->m_bSwap = true;
    else
        this->m_bSwap = false;

    this->m_option = 1;
}

void S2BResDialog::OnRB2Triggered()
{
    this->m_bSwap = false;
    this->m_option = 2;
}

void S2BResDialog::OnRB3Triggered()
{
    this->m_bSwap = false;
    this->m_option = 3;
}

int S2BResDialog::getOption()
{
    return this->m_option;
}
