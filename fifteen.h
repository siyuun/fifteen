#ifndef FIFTEEN_H_INCLUDED
#define FIFTEEN_H_INCLUDED

#include <iostream>
#include <cstring>
#include <limits>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <stdio.h>
#include <windows.h>

using namespace std;

#define MOVE_LEFT (-1)
#define MOVE_RIGHT 1
#define MOVE_UP (-SQUARES)
#define MOVE_DOWN SQUARES
#define MOVE_NONE 0
#define SQUARES 4       // 4*4
#define BASE 16         // 将局面转换为整数的基数
#define DEPTHBOUND 10       // 使用深度优先搜索时的最大搜索深度
#define STEPSBOUND 10       // 解的最大长度


struct node
{
    vector < int > state;     // 表示当前棋盘状态
    int moves[STEPSBOUND];      // 从初始状态到达目前状态的走法序列
    int depth;          // 当前深度
    int score;          // 当前节点的评分
    int blank;          // 空格位置
};
bool operator<(node x, node y);
// 判断给定局面是否可解。
bool solvable(vector < int > tiles);
// 得到当前局面的后继走法。MOVE_LEFT = 向左移动空滑块，MOVE_RIGHT = 向右移动空滑块，
// MOVE_UP = 向上移动空滑块，MOVE_DOWN = 向下移动空滑块。
void valid_moves(node &current);
// 将棋面状态转换为一个整数。
unsigned long long key(vector < int > &tiles);
// 从局面 current 执行 move 所指定的走法。
node execute(node &current, int move);
int score(vector < int > &state, int depth);
// 判断是否已达到目标状态。
bool solved(vector < int > &state);
// 找到局面状态的空滑块位置。
int find_blank(vector < int > &state);
//深度优先搜索
bool depth_first_search(vector < int > initial, int directions[]);
//广度优先搜索
bool breadth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed);
//启发式搜索
bool heuristic_search(vector < int > initial, int directions[],int& m_open,int& m_closed);
void solve_puzzle(vector < int > state);
int absi(int x);
#endif // FIFTEEN_H_INCLUDED
