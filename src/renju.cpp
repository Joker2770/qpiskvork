/*************************************************************************
    > File Name: renju.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Sat Apr  1 14:49:28 2023
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

#include "renju.h"

#include <QDebug>

#include <algorithm>
#include<iostream>
using namespace std;

int Renju::countNearStone(Board *board, const pair<int, int>& p_drt)
{
    int i_count = 0;
    if (board->GetState() == BOARDSTATUS::BOARDEMPTY)    return 0;

    pair<int, int> p_last_move = board->getVRecord().back();
    pair<int, int> p_idx = board->coord2idx(p_last_move.first);
    pair<int, int> p_drt_idx(p_idx.first + p_drt.first, p_idx.second + p_drt.second);

    while (!board->isPosOutOfBoard(p_drt_idx) && !board->isPosEmpty(p_drt_idx))
    {
        for (size_t i = 0; i < board->getVRecord().size(); ++i)
        {
            if (board->getVRecord().at(i).first == board->idx2Coord(p_drt_idx))
            {
                if (board->getVRecord().at(i).second == p_last_move.second)
                {
                    i_count++;
                    break;
                }
                else
                    return i_count;
            }
        }
        p_drt_idx.first += p_drt.first;
        p_drt_idx.second += p_drt.second;
        if (abs(p_idx.first - p_drt_idx.first) > 4 || abs(p_idx.second - p_drt_idx.second) > 4)
            break;
    }

    return i_count;
}

bool Renju::isOverLine(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_down(0, 1), p_drt_left(-1, 0), p_drt_right(1, 0), p_drt_leftup(-1, -1), p_drt_rightdown(1, 1), p_drt_rightup(1, -1), p_drt_leftdown(-1, 1);
    int i_up = countNearStone(board, p_drt_up);
    int i_down = countNearStone(board, p_drt_down);
    int i_left = countNearStone(board, p_drt_left);
    int i_right = countNearStone(board, p_drt_right);
    int i_leftup = countNearStone(board, p_drt_leftup);
    int i_rightdown = countNearStone(board, p_drt_rightdown);
    int i_leftdown = countNearStone(board, p_drt_leftdown);
    int i_rightup = countNearStone(board, p_drt_rightup);

    if (i_up + i_down > 4 || i_left + i_right > 4 || i_leftup + i_rightdown > 4 || i_leftdown + i_rightup > 4)
        return true;

    return false;
}

int Renju::countA4(Board *board, const pair<int, int>& p_drt)
{
    int i_count = 0;
    if (board->getVRecord().size() >= 9)
    {
        vector<int> vColor;
        vColor.push_back(1);

        pair<int, int> p_last_move = board->getVRecord().back();
        pair<int, int> p_idx = board->coord2idx(p_last_move.first);
        pair<int, int> p_drt_idx(p_idx.first + p_drt.first, p_idx.second + p_drt.second);
        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first += p_drt.first;
            p_drt_idx.second += p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 4 || abs(p_idx.second - p_drt_idx.second) > 4)
                break;
        }

        reverse(vColor.begin(), vColor.end());
        p_drt_idx.first = p_idx.first - p_drt.first;
        p_drt_idx.second = p_idx.second - p_drt.second;
        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first -= p_drt.first;
            p_drt_idx.second -= p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 4 || abs(p_idx.second - p_drt_idx.second) > 4)
                break;
        }

        // for (size_t i = 0; i < vColor.size(); i++)
        // {
        //     cout << vColor[i] ;
        // }
        // cout <<endl;

        if (vColor.size() >= 5)
        {
            int i_flag = 0;
            for (size_t j = 0; j <= vColor.size() - 5; ++j)
            {
                for (size_t i = 0; i < 5; ++i)
                {
                    if (((A4_SHAPS[i][0]) == (vColor[j]))
                     && ((A4_SHAPS[i][1]) == (vColor[j+1]))
                      && ((A4_SHAPS[i][2]) == (vColor[j+2]))
                       && ((A4_SHAPS[i][3]) == (vColor[j+3]))
                        && ((A4_SHAPS[i][4]) == (vColor[j+4])))
                    {
                        if ((i == 1 || i == 3)) //'1011101' at left3 to right3, '10111101' at left4 and right4, '111010111' at mid, '11110111', '11101111'
                        {
                            if (i == 1)
                                i_flag |= 0x01;
                            else if (i == 3)
                                i_flag |= 0x02;

                            break;
                        }
                        else if (i == 2)    //'11011011' at left4 and right4, '11110111', '11101111'
                        {
                            if ((i_flag & 0x04) != 0x04)
                                i_flag |= 0x04;
                            else if ((i_flag & 0x08) != 0x08)
                                i_flag |= 0x08;

                            break;
                        }
                        else //'11110011', '11001111', '10110111', '11101101'
                            i_count = 1;
                    }
                }
            }

            if ((i_flag & 0x07) == 0x07) //'11101111' or '11110111'
                i_count = 1;
            else if ((i_flag & 0x03) == 0x03) //'1011101', '10111101'
                i_count = 2;
            else if ((i_flag & (0x04 | 0x08)) == (0x04 | 0x08)) //'11011011'
                i_count = 2;
            else if (((i_flag & 0x03) == 0x01) || ((i_flag & 0x03) == 0x02))
                i_count = 1;
            else if (((i_flag & 0x04) == 0x04) || ((i_flag & 0x08) == 0x08))
                i_count = 1;
        }
    }

    return i_count;
}

bool Renju::isDoubleFour(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    int i_up = countA4(board, p_drt_up);
    int i_left = countA4(board, p_drt_left);
    int i_leftup = countA4(board, p_drt_leftup);
    int i_leftdown = countA4(board, p_drt_leftdown);

    if (i_up + i_left + i_leftup + i_leftdown >= 2)
        return true;

    return false;
}

int Renju::countA3(Board *board, const pair<int, int>& p_drt)
{
    if (board->getVRecord().size() >= 6)
    {
        vector<int> vColor;
        vColor.push_back(1);

        pair<int, int> p_last_move = board->getVRecord().back();
        pair<int, int> p_idx = board->coord2idx(p_last_move.first);
        pair<int, int> p_drt_idx(p_idx.first + p_drt.first, p_idx.second + p_drt.second);
        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first += p_drt.first;
            p_drt_idx.second += p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 4 || abs(p_idx.second - p_drt_idx.second) > 4)
                break;
        }

        reverse(vColor.begin(), vColor.end());
        p_drt_idx.first = p_idx.first - p_drt.first;
        p_drt_idx.second = p_idx.second - p_drt.second;
        while (!board->isPosOutOfBoard(p_drt_idx))
        {
            if (STONECOLOR::BLACK == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(1);
            else if (STONECOLOR::WHITE == board->getIdxStoneColor(p_drt_idx))
                vColor.push_back(2);
            else
                vColor.push_back(0);

            p_drt_idx.first -= p_drt.first;
            p_drt_idx.second -= p_drt.second;
            if (abs(p_idx.first - p_drt_idx.first) > 4 || abs(p_idx.second - p_drt_idx.second) > 4)
                break;
        }

        // for (size_t i = 0; i < vColor.size(); i++)
        // {
        //     cout << vColor[i] ;
        // }
        // cout <<endl;
        

        if (vColor.size() >= 6)
        {
            for (size_t j = 0; j <= vColor.size() - 6; ++j)
            {
                for (size_t i = 0; i < 4; ++i)
                {
                    //qDebug() << "A3_element: " << A3_SHAPS[i][0] << A3_SHAPS[i][1] << A3_SHAPS[i][2] << A3_SHAPS[i][3] << A3_SHAPS[i][4] << A3_SHAPS[i][5];
                    //qDebug() << "vColor_element: " << vColor[j] << vColor[j + 1] << vColor[j + 2] << vColor[j + 3] << vColor[j + 4] << vColor[j + 5];
                    if (((A3_SHAPS[i][0]) == (vColor[j]))
                     && ((A3_SHAPS[i][1]) == (vColor[j+1]))
                      && ((A3_SHAPS[i][2]) == (vColor[j+2]))
                       && ((A3_SHAPS[i][3]) == (vColor[j+3]))
                        && ((A3_SHAPS[i][4]) == (vColor[j+4]))
                       && ((A3_SHAPS[i][5]) == (vColor[j+5])))
                    {
                        //Never could be a double-three(alive) in one line. ('101101' is not alive)

                        /**
                         * @brief 9.3 A black double-three is allowed if at least one of the following conditions  a)   or  b) is/are fulfilled:
                         * a) Not more than one of the three's can be made to a straight four when adding another stone in just any intersection,
                         *  without at the same time an overline or double-four is attained in this intersection. To find out which double-three's,
                         *  which are allowed, you must make the move, which causes the double-three, in your mind, and then continue trying to
                         *  make straight fours, which are allowed, in your mind.
                         * b) Not more than one of the three's can be made to a straight four when adding another stone in just any intersection,
                         *  without at the same time at least two three's meet in this intersection and make a forbidden double-three.
                         *  To find out if this last double-three is forbidden or not, you must at first examine if the double-three is
                         *  allowed according to a) above, and then in your mind continue trying to make straight fours of the three's in your mind.
                         *  If, when making a straight four in your mind, another double-three would be attained also these double-three's must be
                         *  examined in the same way as it is described in this point 9.3, etc.
                         *
                         */
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

bool Renju::isFourThree(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    int i_up_4 = countA4(board, p_drt_up);
    int i_left_4 = countA4(board, p_drt_left);
    int i_leftup_4 = countA4(board, p_drt_leftup);
    int i_leftdown_4 = countA4(board, p_drt_leftdown);
    int i_up_3 = countA3(board, p_drt_up);
    int i_left_3 = countA3(board, p_drt_left);
    int i_leftup_3 = countA3(board, p_drt_leftup);
    int i_leftdown_3 = countA3(board, p_drt_leftdown);

    if (((i_up_4 + i_left_4 + i_leftup_4 + i_leftdown_4) == 1) && (i_up_3 + i_left_3 + i_leftup_3 + i_leftdown_3 >= 1))
    {
        if (i_up_4 == 1)
        {
            if (i_left_3 + i_leftup_3 + i_leftdown_3 == 0) // only four
                return false;
            else if (i_left_3 + i_leftup_3 + i_leftdown_3 > 1) // 433*
                return false;
            else if (i_left_3 + i_leftup_3 + i_leftdown_3 == 1)
                return true;
        }
        else if (i_left_4 == 1)
        {
            if (i_up_3 + i_leftup_3 + i_leftdown_3 == 0) // only four
                return false;
            else if (i_up_3 + i_leftup_3 + i_leftdown_3 > 1) // 433*
                return false;
            else if (i_up_3 + i_leftup_3 + i_leftdown_3 == 1)
                return true;
        }
        else if (i_leftup_4 == 1)
        {
            if (i_up_3 + i_left_3 + i_leftdown_3 == 0) // only four
                return false;
            else if (i_up_3 + i_left_3 + i_leftdown_3 > 1) // 433*
                return false;
            else if (i_up_3 + i_left_3 + i_leftdown_3 == 1)
                return true;
        }
        else if (i_leftdown_4 == 1)
        {
            if (i_left_3 + i_leftup_3 + i_up_3 == 0) // only four
                return false;
            else if (i_left_3 + i_leftup_3 + i_up_3 > 1) //433*
                return false;
            else if (i_left_3 + i_leftup_3 + i_up_3 == 1)
                return true;
        }
    }

    return false;
}

bool Renju::isFour(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    int i_up_4 = countA4(board, p_drt_up);
    int i_left_4 = countA4(board, p_drt_left);
    int i_leftup_4 = countA4(board, p_drt_leftup);
    int i_leftdown_4 = countA4(board, p_drt_leftdown);
    int i_up_3 = countA3(board, p_drt_up);
    int i_left_3 = countA3(board, p_drt_left);
    int i_leftup_3 = countA3(board, p_drt_leftup);
    int i_leftdown_3 = countA3(board, p_drt_leftdown);

    if (((i_up_4 + i_left_4 + i_leftup_4 + i_leftdown_4) == 1) && (i_up_3 + i_left_3 + i_leftup_3 + i_leftdown_3 < 2))
    {
        if (i_up_4 == 1)
        {
            if (i_left_3 + i_leftup_3 + i_leftdown_3 == 0)
                return true;
        }
        else if (i_left_4 == 1)
        {
            if (i_up_3 + i_leftup_3 + i_leftdown_3 == 0)
                return true;
        }
        else if (i_leftup_4 == 1)
        {
            if (i_up_3 + i_left_3 + i_leftdown_3 == 0)
                return true;
        }
        else if (i_leftdown_4 == 1)
        {
            if (i_up_3 + i_left_3 + i_leftup_3 == 0)
                return true;
        }
    }

    return false;
}

bool Renju::isDoubleThree(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    int i_up_4 = countA4(board, p_drt_up);
    int i_left_4 = countA4(board, p_drt_left);
    int i_leftup_4 = countA4(board, p_drt_leftup);
    int i_leftdown_4 = countA4(board, p_drt_leftdown);
    int i_up_3 = countA3(board, p_drt_up);
    int i_left_3 = countA3(board, p_drt_left);
    int i_leftup_3 = countA3(board, p_drt_leftup);
    int i_leftdown_3 = countA3(board, p_drt_leftdown);

    if ((i_up_4 + i_left_4 + i_leftup_4 + i_leftdown_4 < 2) && (i_up_3 + i_left_3 + i_leftup_3 + i_leftdown_3 >= 2))
    {
        if (i_up_4 + i_left_4 + i_leftup_4 + i_leftdown_4 == 0)
        {
            if (i_up_3 + i_left_3 + i_leftup_3 + i_leftdown_3 >= 2)
                return true;
        }
        else
        {
            if (i_up_4 == 1)
            {
                if (i_left_3 + i_leftup_3 + i_leftdown_3 >= 2) // 433*
                    return true;
                else if (i_left_3 + i_leftup_3 + i_leftdown_3 == 1) // 43
                    return false;
                else // only 4
                    return false;
            }
            else if (i_left_4 == 1)
            {
                if (i_up_3 + i_leftup_3 + i_leftdown_3 >= 2) // 433*
                    return true;
                else if (i_up_3 + i_leftup_3 + i_leftdown_3 == 1) // 43
                    return false;
                else // only 4
                    return false;
            }
            else if (i_leftup_4 == 1)
            {
                if (i_up_3 + i_left_3 + i_leftdown_3 >= 2) // 433*
                    return true;
                else if (i_up_3 + i_left_3 + i_leftdown_3 == 1) // 43
                    return false;
                else // only 4
                    return false;
            }
            else if (i_leftdown_4 == 1)
            {
                if (i_up_3 + i_leftup_3 + i_left_3 >= 2) // 433*
                    return true;
                else if (i_up_3 + i_leftup_3 + i_left_3 == 1) // 43
                    return false;
                else // only 4
                    return false;
            }
        }
    }

    return false;
}

bool Renju::isThree(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_left(-1, 0), p_drt_leftup(-1, -1), p_drt_leftdown(-1, 1);
    int i_up_4 = countA4(board, p_drt_up);
    int i_left_4 = countA4(board, p_drt_left);
    int i_leftup_4 = countA4(board, p_drt_leftup);
    int i_leftdown_4 = countA4(board, p_drt_leftdown);
    int i_up_3 = countA3(board, p_drt_up);
    int i_left_3 = countA3(board, p_drt_left);
    int i_leftup_3 = countA3(board, p_drt_leftup);
    int i_leftdown_3 = countA3(board, p_drt_leftdown);

    if ((i_up_4 + i_left_4 + i_leftup_4 + i_leftdown_4 == 0) && (i_up_3 + i_left_3 + i_leftup_3 + i_leftdown_3 == 1))
        return true;

    return false;
}

bool Renju::isLegal(Board *board)
{
    if (board->getVRecord().back().second == STONECOLOR::BLACK)
    {
        if (this->isOverLine(board))
        {
            qDebug() << "Over-Line";
            this->m_renju_state = PATTERN::OVERLINE;
            return false;
        }
        else if (this->isDoubleFour(board))
        {
            qDebug() << "Double-Four";
            this->m_renju_state = PATTERN::DOUBLE_FOUR;
            return false;
        }
        else if (this->isFourThree(board))
        {
            qDebug() << "Four-Three";
            return true;
        }
        else if (this->isFour(board))
        {
            qDebug() << "Four";
            this->m_renju_state = PATTERN::FOUR;
            return true;
        }
        else if (this->isDoubleThree(board))
        {
            qDebug() << "Double-Three";
            this->m_renju_state = PATTERN::DOUBLE_THREE;
            return false;
        }
        else if (this->isThree(board))
        {
            qDebug() << "three";
            this->m_renju_state = PATTERN::THREE;
            return true;
        }
        
    }

    return true;
}

int Renju::getRenjuState()
{
    return this->m_renju_state;
}

bool Renju::checkWin(Board *board)
{
    pair<int, int> p_drt_up(0, -1), p_drt_down(0, 1), p_drt_left(-1, 0), p_drt_right(1, 0)
            , p_drt_leftup(-1, -1), p_drt_rightdown(1, 1), p_drt_rightup(1, -1), p_drt_leftdown(-1, 1);
    int i_up = countNearStone(board, p_drt_up);
    int i_down = countNearStone(board, p_drt_down);
    int i_left = countNearStone(board, p_drt_left);
    int i_right = countNearStone(board, p_drt_right);
    int i_leftup = countNearStone(board, p_drt_leftup);
    int i_rightdown = countNearStone(board, p_drt_rightdown);
    int i_leftdown = countNearStone(board, p_drt_leftdown);
    int i_rightup = countNearStone(board, p_drt_rightup);

    if (board->getVRecord().back().second == STONECOLOR::BLACK)
    {
        if (i_up + i_down == 4 || i_left + i_right == 4 || i_leftup + i_rightdown == 4 || i_leftdown + i_rightup == 4)
        {
            this->m_renju_state = PATTERN::FIVE_IN_A_ROW;
            return true;
        }
    }
    else
    {
        if (i_up + i_down >= 4 || i_left + i_right >= 4 || i_leftup + i_rightdown >= 4 || i_leftdown + i_rightup >= 4)
        {
            this->m_renju_state = PATTERN::FIVE_IN_A_ROW;
            return true;
        }
    }

    return false;
}

