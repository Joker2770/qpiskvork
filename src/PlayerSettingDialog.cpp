/*************************************************************************
    > File Name: PlayerSettingDialog.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sun Feb  5 15:18:57 2023
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

#include "PlayerSettingDialog.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>

#include <iostream>
using namespace std;

PlayerSettingDialog::PlayerSettingDialog(QWidget *parent) :
    QDialog(parent,Qt::WindowCloseButtonHint|Qt::Drawer)
{
    this->gl = new QGridLayout();
    this->gl_p1 = new QGridLayout();
    this->gl_p2 = new QGridLayout();
    this->gb_p1 = new QGroupBox();
    this->gb_p2 = new QGroupBox();
    this->btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    this->p1_bg = new QButtonGroup();
    this->p2_bg = new QButtonGroup();
    this->p1_hbl_1 = new QHBoxLayout();
    this->p1_hbl_2 = new QHBoxLayout();
    this->p2_hbl_1 = new QHBoxLayout();
    this->p2_hbl_2 = new QHBoxLayout();
    this->hbl_btn = new QHBoxLayout();
    this->p1_rb_1 = new QRadioButton();
    this->p1_rb_2 = new QRadioButton();
    this->p2_rb_1 = new QRadioButton();
    this->p2_rb_2 = new QRadioButton();
    this->le_p1 = new QLineEdit();
    this->le_p2 = new QLineEdit();
    this->btn_p1 = new QPushButton();
    this->btn_p2 = new QPushButton();
    this->btn_exchange = new QPushButton();

    this->gl->setSpacing(5);
    this->gl_p1->setSpacing(5);
    this->gl_p2->setSpacing(5);
    this->p1_hbl_1->setSpacing(5);
    this->p1_hbl_2->setSpacing(5);
    this->p2_hbl_1->setSpacing(5);
    this->p2_hbl_2->setSpacing(5);
    this->gb_p1->setTitle(tr("Player1"));
    this->gb_p2->setTitle(tr("Player2"));
    this->p1_rb_1->setText(tr("Human"));
    this->p1_rb_2->setText(tr("AI"));
    this->p2_rb_1->setText(tr("Human"));
    this->p2_rb_2->setText(tr("AI"));
    this->p1_rb_1->setChecked(true);
    this->p2_rb_1->setChecked(true);
    this->btn_p1->setText(tr("open"));
    this->btn_p2->setText(tr("open"));
    this->btn_exchange->setText(tr("exchange(Ctrl+X)"));
    this->btn_exchange->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    this->p1_bg->setExclusive(true);
    this->p2_bg->setExclusive(true);
    this->p1_bg->addButton(this->p1_rb_1);
    this->p1_bg->addButton(this->p1_rb_2);
    this->p2_bg->addButton(this->p2_rb_1);
    this->p2_bg->addButton(this->p2_rb_2);
    this->p1_hbl_1->addWidget(p1_rb_1);
    this->p1_hbl_1->addWidget(p1_rb_2);
    this->p1_hbl_2->addWidget(le_p1);
    this->p1_hbl_2->addWidget(btn_p1);
    this->p2_hbl_1->addWidget(p2_rb_1);
    this->p2_hbl_1->addWidget(p2_rb_2);
    this->p2_hbl_2->addWidget(le_p2);
    this->p2_hbl_2->addWidget(btn_p2);
    this->hbl_btn->addWidget(this->btn_exchange);
    this->hbl_btn->addWidget(this->btn_box);
    this->gl_p1->addLayout(this->p1_hbl_1, 0, 0);
    this->gl_p1->addLayout(this->p1_hbl_2, 1, 0);
    this->gl_p2->addLayout(this->p2_hbl_1, 0, 0);
    this->gl_p2->addLayout(this->p2_hbl_2, 1, 0);
    this->gb_p1->setLayout(gl_p1);
    this->gb_p2->setLayout(gl_p2);
    this->gl->addWidget(this->gb_p1, 0, 0);
    this->gl->addWidget(this->gb_p2, 1, 0);
    this->gl->addLayout(this->hbl_btn, 2, 0);

    this->le_p1->setMaxLength(256);
    this->le_p2->setMaxLength(256);

    //this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(tr("Player Setting"));
    this->setModal(true);
    this->setLayout(this->gl);
    this->resize(450, 300);

    this->m_p1_path = this->le_p1->text();
    this->m_p2_path = this->le_p2->text();
    this->m_is_p1_human = true;
    this->m_is_p2_human = true;

    connect(this->btn_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(this->btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(this->btn_box, SIGNAL(accepted()), this, SLOT(OnDialogPlayerSettingAccept()));
    connect(this->btn_box, SIGNAL(rejected()), this, SLOT(OnDialogPlayerSettingReject()));
    connect(this->btn_p1, SIGNAL(clicked()), this, SLOT(On_p1_btnOpen_clicked()));
    connect(this->btn_p2, SIGNAL(clicked()), this, SLOT(On_p2_btnOpen_clicked()));
    connect(this->btn_exchange, SIGNAL(clicked()), this, SLOT(On_exchange_clicked()));
}

PlayerSettingDialog::~PlayerSettingDialog()
{
    if (nullptr != this->btn_exchange)
    {
        delete this->btn_exchange;
        this->btn_exchange = nullptr;
    }
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
    if (nullptr != this->hbl_btn)
    {
        delete this->hbl_btn;
        this->hbl_btn = nullptr;
    }
    if (nullptr != this->p1_bg)
    {
        delete this->p1_bg;
        this->p1_bg = nullptr;
    }
    if (nullptr != this->p2_bg)
    {
        delete this->p2_bg;
        this->p2_bg = nullptr;
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

void PlayerSettingDialog::On_p1_btnOpen_clicked()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle=tr("Choose a file");
    //QString filter="executable file(*.exe)";
    QString aFileName=QFileDialog::getOpenFileName(this, dlgTitle, curPath, nullptr);
    if (!aFileName.isEmpty())
        this->le_p1->setText(aFileName);
}

void PlayerSettingDialog::On_p2_btnOpen_clicked()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle=tr("Choose a file");
    //QString filter="executable file(*.exe)";
    QString aFileName=QFileDialog::getOpenFileName(this, dlgTitle, curPath, nullptr);
    if (!aFileName.isEmpty())
        this->le_p2->setText(aFileName);
}

void PlayerSettingDialog::On_exchange_clicked()
{
    bool b_tmp_p1_rb_1 = this->p1_rb_1->isChecked();
    bool b_tmp_p2_rb_1 = this->p2_rb_1->isChecked();
    QString s_tmp_p1 = this->le_p1->text();
    QString s_tmp_p2 = this->le_p2->text();

    if (b_tmp_p1_rb_1)
        this->p2_rb_1->setChecked(true);
    else
        this->p2_rb_2->setChecked(true);

    if (b_tmp_p2_rb_1)
        this->p1_rb_1->setChecked(true);
    else
        this->p1_rb_2->setChecked(true);

    this->le_p1->setText(s_tmp_p2);
    this->le_p2->setText(s_tmp_p1);
}

void PlayerSettingDialog::OnDialogPlayerSettingAccept()
{
    this->m_p1_path = this->le_p1->text();
    this->m_p2_path = this->le_p2->text();
    if (this->p1_rb_1->isChecked())
        this->m_is_p1_human = true;
    else
        this->m_is_p1_human = false;

    if (this->p2_rb_1->isChecked())
        this->m_is_p2_human = true;
    else
        this->m_is_p2_human = false;
}

void PlayerSettingDialog::OnDialogPlayerSettingReject()
{
    this->le_p1->setText(this->m_p1_path);
    this->le_p2->setText(this->m_p2_path);

    if (this->m_is_p1_human)
        this->p1_rb_1->setChecked(true);
    else
        this->p1_rb_2->setChecked(true);

    if (this->m_is_p2_human)
        this->p2_rb_1->setChecked(true);
    else
        this->p2_rb_2->setChecked(true);
}

bool PlayerSettingDialog::isP1Human()
{
    return this->m_is_p1_human;
}

bool PlayerSettingDialog::isP2Human()
{
    return this->m_is_p2_human;
}

const QString PlayerSettingDialog::getP1Path()
{
    return this->m_p1_path;
}

const QString PlayerSettingDialog::getP2Path()
{
    return this->m_p2_path;
}
