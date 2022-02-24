#ifndef JUMPOUTWINDOW_H
#define JUMPOUTWINDOW_H

#include <QDialog>
#include<QSpinBox>
#include<QPushButton>

namespace Ui {
class JumpOutWindow;
}

class JumpOutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit JumpOutWindow(QWidget *parent = nullptr);
    ~JumpOutWindow();

private:
    Ui::JumpOutWindow *ui;

};

#endif // JUMPOUTWINDOW_H
