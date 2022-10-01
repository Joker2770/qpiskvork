/*************************************************************************
    > File Name: board.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: Fri Sep 30 18:05:37 2022
 ************************************************************************/

#include "board.h"
#include<iostream>
using namespace std;

Board::Board()
{
    this->vRecord.clear();
}

Board::~Board()
{

}

pair<int, int> Board::coord2idx(int i_coord)
{
    pair<int, int> idx;
    idx.first = i_coord % this->BSize;
    idx.second = i_coord / this->BSize;
    return idx;
}

int Board::idx2Coord(const pair<int, int> idx)
{
    int i_coord = 0;
    i_coord = idx.second * this->BSize + idx.first;
    return i_coord;
}
