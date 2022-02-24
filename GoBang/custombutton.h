//设置自定义按钮，用来实现开始界面的那两个无边框按钮
#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>
#include<QPixmap>
#include<QDebug>
#include<QString>
class CustomButton : public QPushButton
{
    Q_OBJECT
public:
     CustomButton(QString picturepath,int width,int height);

signals:

public slots:
};

#endif // CUSTOMBUTTON_H
