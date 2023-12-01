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

#include <iostream>
using namespace std;

// S2BResDialog::S2BResDialog(QWidget *parent) :
//     QDialog(parent,Qt::WindowCloseButtonHint|Qt::Drawer)
// {
// }

S2BResDialog::S2BResDialog(int i_mode, QWidget *parent) : QDialog(parent, Qt::WindowCloseButtonHint | Qt::Drawer)
{
    this->dbb_c1 = nullptr;
    this->dbb_c2 = nullptr;
    this->dbb_c3 = nullptr;
    this->m_RegEV = nullptr;

    this->m_RegEV = new QRegExpValidator(QRegExp("[0-9]+$"));

    this->m_option_c2 = -1;
    this->m_option_c3 = -1;
    this->m_bCanceled = true;

    if (1 == i_mode)
    {
        this->dbb_c1 = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        this->lb_1_c1.setText(",");
        this->lb_2_c1.setText(",");
        this->lb_3_c1.setText(",");
        this->lb_4_c1.setText(" ");
        this->lb_5_c1.setText(" ");
        this->le_1_c1.setMaxLength(2);
        this->le_2_c1.setMaxLength(2);
        this->le_3_c1.setMaxLength(2);
        this->le_4_c1.setMaxLength(2);
        this->le_5_c1.setMaxLength(2);
        this->le_6_c1.setMaxLength(2);
        this->le_1_c1.setValidator(this->m_RegEV);
        this->le_2_c1.setValidator(this->m_RegEV);
        this->le_3_c1.setValidator(this->m_RegEV);
        this->le_4_c1.setValidator(this->m_RegEV);
        this->le_5_c1.setValidator(this->m_RegEV);
        this->le_6_c1.setValidator(this->m_RegEV);
        this->hbl_c1.addWidget(&(this->le_1_c1));
        this->hbl_c1.addWidget(&(this->lb_1_c1));
        this->hbl_c1.addWidget(&(this->le_2_c1));
        this->hbl_c1.addWidget(&(this->lb_4_c1));
        this->hbl_c1.addWidget(&(this->le_3_c1));
        this->hbl_c1.addWidget(&(this->lb_2_c1));
        this->hbl_c1.addWidget(&(this->le_4_c1));
        this->hbl_c1.addWidget(&(this->lb_5_c1));
        this->hbl_c1.addWidget(&(this->le_5_c1));
        this->hbl_c1.addWidget(&(this->lb_3_c1));
        this->hbl_c1.addWidget(&(this->le_6_c1));
        this->gl_c1.addLayout(&(this->hbl_c1), 0, 0);
        this->gl_c1.addWidget(this->dbb_c1, 1, 0);

        this->setWindowTitle(tr("Input 3 Move"));
        // this.setAttribute (Qt::WA_DeleteOnClose);
        this->setModal(true);
        this->setLayout(&(this->gl_c1));
        this->resize(200, 100);

        this->m_mode = 1;

        connect(this->dbb_c1, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(this->dbb_c1, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(this->dbb_c1, SIGNAL(accepted()), this, SLOT(OnDialogS2BResAccept()));
        connect(this->dbb_c1, SIGNAL(rejected()), this, SLOT(OnDialogS2BResReject()));
    }
    else if (2 == i_mode)
    {
        this->dbb_c2 = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        this->lb_1_c2.setText(tr("swap"));
        this->lb_2_c2.setText(tr("go on"));
        this->lb_3_c2.setText(",");
        this->lb_4_c2.setText(",");
        this->lb_5_c2.setText(" ");
        this->le_1_c2.setMaxLength(2);
        this->le_2_c2.setMaxLength(2);
        this->le_3_c2.setMaxLength(2);
        this->le_4_c2.setMaxLength(2);
        this->le_1_c2.setValidator(this->m_RegEV);
        this->le_2_c2.setValidator(this->m_RegEV);
        this->le_3_c2.setValidator(this->m_RegEV);
        this->le_4_c2.setValidator(this->m_RegEV);
        this->rb_1_c2.setText("1");
        this->rb_2_c2.setText("2");
        this->rb_3_c2.setText("3");

        this->bg_c2.setExclusive(true);
        this->bg_c2.addButton(&(this->rb_1_c2));
        this->bg_c2.addButton(&(this->rb_2_c2));
        this->bg_c2.addButton(&(this->rb_3_c2));
        this->rb_2_c2.setChecked(true);
        this->m_option_c2 = 2;

        this->hbl_c2.addWidget(&(this->le_1_c2));
        this->hbl_c2.addWidget(&(this->lb_3_c2));
        this->hbl_c2.addWidget(&(this->le_2_c2));
        this->hbl_c2.addWidget(&(this->lb_5_c2));
        this->hbl_c2.addWidget(&(this->le_3_c2));
        this->hbl_c2.addWidget(&(this->lb_4_c2));
        this->hbl_c2.addWidget(&(this->le_4_c2));
        this->gl_opts_2.addWidget(&(this->rb_1_c2), 0, 0);
        this->gl_opts_2.addWidget(&(this->rb_2_c2), 1, 0);
        this->gl_opts_2.addWidget(&(this->rb_3_c2), 2, 0);
        this->gl_opts_2.addWidget(&(this->lb_1_c2), 0, 1);
        this->gl_opts_2.addWidget(&(this->lb_2_c2), 1, 1);
        this->gl_opts_2.addLayout(&(this->hbl_c2), 2, 1);
        this->gl_c2.addLayout(&(this->gl_opts_2), 0, 0);
        this->gl_c2.addWidget(this->dbb_c2, 1, 0);

        this->setWindowTitle(tr("Case 2"));
        // this.setAttribute (Qt::WA_DeleteOnClose);
        this->setModal(true);
        this->setLayout(&(this->gl_c2));
        this->resize(250, 150);

        this->m_mode = 2;

        connect(this->dbb_c2, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(this->dbb_c2, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(this->dbb_c2, SIGNAL(accepted()), this, SLOT(OnDialogS2BResAccept()));
        connect(this->dbb_c2, SIGNAL(rejected()), this, SLOT(OnDialogS2BResReject()));
        connect(&(this->rb_1_c2), SIGNAL(clicked()), this, SLOT(OnRB1Triggered()));
        connect(&(this->rb_2_c2), SIGNAL(clicked()), this, SLOT(OnRB2Triggered()));
        connect(&(this->rb_3_c2), SIGNAL(clicked()), this, SLOT(OnRB3Triggered()));
    }
    else if (3 == i_mode)
    {
        this->dbb_c3 = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        this->lb_1_c3.setText(tr("swap"));
        this->lb_2_c3.setText(tr("go on"));
        this->rb_1_c3.setText("1");
        this->rb_2_c3.setText("2");

        this->bg_c3.setExclusive(true);
        this->bg_c3.addButton(&(this->rb_1_c3));
        this->bg_c3.addButton(&(this->rb_2_c3));
        this->rb_2_c3.setChecked(true);
        this->m_option_c3 = 2;

        this->gl_opts_3.addWidget(&(this->rb_1_c3), 0, 0);
        this->gl_opts_3.addWidget(&(this->rb_2_c3), 1, 0);
        this->gl_opts_3.addWidget(&(this->lb_1_c3), 0, 1);
        this->gl_opts_3.addWidget(&(this->lb_2_c3), 1, 1);
        this->gl_c3.addLayout(&(this->gl_opts_3), 0, 0);
        this->gl_c3.addWidget(this->dbb_c3, 1, 0);

        this->setWindowTitle(tr("Case 3"));
        // this.setAttribute (Qt::WA_DeleteOnClose);
        this->setModal(true);
        this->setLayout(&(this->gl_c3));
        this->resize(150, 100);

        this->m_mode = 3;

        connect(this->dbb_c3, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(this->dbb_c3, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(this->dbb_c3, SIGNAL(accepted()), this, SLOT(OnDialogS2BResAccept()));
        connect(this->dbb_c3, SIGNAL(rejected()), this, SLOT(OnDialogS2BResReject()));
        connect(&(this->rb_1_c3), SIGNAL(clicked()), this, SLOT(OnRB1Triggered()));
        connect(&(this->rb_2_c3), SIGNAL(clicked()), this, SLOT(OnRB2Triggered()));
    }
}

S2BResDialog::~S2BResDialog()
{
    if (nullptr != this->m_RegEV)
    {
        delete this->m_RegEV;
        this->m_RegEV = nullptr;
    }
    if (nullptr != this->dbb_c1)
    {
        delete this->dbb_c1;
        this->dbb_c1 = nullptr;
    }
    if (nullptr != this->dbb_c2)
    {
        delete this->dbb_c2;
        this->dbb_c2 = nullptr;
    }
    if (nullptr != this->dbb_c3)
    {
        delete this->dbb_c3;
        this->dbb_c3 = nullptr;
    }
}

vector<pair<int, int>> S2BResDialog::getVPos()
{
    return this->m_vPos;
}

void S2BResDialog::OnDialogS2BResAccept()
{
    this->m_bCanceled = false;
    this->m_vPos.clear();
    if (1 == this->m_mode)
    {
        if (!(this->le_1_c1.text().isEmpty()) && !(this->le_2_c1.text().isEmpty()))
        {
            pair<int, int> p_1(this->le_1_c1.text().toInt(), this->le_2_c1.text().toInt());
            this->m_vPos.push_back(p_1);
        }
        if (!(this->le_3_c1.text().isEmpty()) && !(this->le_4_c1.text().isEmpty()))
        {
            pair<int, int> p_2(this->le_3_c1.text().toInt(), this->le_4_c1.text().toInt());
            this->m_vPos.push_back(p_2);
        }
        if (!(this->le_5_c1.text().isEmpty()) && !(this->le_6_c1.text().isEmpty()))
        {
            pair<int, int> p_3(this->le_5_c1.text().toInt(), this->le_6_c1.text().toInt());
            this->m_vPos.push_back(p_3);
        }
    }
    else if (2 == this->m_mode)
    {
        if (!(this->le_1_c2.text().isEmpty()) && !(this->le_2_c2.text().isEmpty()))
        {
            pair<int, int> p_1(this->le_1_c2.text().toInt(), this->le_2_c2.text().toInt());
            this->m_vPos.push_back(p_1);
        }
        if (!(this->le_3_c2.text().isEmpty()) && !(this->le_4_c2.text().isEmpty()))
        {
            pair<int, int> p_2(this->le_3_c2.text().toInt(), this->le_4_c2.text().toInt());
            this->m_vPos.push_back(p_2);
        }

        if (this->rb_1_c2.isChecked())
            this->m_option_c2 = 1;
        else if (this->rb_2_c2.isChecked())
            this->m_option_c2 = 2;
        else if (this->rb_3_c2.isChecked())
            this->m_option_c2 = 3;
    }
    else if (3 == this->m_mode)
    {
        if (this->rb_1_c3.isChecked())
            this->m_option_c3 = 1;
        else if (this->rb_2_c3.isChecked())
            this->m_option_c3 = 2;
    }
}

void S2BResDialog::OnDialogS2BResReject()
{
    this->m_bCanceled = true;
    if (2 == this->m_mode)
    {
        if (this->m_option_c2 == 1)
            this->rb_1_c2.setChecked(true);
        else if (this->m_option_c2 == 2)
            this->rb_2_c2.setChecked(true);
        else if (this->m_option_c2 == 3)
            this->rb_3_c2.setChecked(true);
    }
    if (3 == this->m_mode)
    {
        if (this->m_option_c3 == 1)
            this->rb_1_c3.setChecked(true);
        else if (this->m_option_c3 == 2)
            this->rb_2_c3.setChecked(true);
    }
}

void S2BResDialog::OnRB1Triggered()
{
    if (2 == this->m_mode)
    {
        if (this->rb_1_c2.isChecked())
            this->m_option_c2 = 1;
    }
    else if (3 == this->m_mode)
    {
        if (this->rb_1_c3.isChecked())
            this->m_option_c3 = 1;
    }
}

void S2BResDialog::OnRB2Triggered()
{
    if (2 == this->m_mode)
    {
        if (this->rb_2_c2.isChecked())
            this->m_option_c2 = 2;
    }
    else if (3 == this->m_mode)
    {
        if (this->rb_2_c3.isChecked())
            this->m_option_c3 = 2;
    }
}

void S2BResDialog::OnRB3Triggered()
{
    if (this->rb_3_c2.isChecked())
        this->m_option_c2 = 3;
}

int S2BResDialog::getOption_c2()
{
    return this->m_option_c2;
}

int S2BResDialog::getOption_c3()
{
    return this->m_option_c3;
}

bool S2BResDialog::isCanceled()
{
    return this->m_bCanceled;
}
