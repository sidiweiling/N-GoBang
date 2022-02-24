//设置自定义按钮，用来实现开始界面的那两个无边框按钮
#include "custombutton.h"

CustomButton::CustomButton(QString path,int width,int height)
{
    QPixmap pic;
    bool test=pic.load(path);
    if(!test)
    {
        qDebug()<<"Loading fail";
        return;
    }
    this->setFixedSize(width,height);
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pic);
    this->setIconSize(QSize(width,height));
}
