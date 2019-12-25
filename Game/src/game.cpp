#include "game.h"

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

void Player::setmoveNum(int *moveNum)
{
    this->moveNum = moveNum;
    isDefaultTable = false;
}

int *Player::getmoveNum()
{
    return moveNum;
}

Payoff::Payoff(int dim)
{
    this->dim = dim;
    tableIndex = new int[dim];
    int temp = 1;
    for (int i = 0; i < dim; i++)
    {
        temp *= 2;
        tableIndex[dim - 1 - i] = temp;
    }
    tableSize = dim * temp;
    payoffTable = new double[tableSize];
    isDefaultTable = true;
}

Payoff::Payoff(int dim, int *dimSize, double *payoffTable)
{
    this->dim = dim;
    tableIndex = new int[dim];
    this->payoffTable = payoffTable;
    isDefaultTable = false;
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
    for (int i = 0; i < dim; i++)
        temp += tableIndex[i] * index[i];
    temp += index[dim];
    payoffTable[temp] = payoff;
}

double Payoff::getPayoffValue(int *index)
{
    int temp = 0;
    for (int i = 0; i < dim; i++)
        temp += tableIndex[i] * index[i];
    temp += index[dim];
    return payoffTable[temp];
}

Strategy::Strategy(int playerNum)
{
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

Strategy::Strategy(int playerNum, int *moveTable, StrategyName *strategyTable)
{
    playerStrategyFunc = new strategyFunc[playerNum];
    isDefaultTable = false;
    this->moveTable = moveTable;
    setStrategy(strategyTable);
}

Strategy::~Strategy()
{
    delete[] playerStrategyFunc;
    if (isDefaultTable == true)
        delete[] moveTable;
}

int Strategy::stay(int playerIndex)
{
    return moveTable[playerIndex];
}

int Strategy::grim(int playerIndex)
{
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
    for (int i = 1; i < playerNum; i++)
    {
        if (moveTable[(playerIndex + i) % playerNum] == 0)
            return 0;
        else
            return 1;
    }
    return 1;
}

void Strategy::setStrategy(StrategyName *strategyTable)
{
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

Game::Game()
{
    players = new Player();
    payoffs = new Payoff();
    strategies = new Strategy();
    isDefault = true;
}

Game::Game(Player *players, Payoff *payoffs, Strategy *strategiesstrategies)
{
    this->players = players;
    this->payoffs = payoffs;
    this->strategies = strategies;
    isDefault = false;
}

Game::~Game()
{
    if (isDefault == true)
    {
        delete players;
        delete payoffs;
        delete strategies;
    }
}

void Game::repeatedPlay(int times)
{
    for (int i = 0; i < times; i++)
    {
        ;
    }

}