#ifndef GAME_H
#define GAME_H

//参与人基本类
class Player
{
private:
    int playerNum; //参与人总数
    int *moveNum;  //每个参与人的行动数

public:
    Player(int playerNum = 2);           //构造函数，初始化参与人数量，默认为2个参与人，每个人的行动数为2
    Player(int playerNum, int *moveNum); //构造函数，初始化参与人数量，同时可设置每个参与人的行动数量
    virtual ~Player();

    int getPlayerNum();            //获取参与人数量
    void setmoveNum(int *moveNum); //设置参与人行动数
    int *getmoveNum();             //获取参与人行动数
};

//支付基本类
class Payoff
{
private:
    Player *players;
    double *payoffTable; //支付表指针

    int *tableIndex; //表索引
    int tableSize;   //支付表大小

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

//策略基本类
class Strategy
{
private:
    Player *players;

    typedef int (Strategy::*strategyFunc)(int);
    strategyFunc *playerStrategyFunc; //各个参与人选择的策略

    int *moveTable; //各个参与人当前的行动

    int stay(int playerIndex); //保持策略
    int grim(int playerIndex); //冷酷策略
    int tft(int playerIndex);  //以牙还牙策略

public:
    Strategy(Player *players);
    Strategy(Player *players, StrategyName *strategyTable);
    virtual ~Strategy();

    void setMoveTable(int *moveTable);             //设置行动表
    int *getMoveTable();                           //获取行动表指针
    void setStrategy(StrategyName *strategyTable); //设置每个参与人的行动策略
    strategyFunc getStrategy(int playerIndex);     //获取指定参与人的行动策略函数指针
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

    void repeatedPlay(int times, int *initMove); //重复博弈函数，设定博弈次数times与初始行动
    double *getFinalPayoff();                    //获取最终总支付
};

#endif //GAME_H