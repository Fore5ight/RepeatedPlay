#include <iostream>
#include "game.h"

int main()
{
    int playerNum = 2;
    int moveNum[2] = {2, 2};
    double payoffTable[8] = {1, 5, 0, 3, 1, 0, 5, 3};
    StrategyName strategyTable[2] = {STAY, TFT};
    int moveTable[2] = {1, 0};

    Player *players = new Player(playerNum, moveNum);
    Payoff *payoffs = new Payoff(players, payoffTable);
    Strategy *strategies = new Strategy(players, strategyTable);
    strategies->setMoveTable(moveTable);

    Game *game = new Game(players, payoffs, strategies);

    int times = 200;
    game->repeatedPlay(times);

    for (int i = 0; i < playerNum; i++)
    {
        std::cout << game->getFinalPayoff()[i] << std::endl;
    }

    delete game;
    delete strategies;
    delete payoffs;
    delete players;

    return 0;
}
