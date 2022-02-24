#ifndef STARTWINDOW_H
#define STARTWINDOW_H
//这个窗口是这个程序创建的第一个窗口，继承了所有后来新增的类
#include <QMainWindow>
#include<QPainter>
#include<QPixmap>
#include<QDebug>
#include<QSpinBox>
#include"gamewindow.h"
namespace Ui {
class StartWindow;
}

class StartWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    GameWindow *gwindow;
private:
    Ui::StartWindow *ui;

    CustomButton *pvp;
    CustomButton *pvc;

    void paintEvent(QPaintEvent*);
    void SwitchtoGameWindow(bool);//跳转到gamewindow,并弹出jumpoutwindow选择时间, 同时使 gamewindow无法接受输入，在设置完时间后才可以接受输入

private slots:
    void GotoPVP();
    void GotoPVC();
    void GotoHome();
    void ActivateGameWindow();//用于相应时间设置窗口的ok按钮，重新激活gamewindow
};

#endif // STARTWINDOW_H
