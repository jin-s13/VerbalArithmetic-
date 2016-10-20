#include "resultdialog.h"
#include "ui_resultdialog.h"
#include "api/myhelper.h"
ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    //myHelper::setStyle(QString(":/qss/black.css"));
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::paintEvent(QPaintEvent *)
{
    QImage image(":/image/black.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void ResultDialog::showResult(QVector<QVector<DigitMap>> res)
{
    for(int i = 0; i<res.size(); i++)   //结果
    {
        QString str;
        QString prex = QString("【解") + QString::number(i+1) + QString("】");
        ui->textEdit->append(prex);
        for(int j = 0; j<res[i].size(); j++)    //字符
        {
            str += QString(res[i][j].key) + QString(":") + QString::number(res[i][j].value) + QString("\t");
        }
        ui->textEdit->append(str);
    }
}
