#include "startwindow.h"
#include "ui_startwindow.h"

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    setWindowState(Qt::WindowMaximized);//set initial size of window
    setWindowIcon(QIcon(":/icon/Resource/Icons/chess.ico"));//set windowicon


//set pvp pvc buttons
    pvp=new CustomButton(":/icon/Resource/Icons/pvp2.ico",128,117);
    pvp->setParent(this);
    pvp->move(400,150);
    pvc=new CustomButton(":/icon/Resource/Icons/computer.ico",128,100);
    pvc->setParent(this);
    pvc->move(400,350);

//connect
    connect(pvp,SIGNAL(clicked()),this,SLOT(GotoPVP()));
    connect(pvc,SIGNAL(clicked()),this,SLOT(GotoPVC()));


}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::paintEvent(QPaintEvent*)//draw background
{
    QPainter pt(this);
    QPixmap px;
    px.load(":/image/Resource/Images/preface.jpg");
    pt.drawPixmap(0,0,this->width(),this->height(),px);
}

void StartWindow::SwitchtoGameWindow(bool x)//转换到gamewindow
{
    gwindow->porc=x;//send parameter x to controll the game type
    gwindow->show();
    gwindow->setDisabled(true);
    gwindow->firstjumpwindow->exec();
    this->hide();

}
void StartWindow::GotoPVP()//pvp按钮的槽函数
{

    gwindow=new GameWindow;
    connect(gwindow->home,SIGNAL(clicked()),this,SLOT(GotoHome()));
    connect(gwindow->ok,SIGNAL(clicked()),this,SLOT(ActivateGameWindow()));
    SwitchtoGameWindow(false);
}
void StartWindow::GotoPVC()//pvc按钮的槽函数
{

    gwindow=new GameWindow;
    connect(gwindow->home,SIGNAL(clicked()),this,SLOT(GotoHome()));
    connect(gwindow->ok,SIGNAL(clicked()),this,SLOT(ActivateGameWindow()));
    SwitchtoGameWindow(true);
}

void StartWindow::GotoHome()//gamewindow 中工具栏第一个按钮的槽函数，用于关闭gamewindow返回主页
{
    this->show();
    gwindow->close();
    delete gwindow;
}
void StartWindow::ActivateGameWindow()//使gamewindow在未完成时间设置时不接收输入，防止在设置时间时误操作产生bug
{
    gwindow->setDisabled(false);
}
