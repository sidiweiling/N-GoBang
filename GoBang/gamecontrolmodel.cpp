#include <utility>
#include <stdlib.h>
#include <time.h>
#include "gamecontrolmodel.h"

GameControlModel::GameControlModel()
{
    for(int i=0;i<kRow+1;i++)//为数组赋值-1
    {
        for(int j=0;j<kColumn+1;j++)
        {
            chessboard[i][j]=-1;
            pchessboard[0][i][j]=-1;
            pchessboard[1][i][j]=-1;
        }
    }
}

void GameControlModel::PositioningPoint(int x, int y)
{
//在棋盘之外，棋盘向外延伸了kposUncertainty长度
    if(    x<kBoardLeftMargin-kPosUncertainty||
            x>kBoardLeftMargin+kColumn*kBlockSize+kPosUncertainty||
            y<kBoardUpMargin-kPosUncertainty||
            y>kBoardUpMargin+kRow*kBlockSize+kPosUncertainty
           )
    {
        nx=0;
        ny=0;
    }
    //在棋盘之内时
    else
    {
        int px=x%kBlockSize,py=y%kBlockSize;
        if(px>=kBlockSize-kPosUncertainty&&py<=kPosUncertainty)//若成立定位到格子右上角
        {
            nx=x-px+kBlockSize;
            ny=y-py;
        }
        if(px>=kBlockSize-kPosUncertainty&&py>=kBlockSize-kPosUncertainty)//若成立定位到格子右下角
        {
            nx=x-px+kBlockSize;
            ny=y-py+kBlockSize;
        }
        if(px<=kPosUncertainty&&py>=kBlockSize-kPosUncertainty)//若成立定位到格子左下角
        {
            nx=x-px;
            ny=y-py+kBlockSize;
        }
        if(px<=kPosUncertainty&&py<=kPosUncertainty)//若成立定位到格子左上角
        {
            nx=x-px;
            ny=y-py;
        }
    }
}

void GameControlModel::startGame(bool type)
{
    porc = type;
    // 初始棋盘
    chessboardclear();

    // 如果是AI模式，需要初始化评分数组
    if (porc == 1)
    {
        scoreMapVecclear();
    }

    // 己方下为true,对方下位false
    playerFlag = true;


}

void GameControlModel::updateGameMap(int row, int col)
{
    if (playerFlag)
        chessboard[row][col] = 1;
    else
        chessboard[row][col] = 0;

    // 换手
    playerFlag = !playerFlag;
}

void GameControlModel::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}

void GameControlModel::actionByAI(int &clickRow, int &clickCol)
{
    // 计算评分
    calculateScore();

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;

    for (int row = 0; row <= kRow; row++)
        for (int col = 0; col <= kColumn; col++)
        {
            // 前提是这个坐标是空的
            if (chessboard[row][col] == -1)
            {
                if (scoreMapVec[row][col] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[row][col];
                    maxPoints.push_back(std::make_pair(row, col));
                }
                else if (scoreMapVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(row, col));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 记录落子点
    clickCol = pointPair.second;
    updateGameMap(clickRow, clickCol);
}

//最关键的计算评分函数
void GameControlModel::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreMapVecclear();

    // 计分（此处是完全遍历，其实可以用bfs或者dfs加减枝降低复杂度，通过调整权重值，调整AI智能程度以及攻守风格）
    for (int row = 0; row <= kRow; row++)
        for (int col = 0; col <= kColumn; col++)
        {
            // 空白点就算
            if (row > 0 && col > 0 &&
                chessboard[row][col] == -1)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(x == 0 && y == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y >= 0 && row + i * y <= kRow &&
                                    col + i * x >= 0 && col + i * x <= kColumn &&
                                    chessboard[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y >= 0 && row + i * y <= kRow &&
                                         col + i * x >= 0 && col + i * x <= kColumn &&
                                         chessboard[row + i * y][col + i * x] == -1) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y >= 0 && row - i * y <= kRow &&
                                    col - i * x >= 0 && col - i * x <= kColumn &&
                                    chessboard[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y >= 0 && row - i * y <= kRow &&
                                         col - i * x >= 0 && col - i * x <= kColumn &&
                                         chessboard[row - i * y][col - i * x] == -1) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)                      // 杀二
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)                 // 杀三
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)                 // 杀四
                            {
                                // 量变空位不一样，优先级不一样
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)                 // 杀五
                                scoreMapVec[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI白子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y >= 0 && row + i * y <= kRow &&
                                    col + i * x >= 0 && col + i * x <= kColumn &&
                                    chessboard[row + i * y][col + i * x] == 0) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y >= 0 && row + i * y <= kRow &&
                                         col + i * x >= 0 && col + i * x <= kColumn &&
                                         chessboard[row + i * y][col + i * x] == -1) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y >= 0 && row - i * y <= kRow &&
                                    col - i * x >= 0 && col - i * x <= kColumn &&
                                    chessboard[row - i * y][col - i * x] == 0) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y >= 0 && row - i * y <= kRow &&
                                         col - i * x >= 0 && col - i * x <= kColumn &&
                                         chessboard[row - i * y][col - i * x] == -1) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)                      // 普通下子
                                scoreMapVec[row][col] += 5;
                            else if (botNum == 1)                 // 活二
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)                // 死三
                                    scoreMapVec[row][col] += 25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;  // 活三
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)                // 死四
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100; // 活四
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] += 10000;   // 活五

                        }
                    }

            }
        }
}

bool GameControlModel::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 竖直方向
    for (int i = 0; i < 5; i++)
    {
        // 往下5个，往上匹配4个子，20种情况
        if (col - i >= 0 &&
            col - i + 4 <= kColumn &&
            chessboard[row][col - i] == chessboard[row][col - i + 1] &&
            chessboard[row][col - i] == chessboard[row][col - i + 2] &&
            chessboard[row][col - i] == chessboard[row][col - i + 3] &&
            chessboard[row][col - i] == chessboard[row][col - i + 4])
            return true;
    }

    // 水平方向(左右延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 <= kRow &&
            chessboard[row - i][col] == chessboard[row - i + 1][col] &&
            chessboard[row - i][col] == chessboard[row - i + 2][col] &&
            chessboard[row - i][col] == chessboard[row - i + 3][col] &&
            chessboard[row - i][col] == chessboard[row - i + 4][col])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (col + i <= kColumn &&
            col + i - 4 >= 0 &&
            row - i >= 0 &&
            row - i + 4 <= kRow &&
            chessboard[row + i][col - i] == chessboard[row + i - 1][col - i + 1] &&
            chessboard[row + i][col - i] == chessboard[row + i - 2][col - i + 2] &&
            chessboard[row + i][col - i] == chessboard[row + i - 3][col - i + 3] &&
            chessboard[row + i][col - i] == chessboard[row + i - 4][col - i + 4])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i >= 0 &&
            row - i + 4 <= kRow &&
            col - i >= 0 &&
            col - i + 4 <= kColumn &&
            chessboard[row - i][col - i] == chessboard[row - i + 1][col - i + 1] &&
            chessboard[row - i][col - i] == chessboard[row - i + 2][col - i + 2] &&
            chessboard[row - i][col - i] == chessboard[row - i + 3][col - i + 3] &&
            chessboard[row - i][col - i] == chessboard[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameControlModel::isdead()
{
    // 所有空格全部填满
    for (int i = 1; i < kRow + 1; i++)
        for (int j = 1; j < kColumn + 1; j++)
        {
            if (!(chessboard[i][j] == 1 || chessboard[i][j] == 0))
                return false;
        }
    return true;
}

void GameControlModel::chessboardclear()
{
    for(int i=0;i<kRow+1;i++)//为数组赋值-1
    {
        for(int j=0;j<kColumn+1;j++)
        {
            chessboard[i][j]=-1;
            pchessboard[0][i][j]=-1;
            pchessboard[1][i][j]=-1;
        }
    }
}

void GameControlModel::scoreMapVecclear()
{
    for (int i = 0; i <= kRow; i++)
    {
        for (int j = 0; j <= kColumn; j++)
            scoreMapVec[i][j]=0;
    }
}
