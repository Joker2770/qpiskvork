/*************************************************************************
    > File Name: S2BResDialog.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Sun Apr  9 09:55:20 2023
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

#ifndef __S2BRESDIALOG_H__
#define __S2BRESDIALOG_H__

#include <QDialog>
#include <QObject>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QRegExpValidator>

#include <iostream>
using namespace std;

class S2BResDialog : public QDialog
{
    Q_OBJECT
private:
    QGridLayout gl_c1, gl_c2, gl_c3, gl_opts_2, gl_opts_3;
    QHBoxLayout hbl_c1, hbl_c2;
    QRadioButton rb_1_c2, rb_2_c2, rb_3_c2, rb_1_c3, rb_2_c3;
    QLabel lb_1_c1, lb_2_c1, lb_3_c1, lb_4_c1, lb_5_c1, lb_1_c2, lb_2_c2, lb_3_c2, lb_4_c2, lb_5_c2, lb_1_c3, lb_2_c3;
    QLineEdit le_1_c1, le_2_c1, le_3_c1, le_4_c1, le_5_c1, le_6_c1, le_1_c2, le_2_c2, le_3_c2, le_4_c2;
    QButtonGroup bg_c2, bg_c3;
    QDialogButtonBox *dbb_c1, *dbb_c2, *dbb_c3;
    QRegExpValidator *m_RegEV;

    vector<pair<int, int>> m_vPos;
    int m_mode;
    int m_option_c2, m_option_c3;
    bool m_bCanceled;

private slots:
    void OnDialogS2BResAccept();
    void OnDialogS2BResReject();
    void OnRB1Triggered();
    void OnRB2Triggered();
    void OnRB3Triggered();

public:
    // S2BResDialog(QWidget* parent=nullptr);
    S2BResDialog(int i_mode, QWidget *parent = nullptr);
    ~S2BResDialog();

    vector<pair<int, int>> getVPos();
    int getOption_c2();
    int getOption_c3();
    bool isCanceled();
};

#endif
