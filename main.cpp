#include <iostream>
#include "game.h"

int fun(int a, int b)
{
    return a + b;
}

int main()
{
    int dim = 2;
    int dimSize[2] = {2, 4};
    double payoffTable[16] = {1, 5, 2, 7, 8, 6, 1, 4, 2, 8, 7, 5, 3, 10, 1, 2};
    Player *players = new Player(2, dimSize);
    Payoff a(players, payoffTable);
    Strategy *b = new Strategy();

    for (int u = 0; u < dim; u++)
    {
        for (int i = 0; i < dimSize[0]; i++)
        {
            for (int j = 0; j < dimSize[1]; j++)
            {
                int index[3] = {u, i, j};
                std::cout << a.getPayoffValue(index) << "       ";
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------" << std::endl;
    }
    return 0;
}
