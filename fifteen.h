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
#define BASE 16         // ������ת��Ϊ�����Ļ���
#define DEPTHBOUND 10       // ʹ�������������ʱ������������
#define STEPSBOUND 10       // �����󳤶�


struct node
{
    vector < int > state;     // ��ʾ��ǰ����״̬
    int moves[STEPSBOUND];      // �ӳ�ʼ״̬����Ŀǰ״̬���߷�����
    int depth;          // ��ǰ���
    int score;          // ��ǰ�ڵ������
    int blank;          // �ո�λ��
};
bool operator<(node x, node y);
// �жϸ��������Ƿ�ɽ⡣
bool solvable(vector < int > tiles);
// �õ���ǰ����ĺ���߷���MOVE_LEFT = �����ƶ��ջ��飬MOVE_RIGHT = �����ƶ��ջ��飬
// MOVE_UP = �����ƶ��ջ��飬MOVE_DOWN = �����ƶ��ջ��顣
void valid_moves(node &current);
// ������״̬ת��Ϊһ��������
unsigned long long key(vector < int > &tiles);
// �Ӿ��� current ִ�� move ��ָ�����߷���
node execute(node &current, int move);
int score(vector < int > &state, int depth);
// �ж��Ƿ��ѴﵽĿ��״̬��
bool solved(vector < int > &state);
// �ҵ�����״̬�Ŀջ���λ�á�
int find_blank(vector < int > &state);
//�����������
bool depth_first_search(vector < int > initial, int directions[]);
//�����������
bool breadth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed);
//����ʽ����
bool heuristic_search(vector < int > initial, int directions[],int& m_open,int& m_closed);
void solve_puzzle(vector < int > state);
int absi(int x);
#endif // FIFTEEN_H_INCLUDED
