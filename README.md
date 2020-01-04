# RepeatedPlay

## 更新日志

- v1.1 补充README.md。
- v1.0 完成基本程序框架，实现重复博弈实验基本功能。

## 问题描述

设计两个无限次重复博弈中的策略，并让两个策略PK，用计算机模拟结果，并分析结果。

计分规则：两个策略都选择了合作，则双方各得3分，如果都背叛则各得1分，如果一方背叛一方合作，则背叛方得5分而合作方得0分，总博弈次数大于200次。

## 问题分析

为解决上述博弈问题，首先建立博弈表。包含2名参与人A、B，类型为同时行动博弈，行动为合作与背叛，如下表所示：

|          | **背叛** | **合作** |
| :------: | :------: | :------: |
| **背叛** |   1, 1   |   5, 0   |
| **合作** |   0, 5   |   3, 3   |

显然上述单次博弈将构成囚徒困境，纳什均衡为双方均选择背叛。此时可通过重复博弈解决，如：

- 保持策略（`STAY`），无论对方选择合作还是背叛，均保持上一次行动不变；
- 冷酷策略（`GRIM`），开始选择合作，若对方有一次背叛就以后均选择背叛；
- 以牙还牙策略（`TFT`），开始选择合作，若对方选择背叛，则下一次选择背叛，若对方选择合作，则下一次选择合作。

在此基础上，参与人可以进一步选择在第一次博弈时选择合作或背叛，从而形成多种多样的重复博弈策略。

## 代码说明

### 代码文件内容

本实验功能代码保存在`.\Game`文件夹中，包括`game.cpp`与`game.h`，`main.cpp`内实现测试代码。

### 代码结构

主要包含4个类，分别是博弈基本类`Game`，以及3个博弈要素类：参与人类`Player`、支付类`Payoff`、策略类`Strategy`。具体成员及成员函数功能参考`game.h`中的注释内容。

### 编译方式

采用cmake生成Makefile，然后编译。以Windows环境下采用MinGW编译为例，在代码文件夹根目录下即`RepeatedPlay`文件夹中依次执行以下命令：

```
cmake -G "MinGW Makefiles" -B ./build
cd build
mingw32-make
```

## 实验方案

本实验选取以下重复博弈策略（`策略名_初始行动`）：

- `STAY_0`，曹操策略，选取保持策略后令第一次行动为背叛，即每一次均选择背叛；
- `GRIM_1`，冷酷策略；
- `TFT_1`，以牙还牙策略，初始行动为合作；
- `TFT_0`，以牙还牙策略，初始行动为背叛。

实验代码如下：

**main.cpp**

```cpp
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
        game->repeatedPlay(times, moveTable[str]);   //设置博弈次数与初始行动

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
```

打印输出结果：

```
STAY_0: 204 GRIM_1: 199
STAY_0: 204 TFT_1: 199
STAY_0: 200 TFT_0: 200
GRIM_1: 600 TFT_1: 600
GRIM_1: 203 TFT_0: 203
TFT_1: 500 TFT_0: 500
```

## 结果分析

从上述代码运行结果可知，最终各策略得分为：

- `STAY_0`：608
- `GRIM_1`：1002
- `TFT_1`：1299
- `TFT_0`：903

可以看出，得分从高到低顺序为`TFT_1`，`GRIM_1`，`TFT_0`，`STAY_0`。显然首次行动选择背叛s虽然能够在本次博弈中取得少量收益，但同时也会造成对手的不信任，从而在之后的博弈中对手也会采取背叛策略，造成巨大损失。`STAY_0`一直背叛造成巨大损失，`TFT_1`首次背叛，后续对手也将不在信任。而与上述两种策略相比，`GRIM_1`只要对手不背叛就会一直保持合作，否则给予惩罚，因此也取得了较好的收益，而`TFT_1`在此基础上还能够原谅对手的背叛，尽可能保持双赢合作关系，故最终收益最高。
