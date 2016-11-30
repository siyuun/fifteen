#include "fifteen.h"

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
#define SQUARES 4       // 局面横向格子数。
#define BASE 16         // 将局面转换为整数的基数。
#define DEPTHBOUND 10      // 使用深度优先搜索时的最大搜索深度。
#define STEPSBOUND 30       // 解的最大长度。

#define MOVE_LEFT (-1)
#define MOVE_RIGHT 1
#define MOVE_UP (-SQUARES)
#define MOVE_DOWN SQUARES
#define MOVE_NONE 0

int nextmove[SQUARES];  // 移动步骤为4

// 启发式搜索中要对节点评分进行比较
bool operator<(node x, node y)
{
    return x.score > y.score;
}

// 求绝对值
int absi(int x)
{
    return x>=0?x:(-x);
}

// 判断给定局面是否可解
bool solvable(vector < int > state)
{
    int sum = 0, row;
    for (int i = 0; i < state.size(); i++)
    {
        int tile = state[i];
        if (tile == 0)
        {
            row = (i/SQUARES + 1);//空格在第几行
            continue;
        }
        for (int m = i; m < state.size(); m++)
            if (state[m] < tile && state[m] != 0)sum++;//逆序数++
    }
    return !((sum + row) % 2);//和为偶数有解 奇数无解
}

// 判断是否达到目标状态
bool solved(vector < int > &state)
{
    if (state[SQUARES * SQUARES - 1] != 0)
        return false;
    for (int i = 0; i < SQUARES * SQUARES - 1; i++)
        if (state[i] != (i + 1))
            return false;
    return true;
}


//启发式搜索中的估价函数
int score(vector < int > &state, int depth)
{
    int errnum = 0;
    for (int i = 0; i < state.size()-1; i++)
        if (state[i] != i+1)
            errnum++;
    if(state[state.size()-1] != 0) errnum++;
    return (depth + errnum);
}

//得到当前局面的后继走法
//MOVE_LEFT = 向左移动空滑块
//MOVE_RIGHT = 向右移动空滑块
//MOVE_UP = 向上移动空滑块
//MOVE_DOWN = 向下移动空滑块。
void valid_moves(node &current)
{
    //获取后继走法，但除去退回到该状态的上一步的走法。
    int last_move = MOVE_NONE;
    if (current.depth)
        last_move = current.moves[current.depth - 1];
    memset(nextmove, MOVE_NONE, sizeof(nextmove));
     if (current.blank % SQUARES > 0 && last_move != MOVE_RIGHT)//空滑块在最左边 上一步是往右 不能走向左
        nextmove[0] = MOVE_LEFT;
    if (current.blank % SQUARES < (SQUARES - 1) && last_move != MOVE_LEFT)//同理
        nextmove[1] = MOVE_RIGHT;
    if (current.blank / SQUARES > 0 && last_move != MOVE_DOWN)//同理
        nextmove[2] = MOVE_UP;
    if (current.blank / SQUARES < (SQUARES - 1) && last_move != MOVE_UP)//同理
        nextmove[3] = MOVE_DOWN;
}

// 将棋面状态转换为一个整数 这个整数是唯一的？
unsigned long long key(vector < int > &state)
{
    unsigned long long key = 0;
    for (int i = 0; i < state.size(); i++)
        key = key * BASE + state[i];
    return key;
}

// 从局面current执行 返回nextmove所指定的上下左右的走法。
node execute(node &current, int move)
{
    node nextstep;

    // 走法步骤设定。
    memcpy(nextstep.moves, current.moves, sizeof(current.moves));
    nextstep.depth = current.depth + 1;
    nextstep.moves[current.depth] = move;

    // 局面状态设定
    nextstep.state = current.state;
    nextstep.blank = current.blank + move;
    nextstep.state[current.blank] =nextstep.state[nextstep.blank];//按 move 指定方向移动
    nextstep.state[nextstep.blank] = 0;//交换空滑块位置
    return nextstep;
}


//找到局面状态的空滑块‘0’的位置
int find_blank(vector <int> &state)
{
    for (int i = 0; i < SQUARES * SQUARES; i++)
        if (state[i] == 0)
            return i;
}

//深度优先搜索
bool depth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));
    // 检测当前局面是否为已解决状态
    if (solved(copy.state))//是就把走法返回
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }
    // 将初始状态放入开放集中。
    stack <node> open;  // 深度优先搜索使用栈来存储节点
    open.push(copy);
    m_open++;
    // 闭合集。
    set <unsigned long long> closed;
    while (!open.empty())
    {
        // 处理开放集中的局面，并将该局面放入闭合集中。
        node current = open.top();
        open.pop();
        closed.insert(key(current.state));
        m_closed++;//close表++
        valid_moves(current);// 获取该局面的后继走法，后继走法都会加入到open中
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current,nextmove[i]);// 在当前局面上执行走法
            if(closed.find(key(nextstep.state))!=closed.end())// 如果已经访问，尝试另外一种走法
                continue;
            // 记录求解中前一步走法，如果找到解，把走法存入，立即退出，返回到moves[]中
            if (solved(nextstep.state))
            {
                memcpy(directions,nextstep.moves,sizeof(nextstep.moves));
                return true;
            }
            // 否则的话将其加入open（限制深度）
            if (nextstep.depth < DEPTHBOUND)
            {
               open.push(nextstep);
                m_open++;
            }
        }
    }
    return false;
}

//广度优先搜索
bool breadth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));

    // 检测当前局面是否为已解决状态
    if (solved(copy.state))
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }

    // 将初始状态放入开放集open中
    queue <node> open;  // 广度优先搜索使用队列存储开放集 先进先出
    open.push(copy);
    m_open++;

    // 闭合集set
    set < unsigned long long > closed;
    while (!open.empty())
    {
        // 处理开放集中的局面，并将该局面放入closed中
        node current = open.front();
        open.pop();
        closed.insert(key(current.state));
        m_closed++;
        valid_moves(current);// 获取该局面的后继走法，后继走法都会加入open中
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current, nextmove[i]);// 在当前局面上执行走法
            if (closed.find(key(nextstep.state)) != closed.end())
                continue;// 如果已经访问，尝试另外一种走法 这种方法更加快捷

            // 记录求解中前一步走法，如果找到解，那么立即退出。
            if (solved(nextstep.state))
            {
                memcpy(directions, nextstep.moves,sizeof(nextstep.moves));
                return true;
            }
            open.push(nextstep); // 将当前局面放入open
            m_open++;
        }
    }
    return false;
}

//启发式搜索
bool heuristic_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    copy.score = score(copy.state, 0);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));

    // 检测当前局面是否为已解决状态
    if (solved(copy.state))
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }
    priority_queue <node> open; //启发式搜索使用优先队列存储open 可以对状态评分进行动态排序 要重载比较符号
    open.push(copy);
    m_open++;
    map < unsigned long long, int > closed;//用表来存储closed 特征值对应状态评分
    while (!open.empty())
    {
        // 删除评价值最小的节点，标记为已访问过
        node current = open.top();
        open.pop();
        // 将状态特征值和状态评分存入闭合集中
        closed.insert(std::make_pair< unsigned long long, int&>(key(current.state), current.score));
        m_closed++;
        if (solved(current.state))// 如果是目标状态，返回
        {
            memcpy(directions, current.moves,
                sizeof(current.moves));
            return true;
        }
        valid_moves(current);// 计算后继走法，存入nextmove
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current, nextmove[i]);// 移动滑块
            nextstep.score = score(nextstep.state, nextstep.depth);// 根据启发式函数对当前状态评分

            // 如果当前状态已经访问过，查看是否能够以更小的代价达到此状态，如果没有，继续，否则从闭合集中提出并处理。
            map <unsigned long long,int>::iterator it =closed.find(key(nextstep.state));

            if (it != closed.end())
            {
                if (nextstep.score >= (*it).second)
                    continue;
                closed.erase(it);
            }
            open.push(nextstep);// 将当前局面放入开放集中。
            m_open++;
        }
    }
    return false;
}


void solve_puzzle(vector < int > initial)
{
    LARGE_INTEGER iLarge;//计算CPU运行时间
    QueryPerformanceFrequency( &iLarge );
    double dbFreq = (double) iLarge.QuadPart;
    int moves[STEPSBOUND];
    bool flag=true;
    while(flag){
        cout<<"----请选择搜索算法----"<<endl<<"1.深度优先搜索"<<endl;
        cout<<"2.宽度优先搜索"<<endl;
        cout<<"3.启发式搜索"<<endl;
        cout<<"0.退出"<<endl;
        int way;
        cin>>way;
        QueryPerformanceCounter( &iLarge );
        double dbBegin = (double) iLarge.QuadPart;//运行前
        int m_closed;
        int m_open=m_closed=0; //重新置为0
        switch (way){
            case 1: depth_first_search(initial, moves,m_open,m_closed); break; // 深度优先搜索
            case 2: breadth_first_search(initial, moves,m_open,m_closed); break;//宽度优先搜索
            case 3: heuristic_search(initial, moves,m_open,m_closed); break;// 启发式搜索
            case 0: flag=false; break;//结束循环
        }
        // 输出走法步骤。
        QueryPerformanceCounter( &iLarge );//运行后
        double dbEnd = (double) iLarge.QuadPart;
        double nms = (( dbEnd - dbBegin ) * 1000.0 / dbFreq );//运行后
        int i;
        if(flag)
        {
            for (i = 0; i < STEPSBOUND; i++)
            {
                if (moves[i] == MOVE_NONE)
                    break;
                switch (moves[i])
                {
                    case MOVE_LEFT:cout << "左 ";break;
                    case MOVE_RIGHT:cout << "右 ";break;
                    case MOVE_UP:cout << "上 ";break;
                    case MOVE_DOWN:cout << "下 ";break;
                }
            }
            cout << endl;
            cout<<"This puzzle is solved in "<<i<<" steps."<<endl;
            cout<<"open  : "<<m_open<<endl;
            cout<<"closed: "<<m_closed<<endl;
            cout<<"This puzzle is solved in "<<nms<<" ms."<<endl;
            cout<<endl;
            }
        }
}

