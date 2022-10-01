/*************************************************************************
    > File Name: board.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 18:05:28 2022
 ************************************************************************/

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>
#include<iostream>
using namespace std;

enum STONECOLOR
{
    BLACK = 1,
    WHITE = 2
};

class Board
{
public:
    Board();
    ~Board();

    pair<int, int> coord2idx(const int iCoord);
    int idx2Coord(const pair<int, int> idx);

    int BSize;

    //first - coord, second - color.
    vector<pair<int, int>> vRecord;
};

#endif
