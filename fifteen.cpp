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
#define SQUARES 4       // ��������������
#define BASE 16         // ������ת��Ϊ�����Ļ�����
#define DEPTHBOUND 10      // ʹ�������������ʱ�����������ȡ�
#define STEPSBOUND 30       // �����󳤶ȡ�

#define MOVE_LEFT (-1)
#define MOVE_RIGHT 1
#define MOVE_UP (-SQUARES)
#define MOVE_DOWN SQUARES
#define MOVE_NONE 0

int nextmove[SQUARES];  // �ƶ�����Ϊ4

// ����ʽ������Ҫ�Խڵ����ֽ��бȽ�
bool operator<(node x, node y)
{
    return x.score > y.score;
}

// �����ֵ
int absi(int x)
{
    return x>=0?x:(-x);
}

// �жϸ��������Ƿ�ɽ�
bool solvable(vector < int > state)
{
    int sum = 0, row;
    for (int i = 0; i < state.size(); i++)
    {
        int tile = state[i];
        if (tile == 0)
        {
            row = (i/SQUARES + 1);//�ո��ڵڼ���
            continue;
        }
        for (int m = i; m < state.size(); m++)
            if (state[m] < tile && state[m] != 0)sum++;//������++
    }
    return !((sum + row) % 2);//��Ϊż���н� �����޽�
}

// �ж��Ƿ�ﵽĿ��״̬
bool solved(vector < int > &state)
{
    if (state[SQUARES * SQUARES - 1] != 0)
        return false;
    for (int i = 0; i < SQUARES * SQUARES - 1; i++)
        if (state[i] != (i + 1))
            return false;
    return true;
}


//����ʽ�����еĹ��ۺ���
int score(vector < int > &state, int depth)
{
    int errnum = 0;
    for (int i = 0; i < state.size()-1; i++)
        if (state[i] != i+1)
            errnum++;
    if(state[state.size()-1] != 0) errnum++;
    return (depth + errnum);
}

//�õ���ǰ����ĺ���߷�
//MOVE_LEFT = �����ƶ��ջ���
//MOVE_RIGHT = �����ƶ��ջ���
//MOVE_UP = �����ƶ��ջ���
//MOVE_DOWN = �����ƶ��ջ��顣
void valid_moves(node &current)
{
    //��ȡ����߷�������ȥ�˻ص���״̬����һ�����߷���
    int last_move = MOVE_NONE;
    if (current.depth)
        last_move = current.moves[current.depth - 1];
    memset(nextmove, MOVE_NONE, sizeof(nextmove));
     if (current.blank % SQUARES > 0 && last_move != MOVE_RIGHT)//�ջ���������� ��һ�������� ����������
        nextmove[0] = MOVE_LEFT;
    if (current.blank % SQUARES < (SQUARES - 1) && last_move != MOVE_LEFT)//ͬ��
        nextmove[1] = MOVE_RIGHT;
    if (current.blank / SQUARES > 0 && last_move != MOVE_DOWN)//ͬ��
        nextmove[2] = MOVE_UP;
    if (current.blank / SQUARES < (SQUARES - 1) && last_move != MOVE_UP)//ͬ��
        nextmove[3] = MOVE_DOWN;
}

// ������״̬ת��Ϊһ������ ���������Ψһ�ģ�
unsigned long long key(vector < int > &state)
{
    unsigned long long key = 0;
    for (int i = 0; i < state.size(); i++)
        key = key * BASE + state[i];
    return key;
}

// �Ӿ���currentִ�� ����nextmove��ָ�����������ҵ��߷���
node execute(node &current, int move)
{
    node nextstep;

    // �߷������趨��
    memcpy(nextstep.moves, current.moves, sizeof(current.moves));
    nextstep.depth = current.depth + 1;
    nextstep.moves[current.depth] = move;

    // ����״̬�趨
    nextstep.state = current.state;
    nextstep.blank = current.blank + move;
    nextstep.state[current.blank] =nextstep.state[nextstep.blank];//�� move ָ�������ƶ�
    nextstep.state[nextstep.blank] = 0;//�����ջ���λ��
    return nextstep;
}


//�ҵ�����״̬�Ŀջ��顮0����λ��
int find_blank(vector <int> &state)
{
    for (int i = 0; i < SQUARES * SQUARES; i++)
        if (state[i] == 0)
            return i;
}

//�����������
bool depth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));
    // ��⵱ǰ�����Ƿ�Ϊ�ѽ��״̬
    if (solved(copy.state))//�ǾͰ��߷�����
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }
    // ����ʼ״̬���뿪�ż��С�
    stack <node> open;  // �����������ʹ��ջ���洢�ڵ�
    open.push(copy);
    m_open++;
    // �պϼ���
    set <unsigned long long> closed;
    while (!open.empty())
    {
        // �����ż��еľ��棬�����þ������պϼ��С�
        node current = open.top();
        open.pop();
        closed.insert(key(current.state));
        m_closed++;//close��++
        valid_moves(current);// ��ȡ�þ���ĺ���߷�������߷�������뵽open��
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current,nextmove[i]);// �ڵ�ǰ������ִ���߷�
            if(closed.find(key(nextstep.state))!=closed.end())// ����Ѿ����ʣ���������һ���߷�
                continue;
            // ��¼�����ǰһ���߷�������ҵ��⣬���߷����룬�����˳������ص�moves[]��
            if (solved(nextstep.state))
            {
                memcpy(directions,nextstep.moves,sizeof(nextstep.moves));
                return true;
            }
            // ����Ļ��������open��������ȣ�
            if (nextstep.depth < DEPTHBOUND)
            {
               open.push(nextstep);
                m_open++;
            }
        }
    }
    return false;
}

//�����������
bool breadth_first_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));

    // ��⵱ǰ�����Ƿ�Ϊ�ѽ��״̬
    if (solved(copy.state))
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }

    // ����ʼ״̬���뿪�ż�open��
    queue <node> open;  // �����������ʹ�ö��д洢���ż� �Ƚ��ȳ�
    open.push(copy);
    m_open++;

    // �պϼ�set
    set < unsigned long long > closed;
    while (!open.empty())
    {
        // �����ż��еľ��棬�����þ������closed��
        node current = open.front();
        open.pop();
        closed.insert(key(current.state));
        m_closed++;
        valid_moves(current);// ��ȡ�þ���ĺ���߷�������߷��������open��
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current, nextmove[i]);// �ڵ�ǰ������ִ���߷�
            if (closed.find(key(nextstep.state)) != closed.end())
                continue;// ����Ѿ����ʣ���������һ���߷� ���ַ������ӿ��

            // ��¼�����ǰһ���߷�������ҵ��⣬��ô�����˳���
            if (solved(nextstep.state))
            {
                memcpy(directions, nextstep.moves,sizeof(nextstep.moves));
                return true;
            }
            open.push(nextstep); // ����ǰ�������open
            m_open++;
        }
    }
    return false;
}

//����ʽ����
bool heuristic_search(vector < int > initial, int directions[],int& m_open,int& m_closed)
{
    node copy;
    copy.state = initial;
    copy.depth = 0;
    copy.blank = find_blank(initial);
    copy.score = score(copy.state, 0);
    memset(copy.moves, MOVE_NONE, sizeof(copy.moves));

    // ��⵱ǰ�����Ƿ�Ϊ�ѽ��״̬
    if (solved(copy.state))
    {
        memcpy(directions, copy.moves, sizeof(copy.moves));
        return true;
    }
    priority_queue <node> open; //����ʽ����ʹ�����ȶ��д洢open ���Զ�״̬���ֽ��ж�̬���� Ҫ���رȽϷ���
    open.push(copy);
    m_open++;
    map < unsigned long long, int > closed;//�ñ����洢closed ����ֵ��Ӧ״̬����
    while (!open.empty())
    {
        // ɾ������ֵ��С�Ľڵ㣬���Ϊ�ѷ��ʹ�
        node current = open.top();
        open.pop();
        // ��״̬����ֵ��״̬���ִ���պϼ���
        closed.insert(std::make_pair< unsigned long long, int&>(key(current.state), current.score));
        m_closed++;
        if (solved(current.state))// �����Ŀ��״̬������
        {
            memcpy(directions, current.moves,
                sizeof(current.moves));
            return true;
        }
        valid_moves(current);// �������߷�������nextmove
        for (int i = 0; i < SQUARES; i++)
        {
            if (nextmove[i] == MOVE_NONE)
                continue;
            node nextstep = execute(current, nextmove[i]);// �ƶ�����
            nextstep.score = score(nextstep.state, nextstep.depth);// ��������ʽ�����Ե�ǰ״̬����

            // �����ǰ״̬�Ѿ����ʹ����鿴�Ƿ��ܹ��Ը�С�Ĵ��۴ﵽ��״̬�����û�У�����������ӱպϼ������������
            map <unsigned long long,int>::iterator it =closed.find(key(nextstep.state));

            if (it != closed.end())
            {
                if (nextstep.score >= (*it).second)
                    continue;
                closed.erase(it);
            }
            open.push(nextstep);// ����ǰ������뿪�ż��С�
            m_open++;
        }
    }
    return false;
}


void solve_puzzle(vector < int > initial)
{
    LARGE_INTEGER iLarge;//����CPU����ʱ��
    QueryPerformanceFrequency( &iLarge );
    double dbFreq = (double) iLarge.QuadPart;
    int moves[STEPSBOUND];
    bool flag=true;
    while(flag){
        cout<<"----��ѡ�������㷨----"<<endl<<"1.�����������"<<endl;
        cout<<"2.�����������"<<endl;
        cout<<"3.����ʽ����"<<endl;
        cout<<"0.�˳�"<<endl;
        int way;
        cin>>way;
        QueryPerformanceCounter( &iLarge );
        double dbBegin = (double) iLarge.QuadPart;//����ǰ
        int m_closed;
        int m_open=m_closed=0; //������Ϊ0
        switch (way){
            case 1: depth_first_search(initial, moves,m_open,m_closed); break; // �����������
            case 2: breadth_first_search(initial, moves,m_open,m_closed); break;//�����������
            case 3: heuristic_search(initial, moves,m_open,m_closed); break;// ����ʽ����
            case 0: flag=false; break;//����ѭ��
        }
        // ����߷����衣
        QueryPerformanceCounter( &iLarge );//���к�
        double dbEnd = (double) iLarge.QuadPart;
        double nms = (( dbEnd - dbBegin ) * 1000.0 / dbFreq );//���к�
        int i;
        if(flag)
        {
            for (i = 0; i < STEPSBOUND; i++)
            {
                if (moves[i] == MOVE_NONE)
                    break;
                switch (moves[i])
                {
                    case MOVE_LEFT:cout << "�� ";break;
                    case MOVE_RIGHT:cout << "�� ";break;
                    case MOVE_UP:cout << "�� ";break;
                    case MOVE_DOWN:cout << "�� ";break;
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

