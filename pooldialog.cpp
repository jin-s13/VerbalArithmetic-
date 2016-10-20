#include "pooldialog.h"
#include "ui_pooldialog.h"
#include "api/myhelper.h"
#include "cryptarithmetic.h"
using namespace  std;

PoolDialog::PoolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PoolDialog)
{
    ui->setupUi(this);
}

PoolDialog::~PoolDialog()
{
    delete ui;
}

void PoolDialog::paintEvent(QPaintEvent *)
{
    QImage image(":/image/POOL.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}
void PoolDialog::mouseReleaseEvent(QMouseEvent *e)
{
    int x = int(e->x());
    int y = int(e->y());
    int n_x = x/138;    //长度
    int n_y = y/125;    //高度
    int num = n_x + n_y * 5;    //序号
    Cryptarithmetic* p = (Cryptarithmetic*)this->parentWidget();
    p->qstNum = num;
    QString str = "题目" + QString::number(num + 1) + "请您做好准备！";
    myHelper::showMessageBoxInfo(str);
    QDialog::accept();
}
