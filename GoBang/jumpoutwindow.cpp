#include "jumpoutwindow.h"
#include "ui_jumpoutwindow.h"

JumpOutWindow::JumpOutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JumpOutWindow)
{
    ui->setupUi(this);
     setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
     setWindowTitle("Time Setting");

}

JumpOutWindow::~JumpOutWindow()
{
    delete ui;
}

