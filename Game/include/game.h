#ifndef GAME_H
#define GAME_H

//参与人
class Player
{
private:
    int playerNum;       //参与人总数
    int *moveNum;        //每个参与人的行动数
    bool isDefaultTable; //是否不采用外部输入的行动表，true为不采用

public:
    Player(int playerNum = 2);
    Player(int playerNum, int *moveNum);
    virtual ~Player();

    int getPlayerNum();
    void setmoveNum(int *moveNum);
    int *getmoveNum();
};

//支付
class Payoff
{
private:
    Player *players;
    double *payoffTable; //支付表指针

    int *tableIndex; //表索引
    int tableSize;   //支付表大小

    bool isDefaultTable; //是否采用默认支付表
public:
    Payoff(Player *players);
    Payoff(Player *players, double *payoffTable);
    virtual ~Payoff();

    //从外部设置支付表，输入表数组指针
    void setPayoffTable(double *payoffTable);

    //单独设置支付值
    void setPayoffValue(double payoff, int *index);

    //返回支付值
    double getPayoffValue(int *index);
};

//策略
enum StrategyName
{
    STAY = 0,
    GRIM,
    TFT
};

class Strategy
{
private:
    Player *players;

    typedef int (Strategy::*strategyFunc)(int);
    strategyFunc *playerStrategyFunc; //各个参与人选择的策略

    int *moveTable; //各个参与人当前的行动

    bool isDefaultTable; //是否采用默认行动表

    //保持策略
    int stay(int playerIndex);

    //冷酷策略
    int grim(int playerIndex);

    //以牙还牙策略
    int tft(int playerIndex);

public:
    Strategy(Player *players);
    Strategy(Player *players, StrategyName *strategyTable);
    virtual ~Strategy();

    void setMoveTable(int *moveTable);
    int *getMoveTable();
    void setStrategy(StrategyName *strategyTable);
    strategyFunc getStrategy(int playerIndex);
};

//博弈基本类
class Game
{
private:
    Player *players;
    Payoff *payoffs;
    Strategy *strategies;

    bool isDefault;

    int *payoffIndex;
    double *finalPayoff;

public:
    Game();
    Game(Player *players, Payoff *payoffs, Strategy *strategies);
    virtual ~Game();

    void repeatedPlay(int times);
    double *getFinalPayoff();
};

#endif //GAME_H