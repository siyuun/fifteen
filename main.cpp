#include <iostream>
#include "fifteen.h"
using namespace std;

int main(int ac, char *av[])
{
    int block;
    vector < int > blocks; // 表示十五数码的位置

        //读入局面状态。
        cout<<"输入表示十五数码的十六个数字，空位置用0代替"<<endl;
        //1 0 3 4 5 2 6 8 9 10 7 12 13 14 11 15
        //1 2 3 4 5 6 0 8 9 10 7 12 13 14 11 15 三步操作
        //1 0 3 4 5 2 7 8 9 6 10 11 13 14 15 12 五步操作
        blocks.clear();
        for (int i = 0; i < SQUARES * SQUARES; i++)
        {
            cin >> block;
            blocks.push_back(block);
        }
        // 判断是否有解，无解则输出相应信息，有解则使用相应算法解题。
        if (solvable(blocks))
           {
               solve_puzzle(blocks);//解
           }
        else
            cout << "This puzzle is not solvable." << endl;
    return 0;
}
