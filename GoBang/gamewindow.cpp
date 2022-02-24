#include <QPainter>
#include <QTimer>
#include <QSound>
#include <QMouseEvent>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include <QMessageBox>
#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);//set initial size of window
    setWindowIcon(QIcon(":/icon/Resource/Icons/chess.ico"));//set windowicon
    centralWidget()->setMouseTracking(true);
    setMouseTracking(true);

//set home button
    home=new CustomButton(":/icon/Resource/Icons/home.ico",30,30);
    home->setParent(this);
    home->move(15,8);

//set BGMplayer
    bgmplayer=new QMediaPlayer(this);
    bgmplayer->setMedia(QUrl("qrc:/sound/Resource/Hungarian Rhapsody No.2.mp3"));
    bgmplayer->stop();

//set firstjumpwindow
    firstjumpwindow=new JumpOutWindow;
    ok=new QPushButton(firstjumpwindow);
    spinboxtotaltime=new QSpinBox(firstjumpwindow);
    spinboxnextsteptime=new QSpinBox(firstjumpwindow);
    ok->setText("OK");
    ok->move(360,150);
    spinboxtotaltime->setPrefix("游戏总时长");
    spinboxtotaltime->setSuffix("秒");
    spinboxtotaltime->setMaximum(9999);
    spinboxtotaltime->setMinimum(10);
    spinboxtotaltime->setValue(300);
    spinboxtotaltime->setSingleStep(60);
    spinboxtotaltime->move(40,60);
    spinboxnextsteptime->setPrefix("下步棋最长思考");
    spinboxnextsteptime->setSuffix("秒");
    spinboxnextsteptime->setMaximum(99);
    spinboxnextsteptime->setMinimum(10);
    spinboxnextsteptime->setValue(60);
    spinboxnextsteptime->setSingleStep(3);
    spinboxnextsteptime->move(270,60);

    gcontrolmodel=new GameControlModel;
    // 开始游戏
    initGame();

//connect
    connect(ui->actionbgm,SIGNAL(triggered(bool)),this,SLOT(BGMcontroll(bool)));
    connect(ui->actionrestart,SIGNAL(triggered()),this,SLOT(gamerestart()));
    connect(ui->actionhuiqi, SIGNAL(triggered()),this,SLOT(confess()));
    connect(ok,SIGNAL(clicked()),this,SLOT(IniGameWindow()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerslot()));

}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::initGame()
{
    gcontrolmodel->startGame(porc);
    gcontrolmodel->gameStatus = PLAYING;
}

void GameWindow::BGMcontroll(bool t)//bgm槽函数
{
    if(t)bgmplayer->play();
    if(!t)bgmplayer->stop();
}

void GameWindow::IniGameWindow()//时间设置窗口中ok 按钮的槽函数
{
    totaltime=spinboxtotaltime->value();
    nextsteptime=spinboxnextsteptime->value();
    firstjumpwindow->close();
    timenum_01 = totaltime;
    timenum_02 = nextsteptime;
    QString s=QString("总剩余时间:");
    ui->totaltime->setText(s);
    ui->lcdNumbertotaltime->display(totaltime);
    ui->lcdNumbernextsteptime->display(nextsteptime);
    if (false == this->timer.isActive())
    {
        this->timer.start(1000);
    }
    initGame();
    this->update();
}


void GameWindow::DrawPreChess(int x, int y)
{
    QPainter ptpre(this);
    ptpre.setRenderHint(QPainter::Antialiasing, true);
    QPen pnpre;
    pnpre.setWidth(1);
    ptpre.setPen(pnpre);
    QBrush brpre;
    brpre.setStyle(Qt::SolidPattern);
    brpre.setColor(Qt::white);
    ptpre.setBrush(brpre);
    ptpre.drawEllipse(x-kMarkSize*0.5,y-kMarkSize*0.5,kMarkSize,kMarkSize);
}

void GameWindow::DrawChess(int x,int y,bool color)
{
    QPainter ptchs(this);
    ptchs.setRenderHint(QPainter::Antialiasing, true);
    QPen pnchs;
    pnchs.setWidth(1);
    ptchs.setPen(pnchs);
    QBrush brchs;
    brchs.setStyle(Qt::SolidPattern);
    if(color==1)brchs.setColor(Qt::black);
    if(color==0)brchs.setColor(Qt::white);
    ptchs.setBrush(brchs);
    ptchs.drawEllipse(x-kRadius,y-kRadius,kRadius*2,kRadius*2);
}

void GameWindow::paintEvent(QPaintEvent* ev)//画图
{
    Q_UNUSED(ev);
//绘制棋盘格
    QPainter ptbg(this);
    QPixmap pxbg;
    pxbg.load(":/image/Resource/Images/gamebackground.jpg");
    ptbg.drawPixmap(0,0,this->width(),this->height(),pxbg);
    QPainter pt(this);
    pt.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    QPen pen; // 调整线条宽度
    pen.setWidth(1);
    pt.setPen(pen);
    for(int i=0;i<=kRow;i++)
    {
        pt.drawLine(kBoardLeftMargin,kBoardUpMargin+kBlockSize*i,this->width()-kBoardRightMargin,kBoardUpMargin+kBlockSize*i);
    }
    for(int i=0;i<=kColumn;i++)
    {
        pt.drawLine(kBoardLeftMargin+kBlockSize*i,kBoardUpMargin,kBoardLeftMargin+kBlockSize*i,this->height()-kBoardDownMargin);
    }
//绘制prechess小标志, 在winposx和winposy有效时（即不为0）才绘制
    if(winposx!=0&&winposy!=0)DrawPreChess(winposx,winposy);
//绘制棋子
    for(int i=0;i<=kRow;i++)
    {
        for(int j=0;j<=kColumn;j++)
        {
            if(gcontrolmodel->chessboard[i][j]==0)
            {
                DrawChess(kBoardLeftMargin+j*kBlockSize,kBoardUpMargin+i*kBlockSize,0);
            }
            else if (gcontrolmodel->chessboard[i][j]==1)
            {
                DrawChess(kBoardLeftMargin+j*kBlockSize,kBoardUpMargin+i*kBlockSize,1);
            }
        }
    }

}

void GameWindow::mouseMoveEvent(QMouseEvent *ev)
{
    int rawx=ev->x();
    int rawy=ev->y();
    gcontrolmodel->nx=0;
    gcontrolmodel->ny=0;
    gcontrolmodel->PositioningPoint(rawx,rawy);
    winposx=gcontrolmodel->nx;
    winposy=gcontrolmodel->ny;
    chsboardposx=(winposx-kBoardLeftMargin)/40;
    chsboardposy=(winposy-kBoardUpMargin)/40;
    if(winposx!=0&&winposy!=0&&gcontrolmodel->chessboard[chsboardposy][chsboardposx]==-1)
        update();
}

void GameWindow::mousePressEvent(QMouseEvent*)
{
    // 人下棋，并且不能抢机器的棋
    if (!(porc == 1 && !gcontrolmodel->playerFlag))
    {
        chessOneByPerson();
        // 如果是人机模式，需要调用AI下棋
        if (porc == 1 && !gcontrolmodel->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
        }
    }
    update();
}

void GameWindow::AddScoreAndDisplay()
{
    if (chsboardposx >= 0 && chsboardposx <= kColumn &&
        chsboardposy >= 0 && chsboardposy <= kRow &&
        (gcontrolmodel->chessboard[chsboardposy][chsboardposx] == 1 ||gcontrolmodel->chessboard[chsboardposy][chsboardposx] == 0))
    {
        if (gcontrolmodel->isWin(chsboardposy, chsboardposx)
                && gcontrolmodel->gameStatus == PLAYING
                &&gcontrolmodel->chessboard[chsboardposy][chsboardposx] == 1)
        {
            gcontrolmodel->scoreb++;
            QString sb=QString("黑棋得分：%1").arg(gcontrolmodel->scoreb);
            ui->labelblackscore->setText(sb);
        }
        if (gcontrolmodel->isWin(chsboardposy, chsboardposx)
                && gcontrolmodel->gameStatus == PLAYING
                &&gcontrolmodel->chessboard[chsboardposy][chsboardposx] == 0)
        {
            gcontrolmodel->scorew++;
            QString sw=QString("白棋得分：%1").arg(gcontrolmodel->scorew);
            ui->labelwhitescore->setText(sw);
        }
    }
}

void GameWindow::timerslot()
{
    this->timenum_02--; // 每次减1
    this->timenum_01--;          // 每次减1
    ui->lcdNumbertotaltime->display(timenum_01);   // 时间显示到LCD上
    ui->lcdNumbernextsteptime->display(timenum_02); // 时间显示到LCD上
    if(0 == this->timenum_01)   // 时间到就判断游戏最终输赢，结束游戏
    {
        timer.stop();
        gcontrolmodel->gameStatus = WIN;
        QString s="o";
        if(gcontrolmodel->scoreb>gcontrolmodel->scorew)
            s="Black chess";
        if(gcontrolmodel->scoreb<gcontrolmodel->scorew)
            s="White chess";
        if(gcontrolmodel->scoreb==gcontrolmodel->scorew)
            s="None of you";

            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "",s+" won the game!");
            gcontrolmodel->scoreb=0;
            gcontrolmodel->scorew=0;
            QString sb=QString("黑棋得分：%1").arg(gcontrolmodel->scoreb);
            ui->labelblackscore->setText(sb);
            QString sw=QString("白棋得分：%1").arg(gcontrolmodel->scorew);
            ui->labelwhitescore->setText(sw);
       // 重置游戏状态，否则容易死循环
       if (btnValue == QMessageBox::Ok)
       {
           IniGameWindow();
           update();
       }
    }
    if(0 == this->timenum_02)//时间到就切换下棋方
    {
        playingtimer();
        if (porc == 1 && !gcontrolmodel->playerFlag)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessOneByAI()));
        }
        update();
    }
}

void GameWindow::playingtimer()
{
    if(gcontrolmodel->playerFlag == 1)
        gcontrolmodel->playerFlag=0;
    else
        gcontrolmodel->playerFlag=1;
    timenum_02 = nextsteptime;
}

void GameWindow::chessOneByPerson()
{
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (chsboardposx != -1 && chsboardposy != -1 && gcontrolmodel->chessboard[chsboardposy][chsboardposx] == -1)
    {
        previous_step();
        gcontrolmodel->actionByPerson(chsboardposy, chsboardposx);
        AddScoreAndDisplay();

        // 重绘
        timenum_02 = nextsteptime;
        update();
    }
}

void GameWindow::chessOneByAI()
{
    previous_step();
    gcontrolmodel->actionByAI(chsboardposy, chsboardposx);
    AddScoreAndDisplay();
    timenum_02 = nextsteptime;
    update();
}

void GameWindow::previous_step()
{
    //储存上两步棋，方便悔棋
    for(int i=0; i<= kRow; i++)
        for(int j=0; j<=kColumn; j++)
        {
            gcontrolmodel->pchessboard[1][i][j] = gcontrolmodel->pchessboard[0][i][j];
            gcontrolmodel->pchessboard[0][i][j] = gcontrolmodel->chessboard[i][j];
        }
}

void GameWindow::confess()
{
    //悔棋，双人模式后退一步，人机模式则后退两步
    if(porc==0)
    {
        for(int i=0; i<= kRow; i++)
            for(int j=0; j<=kColumn; j++)
            {
                gcontrolmodel->chessboard[i][j] = gcontrolmodel->pchessboard[0][i][j];
                gcontrolmodel->playerFlag = !gcontrolmodel->playerFlag;
            }
        update();
    }
    else if(porc==1)
        {
        for(int i=0; i<= kRow; i++)
            for(int j=0; j<=kColumn; j++)
            {
                gcontrolmodel->chessboard[i][j] = gcontrolmodel->pchessboard[1][i][j];
            }
        update();
        }
}

void GameWindow::gamerestart()
{
    //重开游戏
    gcontrolmodel=new GameControlModel;
    ui->labelblackscore->setText("黑棋得分：0");
    ui->labelwhitescore->setText("白棋得分：0");
    IniGameWindow();
}
