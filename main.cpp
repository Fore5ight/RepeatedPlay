#include <iostream>
#include "game.h"

int main()
{
    //初始化参与人类
    int playerNum = 2;       //参与人数为2
    int moveNum[2] = {2, 2}; //每个参与人包含2种行动
    Player *players = new Player(playerNum, moveNum);

    //初始化支付类
    //支付表设置，先输入第一人的支付，按先行后列的顺序。输入完后再按前述方式输入第二人的支付
    double payoffTable[8] = {1, 5, 0, 3, 1, 0, 5, 3};
    Payoff *payoffs = new Payoff(players, payoffTable);

    //初始化策略类
    Strategy *strategies = new Strategy(players);

    //初始化博弈类
    Game *game = new Game(players, payoffs, strategies);

    const int times = 200;                                                       //重复博弈次数
    StrategyName strategyTable[3][2] = {{STAY, GRIM}, {STAY, TFT}, {GRIM, TFT}}; //3种策略两两之间比较
    int moveTable[2] = {0, 1};                                                   //0表示背叛，1表示合作。2名参与人的初始行动

    //开始重复博弈
    for (int str = 0; str < 3; str++)
    {
        if (str == 2)
            moveTable[0] = 1;
        else
            moveTable[0] = 0;
        strategies->setStrategy(strategyTable[str]); //设置策略
        game->repeatedPlay(times, moveTable);        //设置博弈次数与初始行动
        
        for (int no = 0; no < playerNum; no++)
        {
            std::cout << strategyTable[str][no] << ": " << game->getFinalPayoff()[no] << " ";
        }
        std::cout << std::endl;
    }

    delete game;
    delete strategies;
    delete payoffs;
    delete players;

    return 0;
}
