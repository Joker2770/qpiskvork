/*************************************************************************
    > File Name: renju_rules_test.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
 ************************************************************************/

/**
    qpiskvork is another gomoku or renju manager adapting to Windows and Linux systems.
    Copyright (C) 2022-2026 Jintao Yang <yjt950840@outlook.com>

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

// Regression tests for the renju rule engine (src/renju.cpp) against the RIF
// "International Rules of Renju", https://www.renju.net/rifrules/ :
//
//   9.1  the winner is the player who is first to attain five in a row, for
//        white an overline counts as a win as well
//   9.2  the game is won for white if black, without at the same time attaining
//        five in a row, makes a) an overline, b) a double-four or c) a
//        double-three
//   9.3  a black double-three is allowed when not more than one of the threes
//        can be made into a straight four without attaining an overline or a
//        double-four in the same intersection (9.3.a), respectively without
//        creating another forbidden double-three (9.3.b)
//
// Build and run (the tests are not part of the application build):
//
//   cmake -S . -B build -DQPISKVORK_BUILD_TESTS=ON
//   cmake --build build --target renju_rules_test
//   ctest --test-dir build -V
//
// Every case prints one line, the process returns the number of failed cases.

#include "board.h"
#include "renju.h"

#include <QString>
#include <QtGlobal>

#include <iostream>
#include <string>
#include <utility>
using namespace std;

namespace
{
// The rule engine reports its verdict through qDebug(); the test prints the
// same information itself, so drop the messages to keep the output readable.
void DiscardQtMessage(QtMsgType, const QMessageLogContext &, const QString &)
{
}

// -1 - do not check, 0 - not a win / illegal, 1 - win / legal
const int SKIP = -1;
const int NO = 0;
const int YES = 1;

int g_i_failures = 0;

void Put(Board &board, int x, int y, STONECOLOR color)
{
    if (!board.placeStone(make_pair(x, y), color))
        cout << "      [setup] could not place a stone on " << x << "," << y << "\n";
}

void Check(const string &s_name, const string &s_rif, Board &board, Renju &renju,
           int i_expect_win, int i_expect_legal, int i_expect_pattern)
{
    const int i_win = renju.checkWin(&board) ? 1 : 0;
    const int i_legal = renju.isLegal(&board) ? 1 : 0;
    const int i_pattern = renju.getRenjuState();

    const bool b_win_ok = (SKIP == i_expect_win) || (i_expect_win == i_win);
    const bool b_legal_ok = (SKIP == i_expect_legal) || (i_expect_legal == i_legal);
    const bool b_pattern_ok = (SKIP == i_expect_pattern) || (i_expect_pattern == i_pattern);
    const bool b_ok = b_win_ok && b_legal_ok && b_pattern_ok;

    if (!b_ok)
        ++g_i_failures;

    cout << (b_ok ? "PASS  " : "FAIL  ") << s_name << " (" << board.getVRecord().size() << " stones)"
         << ": win=" << i_win << " legal=" << i_legal << " pattern=" << i_pattern
         << "  | RIF: " << s_rif << "\n";
}
} // namespace

int main()
{
    qInstallMessageHandler(DiscardQtMessage);

    // board.h declares BLACK and WHITE as global enumerators of stone_color,
    // they are used directly in the cases below.

    {
        // No stone at all: nothing can be won and there is no illegal move.
        Board board;
        Renju renju;
        Check("empty board", "no win, no forbidden move", board, renju, NO, YES, PATTERN::ROW);
    }

    {
        // Black closes two open threes with its 5th stone.  RIF 9.2.c ->
        // double-three -> white wins.  Both threes can be extended to a straight
        // four without an overline or double-four, so 9.3.a does not allow it.
        // The old implementation ignored forbidden moves until 9 stones were on
        // the board, so this one used to be accepted.
        Board board;
        Renju renju;
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("double-three with only 5 stones on the board", "ILLEGAL (double-three)",
              board, renju, NO, NO, PATTERN::DOUBLE_THREE);
    }

    {
        // The very same shape plus one irrelevant stone: the verdict must not
        // depend on the number of stones that are on the board.
        Board board;
        Renju renju;
        Put(board, 0, 0, BLACK); // filler
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("double-three plus a filler stone", "ILLEGAL (double-three)",
              board, renju, NO, NO, PATTERN::DOUBLE_THREE);
    }

    {
        // Two straight fours meeting in the same intersection.  RIF 9.2.b ->
        // double-four -> white wins (7 stones, so this used to be accepted).
        Board board;
        Renju renju;
        Put(board, 4, 7, BLACK);
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 4, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("double-four with 7 stones on the board", "ILLEGAL (double-four)",
              board, renju, NO, NO, PATTERN::DOUBLE_FOUR);
    }

    {
        Board board;
        Renju renju;
        Put(board, 0, 0, BLACK);
        Put(board, 1, 0, BLACK);
        Put(board, 4, 7, BLACK);
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 4, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("double-four plus filler stones", "ILLEGAL (double-four)",
              board, renju, NO, NO, PATTERN::DOUBLE_FOUR);
    }

    {
        // The row 0 1 2 [3] _ 5 6 7 on y=0: the four 0-3 can only be completed
        // on x=4, and that would create an unbroken row of EIGHT stones, i.e. an
        // overline and not a "five in a row".  Adding one stone does therefore
        // not attain five in a row, the row is no four at all (RIF chapter 3),
        // and only the vertical four meets the intersection -> legal.
        Board board;
        Renju renju;
        Put(board, 0, 0, BLACK);
        Put(board, 1, 0, BLACK);
        Put(board, 2, 0, BLACK);
        Put(board, 5, 0, BLACK);
        Put(board, 6, 0, BLACK);
        Put(board, 7, 0, BLACK);
        Put(board, 3, 1, BLACK);
        Put(board, 3, 2, BLACK);
        Put(board, 3, 3, BLACK);
        Put(board, 3, 0, BLACK); // the move
        Check("four that can only be completed into an overline + one real four",
              "LEGAL (only one four meets the intersection)", board, renju, NO, YES, PATTERN::FOUR);
    }

    {
        // RIF 9.3.a: the "three" on y=7 (stones on x=5, 7, 8 with the gap on
        // x=6) can only be turned into a straight four by playing x=6, and the
        // resulting four x=5..8 can be completed on x=4 only -- but black already
        // owns x=3, so that completion is an overline.  The four has one legal
        // completion only, hence it is not a straight four and that three does
        // not exist -> not more than one three is made -> 9.3.a allows it.
        Board board;
        Renju renju;
        Put(board, 3, 7, BLACK);
        Put(board, 5, 7, BLACK);
        Put(board, 7, 7, BLACK);
        Put(board, 8, 5, BLACK);
        Put(board, 8, 6, BLACK);
        Put(board, 8, 7, BLACK); // the move
        Check("double-three allowed by RIF 9.3.a", "LEGAL (only one real three)",
              board, renju, NO, YES, PATTERN::THREE);
    }

    {
        // Four and three: one four and one three meet in the intersection, none
        // of the forbidden shapes of 9.2 is made -> legal.
        Board board;
        Renju renju;
        Put(board, 4, 7, BLACK);
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("four-three (4 + 3)", "LEGAL (one four, one three)",
              board, renju, NO, YES, PATTERN::FOUR);
    }

    {
        // 4-3-3: the four does not legalise the double-three (9.2.c).
        Board board;
        Renju renju;
        Put(board, 4, 7, BLACK);
        Put(board, 5, 7, BLACK);
        Put(board, 6, 7, BLACK);
        Put(board, 7, 5, BLACK);
        Put(board, 7, 6, BLACK);
        Put(board, 5, 5, BLACK);
        Put(board, 6, 6, BLACK);
        Put(board, 7, 7, BLACK); // the move
        Check("4-3-3 (one four and two threes)", "ILLEGAL (double-three)",
              board, renju, NO, NO, PATTERN::DOUBLE_THREE);
    }

    {
        // Six stones in a row: RIF 9.2.a -> overline -> white wins.  Black does
        // not attain five in a row, an unbroken row of six is an overline.
        Board board;
        Renju renju;
        Put(board, 0, 0, BLACK);
        Put(board, 1, 0, BLACK);
        Put(board, 2, 0, BLACK);
        Put(board, 4, 0, BLACK);
        Put(board, 5, 0, BLACK);
        Put(board, 6, 0, BLACK);
        Put(board, 3, 0, BLACK); // the move
        Check("black overline", "ILLEGAL (overline)", board, renju, NO, NO, PATTERN::OVERLINE);
    }

    {
        // Exactly five in a row: RIF 9.1 -> black wins.
        Board board;
        Renju renju;
        Put(board, 4, 0, BLACK);
        Put(board, 5, 0, BLACK);
        Put(board, 6, 0, BLACK);
        Put(board, 7, 0, BLACK);
        Put(board, 3, 0, BLACK); // the move
        Check("black exactly five in a row", "WIN for black", board, renju, YES, SKIP, SKIP);
    }

    {
        // Overline of white: RIF 9.1 -> white wins as well.
        Board board;
        Renju renju;
        Put(board, 0, 5, WHITE);
        Put(board, 1, 5, WHITE);
        Put(board, 2, 5, WHITE);
        Put(board, 4, 5, WHITE);
        Put(board, 5, 5, WHITE);
        Put(board, 6, 5, WHITE);
        Put(board, 3, 5, WHITE); // the move
        Check("white overline", "WIN for white", board, renju, YES, SKIP, SKIP);
    }

    {
        // The forbidden moves of RIF 9.2 are forbidden for black only.
        Board board;
        Renju renju;
        Put(board, 5, 7, WHITE);
        Put(board, 6, 7, WHITE);
        Put(board, 7, 5, WHITE);
        Put(board, 7, 6, WHITE);
        Put(board, 7, 7, WHITE); // the move
        Check("white double-three", "LEGAL (9.2 restricts black only)",
              board, renju, NO, YES, PATTERN::ROW);
    }

    if (0 == g_i_failures)
        cout << "\nall cases passed\n";
    else
        cout << "\n" << g_i_failures << " case(s) FAILED\n";

    return g_i_failures;
}
