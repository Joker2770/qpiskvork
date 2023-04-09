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
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QRegExpValidator>

#include<iostream>
using namespace std;

class S2BResDialog : public QDialog
{
    Q_OBJECT
private:
    QGridLayout *gl;
    QVBoxLayout *vbl_l, *vbl_r;
    QHBoxLayout *hbl;
    QRadioButton *rb_1, *rb_2, *rb_3;
    QLabel *lb_1, *lb_2, *lb_3, *lb_4, *lb_5, *lb_6, *lb_7;
    QLineEdit *le_1, *le_2, *le_3, *le_4, *le_5, *le_6;
    QButtonGroup *bg;
    QDialogButtonBox *dbb;
    QRegExpValidator *m_RegEV;

    vector<pair<int,int>> m_vPos;
    int m_mode;
    int m_option;
    bool m_bSwap;

private slots:
    void OnDialogS2BResAccept();
    void OnDialogS2BResReject();
    void OnRB1Triggered();
    void OnRB2Triggered();
    void OnRB3Triggered();

public:
    //S2BResDialog(QWidget* parent=nullptr);
    S2BResDialog(int i_mode, QWidget* parent=nullptr);
    ~S2BResDialog();

    vector<pair<int,int>> getVPos();
    int getOption();
    bool isSwap();
};

#endif
