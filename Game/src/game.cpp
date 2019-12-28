#include "game.h"
#include <cstring>

Player::Player(int playerNum)
{
    this->playerNum = playerNum;
    moveNum = new int[playerNum];
    for (int i = 0; i < playerNum; i++)
        moveNum[i] = 2;
    isDefaultTable = true;
}

Player::Player(int playerNum, int *moveNum)
{
    this->playerNum = playerNum;
    this->moveNum = moveNum;
    isDefaultTable = false;
}

Player::~Player()
{
    if (isDefaultTable == true)
        delete[] moveNum;
}

int Player::getPlayerNum()
{
    return playerNum;
}

void Player::setmoveNum(int *moveNum)
{
    this->moveNum = moveNum;
    isDefaultTable = false;
}

int *Player::getmoveNum()
{
    return moveNum;
}

Payoff::Payoff()
{
    players = new Player();
    int dim = players->getPlayerNum();
    tableIndex = new int[dim];
    int temp = 1;
    for (int i = 0; i < dim; i++)
    {
        temp *= 2;
        tableIndex[dim - 1 - i] = temp;
    }
    tableSize = dim * temp;
    payoffTable = new double[tableSize];
    memset(payoffTable, 0, tableSize * sizeof(double));
    isDefaultTable = true;
}

Payoff::Payoff(Player *players, double *payoffTable)
{
    int dim = players->getPlayerNum();
    tableIndex = new int[dim];
    this->payoffTable = payoffTable;
    isDefaultTable = false;

    int *dimSize = players->getmoveNum();
    int temp = 1;
    for (int i = dim - 1; i >= 0; i--)
    {
        temp *= dimSize[i];
        tableIndex[i] = temp;
    }
    tableSize = dim * temp;
}

Payoff::~Payoff()
{
    if (isDefaultTable == true)
        delete[] payoffTable;
    delete[] tableIndex;
    delete players;
}

void Payoff::setPayoffTable(double *payoffTable)
{
    if (isDefaultTable == true)
        delete[] this->payoffTable;
    this->payoffTable = payoffTable;
    isDefaultTable = false;
}

void Payoff::setPayoffValue(double payoff, int *index)
{
    int temp = 0;
    int dim = players->getPlayerNum();
    for (int i = 0; i < dim; i++)
        temp += tableIndex[i] * index[i];
    temp += index[dim];
    payoffTable[temp] = payoff;
}

double Payoff::getPayoffValue(int *index)
{
    int temp = 0;
    int dim = players->getPlayerNum();

    for (int i = 0; i < dim; i++)
        temp += tableIndex[i] * index[i];
    temp += index[dim];
    return payoffTable[temp];
}

Strategy::Strategy()
{
    int playerNum = players->getPlayerNum();
    StrategyName *strategyTable = new StrategyName[playerNum];
    playerStrategyFunc = new strategyFunc[playerNum];
    moveTable = new int[playerNum];
    isDefaultTable = true;

    for (int i = 0; i < playerNum; i++)
    {
        moveTable[i] = 1;
        strategyTable[i] = TFT;
    }

    setStrategy(strategyTable);
    delete[] strategyTable;
}

Strategy::Strategy(Player *players, StrategyName *strategyTable)
{
    int playerNum = players->getPlayerNum();
    playerStrategyFunc = new strategyFunc[playerNum];
    isDefaultTable = false;
    for (int i = 0; i < playerNum; i++)
        moveTable[i] = 1;
    setStrategy(strategyTable);
}

Strategy::~Strategy()
{
    delete[] playerStrategyFunc;
    if (isDefaultTable == true)
        delete[] moveTable;
    delete players;
}

int Strategy::stay(int playerIndex)
{
    return moveTable[playerIndex];
}

int Strategy::grim(int playerIndex)
{
    int playerNum = players->getPlayerNum();
    if (moveTable[playerIndex] == 1)
    {
        for (int i = 1; i < playerNum; i++)
            if (moveTable[(playerIndex + i) % playerNum] == 0)
                return 0;
        return 1;
    }
    else
        return 0;
}

int Strategy::tft(int playerIndex)
{
    int playerNum = players->getPlayerNum();
    for (int i = 1; i < playerNum; i++)
    {
        if (moveTable[(playerIndex + i) % playerNum] == 0)
            return 0;
        else
            return 1;
    }
    return 1;
}

void Strategy::setMoveTable(int *moveTable)
{
    this->moveTable = moveTable;
}

void Strategy::setStrategy(StrategyName *strategyTable)
{
    int playerNum = players->getPlayerNum();
    for (int i = 0; i < playerNum; i++)
    {
        switch (strategyTable[i])
        {
        case STAY:
            playerStrategyFunc[i] = stay;
            break;

        case GRIM:
            playerStrategyFunc[i] = grim;
            break;

        case TFT:
            playerStrategyFunc[i] = tft;
            break;

        default:
            playerStrategyFunc[i] = stay;
            break;
        }
    }
}

typedef int (Strategy::*strategyFunc)(int);
strategyFunc Strategy::getStrategy(int playerIndex)
{
    return playerStrategyFunc[playerIndex];
}

Game::Game()
{
    players = new Player();
    payoffs = new Payoff();
    strategies = new Strategy();
    isDefault = true;
    moveTableTemp = new int[players->getPlayerNum()];
}

Game::Game(Player *players, Payoff *payoffs, Strategy *strategiesstrategies)
{
    this->players = players;
    this->payoffs = payoffs;
    this->strategies = strategies;
    isDefault = false;

    moveTableTemp = new int[players->getPlayerNum()];
}

Game::~Game()
{
    if (isDefault == true)
    {
        delete players;
        delete payoffs;
        delete strategies;
    }
    delete[] moveTableTemp;
}

void Game::repeatedPlay(int times)
{
    strategyFunc pf;
    for (int i = 0; i < times; i++)
    {
        for (int j = 0; j < players->getPlayerNum(); j++)
        {
            pf = strategies->getStrategy(j);
            moveTableTemp[j] = (strategies->*pf)(j);
        }

        ;
    }
}