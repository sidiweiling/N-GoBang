#ifndef GAMECONTROLMODEL_H
#define GAMECONTROLMODEL_H

#include<vector>

// 游戏状态
enum GameStatus
{
    PLAYING,
    WIN
};

//初始窗口的大小为1920*1001, 棋盘的范围为 横向[1320,1880]   纵向[80,960]  棋盘中的点均为40的倍数
const int kRow=22;//棋盘的行数
const int kColumn=32;//棋盘的列数
const int kBoardLeftMargin = 600; //左间隙
const int kBoardRightMargin=40;//右间隙
const int kBoardUpMargin=80;//上间隙
const int kBoardDownMargin=40;//下间隙
const int kRadius =15; // 棋半径
const int kPosUncertainty = 19; // 鼠标点击的模糊距离上限
const int kMarkSize = 8; // 落子标记边长
const int kBlockSize = 40; // 格子的大小
const int kAIDelay = 700; // AI下棋的思考时间

class GameControlModel
{
public:
    GameControlModel();
    bool porc;//用于记录初始化为PVP=0还是PVC=1
    int chessboard[kRow+1][kColumn+1];//初始化赋值-1代表没有棋，0代表白棋，1代表黑棋
    int pchessboard[2][kRow+1][kColumn+1];//初始化赋值-1代表没有棋，0代表白棋，1代表黑棋，用于返回上一步
    int scoreMapVec[kRow+1][kColumn+1]; // 存储各个点位的评分情况，作为AI下棋依据,默认AI固定下白子
    int nx=0,ny=0;
    bool playerFlag; // 标示下棋方，0代表白棋，1代表黑棋,默认黑棋先下
    int scorew=0,scoreb=0;
    GameStatus gameStatus; // 游戏状态

    void PositioningPoint(int x,int y);//为nx和ny赋予判断出的点的坐标，未筛除点已经存在棋子的情况
    void startGame(bool porc); // 开始游戏
    bool isdead(); //判断死局
    void calculateScore(); // 计算评分
    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋
    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    bool isWin(int row, int col); // 判断游戏是否胜利
    void chessboardclear();
    void scoreMapVecclear();

};

#endif // GAMECONTROLMODEL_H
