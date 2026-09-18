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
#include <set>
#include <utility>
#include <vector>
using namespace std;

namespace
{
// ---------------------------------------------------------------------------
// International Rules of Renju (RIF), chapter 3 "terms and definitions":
//
//   FIVE IN A ROW   an unbroken row with five stones
//   OVERLINE        an unbroken row consisting of six or more stones
//   FOUR            a row with four stones to which you can add one more stone
//                   to attain five in a row
//   STRAIGHT FOUR   an unbroken row with four stones to which you, in two
//                   different ways, can add one more stone to attain five
//   THREE           a row with three stones to which you, without at the same
//                   time a five in a row is made, can add one more stone to
//                   attain a straight four
//
// A shape is identified by the set of its stones: 011110 is one straight four
// with two completion points and never two fours, and a row whose only
// completion would produce six or more stones in a row is an overline, so it is
// not a four at all.
// ---------------------------------------------------------------------------

const int ST_EMPTY = 0;
const int ST_EDGE = 3;
const int STONE_BLACK = 1;
const int STONE_WHITE = 2;

// One direction per line is enough, every line is analysed over the whole board
// around the intersection.
const int DIRECTIONS[4][2] = {{0, -1}, {-1, 0}, {-1, -1}, {-1, 1}};

// RIF 9.3.b is recursive by nature ("... must be examined in the same way as it
// is described in this point 9.3, etc."); the depth is capped to stay finite.
const int MAX_THREE_DEPTH = 3;

inline int CellId(int x, int y)
{
    return (y * MAXBSIZE) + x;
}

// A set of stones in ascending cell order, used as a key to compare shapes.
typedef vector<int> Shape;

void SortShape(Shape &shape)
{
    sort(shape.begin(), shape.end());
}

void RemoveCell(Shape &shape, int x, int y)
{
    shape.erase(remove(shape.begin(), shape.end(), CellId(x, y)), shape.end());
}

bool ShapeHasCell(const Shape &shape, int x, int y)
{
    return (shape.end() != find(shape.begin(), shape.end(), CellId(x, y)));
}

// ---------------------------------------------------------------------------
// View of a board which can additionally hold stones that are played "in mind"
// (RIF 9.3: "you must make the move, which causes the double-three, in your
// mind").  0 - empty, 1 - black, 2 - white, 3 - outside of the board.
// ---------------------------------------------------------------------------
class BoardView
{
public:
    explicit BoardView(Board *p_board)
    {
        for (int y = 0; y < MAXBSIZE; ++y)
        {
            for (int x = 0; x < MAXBSIZE; ++x)
                m_cell[y][x] = ST_EDGE;
        }

        const unsigned int i_width = p_board->getBSize().first;
        const unsigned int i_height = p_board->getBSize().second;
        for (unsigned int y = 0; y < i_height; ++y)
        {
            for (unsigned int x = 0; x < i_width; ++x)
                m_cell[y][x] = ST_EMPTY;
        }

        const vector<pair<int, int>> &vRecord = p_board->getVRecord();
        for (size_t i = 0; i < vRecord.size(); ++i)
        {
            const pair<int, int> idx = p_board->coord2idx(vRecord[i].first);
            if ((0 <= idx.first) && (idx.first < (int)i_width) && (0 <= idx.second) && (idx.second < (int)i_height))
                m_cell[idx.second][idx.first] = vRecord[i].second;
        }
    }

    void AddStone(int x, int y, int i_color)
    {
        m_vExtra.push_back(make_pair(pair<int, int>(x, y), i_color));
    }

    void RemoveStone()
    {
        if (!m_vExtra.empty())
            m_vExtra.pop_back();
    }

    int At(int x, int y) const
    {
        if ((0 > x) || (0 > y) || (MAXBSIZE <= x) || (MAXBSIZE <= y))
            return ST_EDGE;

        for (vector<pair<pair<int, int>, int>>::const_reverse_iterator it = m_vExtra.rbegin(); it != m_vExtra.rend(); ++it)
        {
            if ((it->first.first == x) && (it->first.second == y))
                return it->second;
        }

        return m_cell[y][x];
    }

private:
    int m_cell[MAXBSIZE][MAXBSIZE];
    vector<pair<pair<int, int>, int>> m_vExtra;
};

// Length of the unbroken run of `i_color` through (x, y) in the axis (dx, dy),
// the stone on (x, y) itself included.
int RunLength(const BoardView &view, int x, int y, int dx, int dy, int i_color)
{
    int i_length = 1;

    for (int i_step = 1; view.At(x + (i_step * dx), y + (i_step * dy)) == i_color; ++i_step)
        ++i_length;
    for (int i_step = 1; view.At(x - (i_step * dx), y - (i_step * dy)) == i_color; ++i_step)
        ++i_length;

    return i_length;
}

// Stones of the unbroken run of `i_color` through (x, y) in the axis (dx, dy).
Shape RunShape(const BoardView &view, int x, int y, int dx, int dy, int i_color)
{
    Shape shape;

    int i_step = 0;
    while (view.At(x + (i_step * dx), y + (i_step * dy)) == i_color)
        --i_step;

    for (++i_step; view.At(x + (i_step * dx), y + (i_step * dy)) == i_color; ++i_step)
        shape.push_back(CellId(x + (i_step * dx), y + (i_step * dy)));

    SortShape(shape);
    return shape;
}

// RIF 9.2.a: an unbroken row of six or more stones.
bool HasOverLine(const BoardView &view, int x, int y, int i_color)
{
    for (int i_dir = 0; i_dir < 4; ++i_dir)
    {
        if (6 <= RunLength(view, x, y, DIRECTIONS[i_dir][0], DIRECTIONS[i_dir][1], i_color))
            return true;
    }

    return false;
}

// Number of distinct fours that are made by playing `i_color` on (x, y) and
// that meet each other in this intersection.  The completion stone is played in
// mind, so a row that can only be completed into an overline is not a four.
int CountFours(BoardView &view, int x, int y, int i_color)
{
    set<Shape> sFours;

    for (int i_dir = 0; i_dir < 4; ++i_dir)
    {
        const int dx = DIRECTIONS[i_dir][0], dy = DIRECTIONS[i_dir][1];

        for (int i_off = -4; i_off <= 4; ++i_off)
        {
            if (0 == i_off)
                continue;

            const int px = x + (i_off * dx), py = y + (i_off * dy);
            if (ST_EMPTY != view.At(px, py))
                continue;

            view.AddStone(px, py, i_color);
            if (5 == RunLength(view, px, py, dx, dy, i_color))
            {
                Shape shape = RunShape(view, px, py, dx, dy, i_color);
                RemoveCell(shape, px, py);
                if (ShapeHasCell(shape, x, y))
                    sFours.insert(shape);
            }
            view.RemoveStone();
        }
    }

    return (int)sFours.size();
}

// RIF chapter 3: two different stones must be able to complete the row of four.
bool IsStraightFour(BoardView &view, int x, int y, int dx, int dy, int i_color)
{
    if (4 != RunLength(view, x, y, dx, dy, i_color))
        return false;

    int i_ways = 0;
    for (int i_sign = -1; (i_sign <= 1) && (i_ways < 2); i_sign += 2)
    {
        int i_step = i_sign;
        while (view.At(x + (i_step * dx), y + (i_step * dy)) == i_color)
            i_step += i_sign;

        const int qx = x + (i_step * dx), qy = y + (i_step * dy);
        if (ST_EMPTY != view.At(qx, qy))
            continue;

        view.AddStone(qx, qy, i_color);
        if (5 == RunLength(view, qx, qy, dx, dy, i_color))
            ++i_ways;
        view.RemoveStone();
    }

    return (2 == i_ways);
}

// Distinct threes that are made by playing `i_color` on (x, y).
void CollectThrees(BoardView &view, int x, int y, int i_color, set<Shape> &sThrees)
{
    for (int i_dir = 0; i_dir < 4; ++i_dir)
    {
        const int dx = DIRECTIONS[i_dir][0], dy = DIRECTIONS[i_dir][1];

        for (int i_off = -4; i_off <= 4; ++i_off)
        {
            if (0 == i_off)
                continue;

            const int px = x + (i_off * dx), py = y + (i_off * dy);
            if (ST_EMPTY != view.At(px, py))
                continue;

            view.AddStone(px, py, i_color);
            if (IsStraightFour(view, px, py, dx, dy, i_color))
            {
                Shape shape = RunShape(view, px, py, dx, dy, i_color);
                RemoveCell(shape, px, py);
                if (ShapeHasCell(shape, x, y))
                    sThrees.insert(shape);
            }
            view.RemoveStone();
        }
    }
}

bool IsForbiddenDoubleThree(BoardView &view, int x, int y, int i_color, int i_depth);

// RIF 9.3.a: "Not more than one of the three's can be made to a straight four
// when adding another stone in just any intersection, without at the same time
// an overline or double-four is attained in this intersection."
// RIF 9.3.b: "... without at the same time at least two three's meet in this
// intersection and make a forbidden double-three."
bool CanExtendToStraightFour(BoardView &view, const Shape &three, int x, int y, int i_color, int i_depth)
{
    for (int i_dir = 0; i_dir < 4; ++i_dir)
    {
        const int dx = DIRECTIONS[i_dir][0], dy = DIRECTIONS[i_dir][1];

        for (int i_off = -4; i_off <= 4; ++i_off)
        {
            if (0 == i_off)
                continue;

            const int px = x + (i_off * dx), py = y + (i_off * dy);
            if (ST_EMPTY != view.At(px, py))
                continue;

            view.AddStone(px, py, i_color);

            bool b_candidate = false;
            if (IsStraightFour(view, px, py, dx, dy, i_color))
            {
                Shape shape = RunShape(view, px, py, dx, dy, i_color);
                RemoveCell(shape, px, py);
                b_candidate = (shape == three);
            }

            bool b_allowed = false;
            if (b_candidate)
            {
                b_allowed = !HasOverLine(view, px, py, i_color) &&
                            (CountFours(view, px, py, i_color) < 2) &&
                            !IsForbiddenDoubleThree(view, px, py, i_color, i_depth + 1);
            }

            view.RemoveStone();

            if (b_allowed)
                return true;
        }
    }

    return false;
}

// RIF 9.2.c with the exceptions of 9.3: more than one three meeting in the same
// intersection is forbidden for black, unless not more than one of them can be
// made into a straight four without creating an overline, a double-four (9.3.a)
// or another forbidden double-three (9.3.b).
bool IsForbiddenDoubleThree(BoardView &view, int x, int y, int i_color, int i_depth)
{
    if (MAX_THREE_DEPTH < i_depth)
        return true; // undecidable within the search limit: do not grant 9.3

    set<Shape> sThrees;
    CollectThrees(view, x, y, i_color, sThrees);
    if (2 > sThrees.size())
        return false;

    int i_extendable = 0;
    for (set<Shape>::const_iterator it = sThrees.begin(); it != sThrees.end(); ++it)
    {
        if (CanExtendToStraightFour(view, *it, x, y, i_color, i_depth))
        {
            ++i_extendable;
            if (2 <= i_extendable)
                return true;
        }
    }

    return false;
}

pair<int, int> LastMoveIdx(Board *board)
{
    return board->coord2idx(board->getVRecord().back().first);
}

} // namespace

Renju::Renju() : m_renju_state(PATTERN::ROW)
{
}

bool Renju::isOverLine(Board *board)
{
    BoardView view(board);
    const pair<int, int> idx = LastMoveIdx(board);

    return HasOverLine(view, idx.first, idx.second, STONE_BLACK);
}

bool Renju::isDoubleFour(Board *board)
{
    BoardView view(board);
    const pair<int, int> idx = LastMoveIdx(board);

    return (2 <= CountFours(view, idx.first, idx.second, STONE_BLACK));
}

bool Renju::isDoubleThree(Board *board)
{
    BoardView view(board);
    const pair<int, int> idx = LastMoveIdx(board);

    return IsForbiddenDoubleThree(view, idx.first, idx.second, STONE_BLACK, 0);
}



bool Renju::isLegal(Board *board)
{
    // There is no illegal move without a last move.
    if ((nullptr == board) || board->getVRecord().empty())
        return true;

    // RIF 9.2: the forbidden moves are forbidden for black only.
    if (STONECOLOR::BLACK != board->getVRecord().back().second)
        return true;

    if (this->isOverLine(board)) // RIF 9.2.a
    {
        qDebug() << "Over-Line";
        this->m_renju_state = PATTERN::OVERLINE;
        return false;
    }

    if (this->isDoubleFour(board)) // RIF 9.2.b
    {
        qDebug() << "Double-Four";
        this->m_renju_state = PATTERN::DOUBLE_FOUR;
        return false;
    }

    if (this->isDoubleThree(board)) // RIF 9.2.c, the exceptions of 9.3 are applied
    {
        qDebug() << "Double-Three";
        this->m_renju_state = PATTERN::DOUBLE_THREE;
        return false;
    }

    // The move is legal (a four-three is legal as well).  What the move created
    // is remembered for the log only, a four or a three is never forbidden.
    BoardView view(board);
    const pair<int, int> idx = LastMoveIdx(board);
    const int i_fours = CountFours(view, idx.first, idx.second, STONE_BLACK);
    set<Shape> sThrees;
    CollectThrees(view, idx.first, idx.second, STONE_BLACK, sThrees);

    if (1 == i_fours)
    {
        qDebug() << "Four";
        this->m_renju_state = PATTERN::FOUR;
    }
    else if (1 == sThrees.size())
    {
        qDebug() << "three";
        this->m_renju_state = PATTERN::THREE;
    }
    else
        this->m_renju_state = PATTERN::ROW;

    return true;
}

int Renju::getForbiddenPatternAt(Board *board, int x, int y)
{
    if (nullptr == board)
        return PATTERN::ROW;

    const pair<int, int> idx(x, y);
    if (board->isPosOutOfBoard(idx) || !board->isPosEmpty(idx))
        return PATTERN::ROW;

    BoardView view(board);
    view.AddStone(x, y, STONE_BLACK);

    // RIF 9.2 forbids a move only "without at the same time attaining five in a
    // row": an exact five wins and is never forbidden.  When a move is played
    // for real checkWin() is asked first and short circuits isLegal(), so the
    // very same test has to be made here for a stone played "in mind".
    bool b_five = false;
    for (int i_dir = 0; (i_dir < 4) && !b_five; ++i_dir)
        b_five = (5 == RunLength(view, x, y, DIRECTIONS[i_dir][0], DIRECTIONS[i_dir][1], STONE_BLACK));

    int i_pattern = PATTERN::ROW;
    if (!b_five)
    {
        if (HasOverLine(view, x, y, STONE_BLACK)) // RIF 9.2.a
            i_pattern = PATTERN::OVERLINE;
        else if (2 <= CountFours(view, x, y, STONE_BLACK)) // RIF 9.2.b
            i_pattern = PATTERN::DOUBLE_FOUR;
        else if (IsForbiddenDoubleThree(view, x, y, STONE_BLACK, 0)) // RIF 9.2.c
            i_pattern = PATTERN::DOUBLE_THREE;
    }

    view.RemoveStone();

    return i_pattern;
}

void Renju::collectForbiddenPoints(Board *board, vector<pair<pair<int, int>, int>> &vPoints)
{
    vPoints.clear();

    if ((nullptr == board) || board->getVRecord().empty())
        return;

    static const int i_dirs[4][2] = {{0, -1}, {-1, 0}, {-1, -1}, {-1, 1}};

    const int i_width = (int)board->getBSize().first;
    const int i_height = (int)board->getBSize().second;
    const vector<pair<int, int>> &vRecord = board->getVRecord();

    set<int> sCandidates;
    for (size_t i = 0; i < vRecord.size(); ++i)
    {
        if (STONE_BLACK != vRecord[i].second)
            continue;

        const pair<int, int> idx = board->coord2idx(vRecord[i].first);
        for (int i_dir = 0; i_dir < 4; ++i_dir)
        {
            for (int i_off = -4; i_off <= 4; ++i_off)
            {
                if (0 == i_off)
                    continue;

                const pair<int, int> p(idx.first + (i_off * i_dirs[i_dir][0]),
                                       idx.second + (i_off * i_dirs[i_dir][1]));
                if ((0 > p.first) || (0 > p.second) || (i_width <= p.first) || (i_height <= p.second))
                    continue;
                if (!board->isPosEmpty(p))
                    continue;

                sCandidates.insert(board->idx2Coord(p));
            }
        }
    }

    for (set<int>::const_iterator it = sCandidates.begin(); it != sCandidates.end(); ++it)
    {
        const pair<int, int> idx = board->coord2idx(*it);
        const int i_pattern = this->getForbiddenPatternAt(board, idx.first, idx.second);
        if (PATTERN::ROW != i_pattern)
            vPoints.push_back(make_pair(idx, i_pattern));
    }
}

int Renju::getRenjuState()
{
    return this->m_renju_state;
}

bool Renju::checkWin(Board *board)
{
    // An empty board has no last move and can never be a win.
    if ((nullptr == board) || board->getVRecord().empty())
        return false;

    const pair<int, int> p_last_move = board->getVRecord().back();
    const pair<int, int> idx = board->coord2idx(p_last_move.first);
    BoardView view(board);

    bool b_five = false;
    bool b_overline = false;
    for (int i_dir = 0; i_dir < 4; ++i_dir)
    {
        const int i_run = RunLength(view, idx.first, idx.second, DIRECTIONS[i_dir][0], DIRECTIONS[i_dir][1], p_last_move.second);
        if (5 == i_run)
            b_five = true;
        else if (6 <= i_run)
            b_overline = true;
    }

    // RIF 9.1: the winner is the player who is first to attain five in a row,
    // for white an overline counts as a win as well.  Black wins with five even
    // when another line became an overline at the same time, because 9.2 only
    // applies "without at the same time attaining five in a row".
    if (b_five || ((STONECOLOR::WHITE == p_last_move.second) && b_overline))
    {
        this->m_renju_state = PATTERN::FIVE_IN_A_ROW;
        return true;
    }

    return false;
}
