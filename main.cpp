#include <iostream>
#include "game.h"

//用于将策略名称转换为可打印输出的字符串
std::string nameString(StrategyName strategyName)
{
    std::string name;
    switch (strategyName)
    {
    case STAY:
        name = "STAY";
        break;

    case GRIM:
        name = "GRIM";
        break;

    case TFT:
        name = "TFT";
        break;

    default:
        name = "ERROR";
        break;
    }
    return name;
}

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

    const int times = 200;                                                                                                  //重复博弈次数
    const int strNum = 6;                                                                                                   //实验策略种类数
    StrategyName strategyTable[strNum][2] = {{STAY, GRIM}, {STAY, TFT}, {STAY, TFT}, {GRIM, TFT}, {GRIM, TFT}, {TFT, TFT}}; //3种策略两两之间比较
    int moveTable[strNum][2] = {{0, 1}, {0, 1}, {0, 0}, {1, 1}, {1, 0}, {1, 0}};                                            //0表示背叛，1表示合作。2名参与人的初始行动

    //开始重复博弈
    for (int str = 0; str < strNum; str++)
    {
        strategies->setStrategy(strategyTable[str]); //设置策略
        game->repeatedPlay(times, moveTable[str]);   //设置博弈次数与初始行动，开始重复博弈

        for (int no = 0; no < playerNum; no++)
        {
            //打印博弈结果
            std::cout << nameString(strategyTable[str][no]) << "_" << moveTable[str][no] << ": " << game->getFinalPayoff()[no] << " ";
        }
        std::cout << std::endl;
    }

    delete game;
    delete strategies;
    delete payoffs;
    delete players;

    return 0;
}
