#include <iostream>
#include "fifteen.h"
using namespace std;

int main(int ac, char *av[])
{
    int block;
    vector < int > blocks; // ��ʾʮ�������λ��

        //�������״̬��
        cout<<"�����ʾʮ�������ʮ�������֣���λ����0����"<<endl;
        //1 0 3 4 5 2 6 8 9 10 7 12 13 14 11 15
        //1 2 3 4 5 6 0 8 9 10 7 12 13 14 11 15 ��������
        //1 0 3 4 5 2 7 8 9 6 10 11 13 14 15 12 �岽����
        blocks.clear();
        for (int i = 0; i < SQUARES * SQUARES; i++)
        {
            cin >> block;
            blocks.push_back(block);
        }
        // �ж��Ƿ��н⣬�޽��������Ӧ��Ϣ���н���ʹ����Ӧ�㷨���⡣
        if (solvable(blocks))
           {
               solve_puzzle(blocks);//��
           }
        else
            cout << "This puzzle is not solvable." << endl;
    return 0;
}
