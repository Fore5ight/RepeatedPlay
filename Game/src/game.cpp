#include "game.h"
#include <cstring>

Player::Player(int playerNum)
{
    this->playerNum = playerNum;
    this->moveNum = new int[playerNum];
    for (int i = 0; i < playerNum; i++)
        this->moveNum[i] = 2;
}

Player::Player(int playerNum, int *moveNum)
{
    this->playerNum = playerNum;
    this->moveNum = new int[playerNum];
    memcpy(this->moveNum, moveNum, playerNum * sizeof(int));
}

Player::~Player()
{
    delete[] moveNum;
}

int Player::getPlayerNum()
{
    return playerNum;
}

void Player::setmoveNum(int *moveNum)
{
    memcpy(this->moveNum, moveNum, playerNum * sizeof(int));
}

int *Player::getmoveNum()
{
    return moveNum;
}

Payoff::Payoff(Player *players)
{
    this->players = players;
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
}

Payoff::Payoff(Player *players, double *payoffTable)
{
    this->players = players;
    int dim = players->getPlayerNum();
    tableIndex = new int[dim];

    int *dimSize = players->getmoveNum();
    int temp = 1;
    for (int i = dim - 1; i >= 0; i--)
    {
        temp *= dimSize[i];
        tableIndex[i] = temp;
    }
    tableSize = dim * temp;
    this->payoffTable = new double[tableSize];
    memcpy(this->payoffTable, payoffTable, tableSize * sizeof(double));
}

Payoff::~Payoff()
{
    delete[] payoffTable;
    delete[] tableIndex;
}

void Payoff::setPayoffTable(double *payoffTable)
{
    memcpy(this->payoffTable, payoffTable, tableSize * sizeof(double));
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

Strategy::Strategy(Player *players)
{
    this->players = players;
    int playerNum = players->getPlayerNum();
    StrategyName *strategyTable = new StrategyName[playerNum];
    playerStrategyFunc = new strategyFunc[playerNum];
    moveTable = new int[playerNum];

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
    this->players = players;
    int playerNum = players->getPlayerNum();
    playerStrategyFunc = new strategyFunc[playerNum];
    moveTable = new int[playerNum];

    for (int i = 0; i < playerNum; i++)
        moveTable[i] = 1;
    setStrategy(strategyTable);
}

Strategy::~Strategy()
{
    delete[] playerStrategyFunc;
    delete[] moveTable;
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
    memcpy(this->moveTable, moveTable, (players->getPlayerNum()) * sizeof(int));
}

int *Strategy::getMoveTable()
{
    return this->moveTable;
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
    payoffs = new Payoff(players);
    strategies = new Strategy(players);
    isDefault = true;

    payoffIndex = new int[players->getPlayerNum() + 1];
    finalPayoff = new double[players->getPlayerNum()];
    memset(finalPayoff, 0, (players->getPlayerNum()) * sizeof(double));
}

Game::Game(Player *players, Payoff *payoffs, Strategy *strategies)
{
    this->players = players;
    this->payoffs = payoffs;
    this->strategies = strategies;
    isDefault = false;

    payoffIndex = new int[players->getPlayerNum() + 1];
    // memcpy(payoffIndex + 1, strategies->getMoveTable(), (players->getPlayerNum()) * sizeof(int));
    finalPayoff = new double[players->getPlayerNum()];
}

Game::~Game()
{
    if (isDefault == true)
    {
        delete payoffs;
        delete strategies;
        delete players;
    }
    delete[] payoffIndex;
    delete[] finalPayoff;
}

void Game::repeatedPlay(int times, int *initMove)
{
    strategyFunc pf;
    int playerNum = players->getPlayerNum();
    memcpy(strategies->getMoveTable(), initMove, playerNum * sizeof(int));
    memcpy(payoffIndex + 1, initMove, playerNum * sizeof(int));
    memset(finalPayoff, 0, (players->getPlayerNum()) * sizeof(double));
    for (int i = 0; i < times; i++)
    {
        for (int k = 0; k < playerNum; k++)
        {
            payoffIndex[0] = k;
            finalPayoff[k] += payoffs->getPayoffValue(payoffIndex);
        }
        for (int j = 0; j < playerNum; j++)
        {
            pf = strategies->getStrategy(j);
            payoffIndex[j + 1] = (strategies->*pf)(j);
        }

        memcpy(strategies->getMoveTable(), payoffIndex + 1, playerNum * sizeof(int));
    }
}

double *Game::getFinalPayoff()
{
    return finalPayoff;
}
