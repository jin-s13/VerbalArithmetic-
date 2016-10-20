#include "waitingdialog.h"
#include "ui_waitingdialog.h"
#include "api/myhelper.h"

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);
    //myHelper::setStyle(QString(":/qss/black.css"));
    ui->label->setText("可能需要数秒钟的时间。。\n请耐心等候。");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpDate()));
    timer->setInterval(1000);
    timer->start();
}

void WaitingDialog::paintEvent(QPaintEvent *)
{
    QImage image(":/image/black.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::timeUpDate()
{
    waitTime++;
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setMode(QLCDNumber::Dec);
    ui->lcdNumber->display(waitTime);

}
