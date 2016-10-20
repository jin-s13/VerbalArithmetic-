#include "gendialog.h"
#include "ui_gendialog.h"
#include "cryptarithmetic.h"
#include "api/myhelper.h"
GenDialog::GenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenDialog)
{
    ui->setupUi(this);
    //myHelper::setStyle(QString(":/qss/black.css"));
    ui->lbl_type->setText(QString("题目类型"));
    ui->lbl_difficulty->setText(QString("难度评分"));
}

GenDialog::~GenDialog()
{
    delete ui;
}

void GenDialog::paintEvent(QPaintEvent *)
{
    QImage image(":/image/white.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void GenDialog::on_btnOk_clicked()
{
    Cryptarithmetic* p = (Cryptarithmetic*)this->parentWidget();
    switch (ui->comboBox_difficulty->currentIndex()) {
    case 0:
        p->difficulty = Cryptarithmetic::EASY;
        break;
    case 1:
        p->difficulty = Cryptarithmetic::MEDIUM;
        break;
    case 2:
        p->difficulty = Cryptarithmetic::HARD;
        break;
    default:
        break;
    }

    switch (ui->comboBox_type->currentIndex()) {
    case 0:
    {
        p->exp = Equation::add;
    }
        break;
    case 1:
    {
        p->exp = Equation::sub;
    }

        break;
    case 2:
    {
        p->exp = Equation::mul;
    }
        break;
    case 3:
    {
        p->exp = Equation::div;
    }
        break;
    default:
        break;
    }
    this->accept();
}
