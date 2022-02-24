#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QMediaPlayer>
#include<QUrl>
#include<QPainter>
#include<QTimer>
#include<QLCDNumber>
#include<QMouseEvent>
#include<QDebug>
#include"jumpoutwindow.h"
#include"custombutton.h"
#include"gamecontrolmodel.h"



namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    bool porc;//用于记录初始化为PVP=0还是PVC=1
    int totaltime,nextsteptime;//记录游戏总时长和下一步时长
    int timenum_01, timenum_02;
    CustomButton *home;//返回startwindow,因为要被startwindow调用所以放在public中

//startwindow和gamewindow之间会弹出这个用来设置时间的小窗口, 下面声明的三个部件都是用在哪个弹出窗口中的
    JumpOutWindow *firstjumpwindow;
    QPushButton *ok;
    QSpinBox *spinboxtotaltime;
    QSpinBox *spinboxnextsteptime;

    GameControlModel *gcontrolmodel;

public slots:
    void timerslot(); // 倒计时的槽函数

private:
    Ui::GameWindow *ui;
    QMediaPlayer *bgmplayer;//bgm播放器
    QTimer timer;
    int winposx=0,winposy=0;
    int chsboardposx=-1,chsboardposy=-1;
    QTimer machineTime;     // 机器的时间，延时作用

    void initGame(); //开始游戏
    void paintEvent(QPaintEvent*);//画图
    void DrawPreChess(int x,int y);//绘制鼠标移动时在将下位置产生的小标志, 默认白色
    void DrawChess(int x,int y,bool color);//绘制棋子, x 横坐标, y纵坐标, color为颜色  1黑色  0白色
    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void playingtimer(); // 转换倒计时方
    void previous_step(); // 记录上一至两步棋局
    void AddScoreAndDisplay();

signals:
    void initPVPGame();
    void initPVEGame();

private slots:
    void BGMcontroll(bool);
    void IniGameWindow();
    void chessOneByPerson(); // 人执行
    void chessOneByAI(); // AI下棋
    void confess(); // 悔棋
    void gamerestart(); //重新再来

};

#endif // GAMEWINDOW_H
