/*************************************************************************
    > File Name: PlayerSettingDialog.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: Sun Feb  5 15:18:50 2023
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

#ifndef __PLAYER_SETTING_DIALOG_H__
#define __PLAYER_SETTING_DIALOG_H__

#include <QDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QButtonGroup>
#include <QObject>

#include <iostream>
using namespace std;

class PlayerSettingDialog : public QDialog
{
    Q_OBJECT
public:
    PlayerSettingDialog(QWidget *parent = nullptr);
    ~PlayerSettingDialog();

public:
    bool isP1Human();
    bool isP2Human();
    const QString getP1Path();
    const QString getP2Path();

private slots:
    void OnDialogPlayerSettingAccept();
    void OnDialogPlayerSettingReject();
    void On_p1_btnOpen_clicked();
    void On_p2_btnOpen_clicked();
    void On_exchange_clicked();

private:
    QGridLayout *gl, *gl_p1, *gl_p2;
    QGroupBox *gb_p1, *gb_p2;
    QHBoxLayout *p1_hbl_1, *p1_hbl_2, *p2_hbl_1, *p2_hbl_2, *hbl_btn;

    QLineEdit *le_p1, *le_p2;
    QPushButton *btn_p1, *btn_p2, *btn_exchange;

    QDialogButtonBox *btn_box;
    QRadioButton *p1_rb_1, *p1_rb_2, *p2_rb_1, *p2_rb_2;
    QButtonGroup *p1_bg, *p2_bg;

    QString m_p1_path;
    QString m_p2_path;
    bool m_is_p1_human;
    bool m_is_p2_human;
};

#endif
