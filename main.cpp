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
    Payoff a(dim, dimSize, payoffTable);
    Strategy b;

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

    // int (*pf[10])(int, int);
    int l = 10;
    typedef int *(*pf)(int);
    pf *p = new pf[100];
    // int (**pf)(int) = new (int (*[l])(int));
    // int **p = new int *[10];

    return 0;
}
