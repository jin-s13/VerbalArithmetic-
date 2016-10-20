#include "editdialog.h"
#include "ui_editdialog.h"
#include "equation.h"
#include "cryptarithmetic.h"
#include "api/myhelper.h"
using namespace std;
EditDialog::EditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    //myHelper::setStyle(QString(":/qss/black.css"));
    ui->btnCancel->hide();
    ui->btnOk->hide();
    QMovie *movie =new QMovie(":/image/pointer.gif");
    ui->lbl_pointer->setMovie(movie);
    movie->start();
}

EditDialog::~EditDialog()
{
    delete ui;
}
void EditDialog::paintEvent(QPaintEvent *)
{
    QImage image(":/image/white.png");
    QPainter painter(this);
    painter.drawImage(0,0,image);
}

void EditDialog::on_comboBox_currentIndexChanged(int index)
{
    ui->lbl_pointer->hide();
    ui->btnCancel->show();
    ui->btnOk->show();
    exp = index;
    while(ui->formLayout->count() > 0)
    {
        QWidget* widget = ui->formLayout->itemAt(0)->widget();
        ui->formLayout->removeWidget(widget);
        delete widget;
        widget = nullptr;
    }
    switch (index) {
    case 0://add
    {
        lbl[0] = new QLabel("加数的个数(不包括被加数)");
        lbl[1] = new QLabel("加数的最大位数长度(包含被加数)");
        lbl[2] = new QLabel("和的位数长度");
        for(int i = 0; i<3; i++)
        {
            edit[i] = new QLineEdit();
            QRegExp rx("^[1-9]$");
            QRegExpValidator *pReg = new QRegExpValidator(rx, this);
            edit[i]->setValidator(pReg);
        }
        for(int i = 0; i<3;i++)
            ui->formLayout->addRow(lbl[i],edit[i]);
    }
        break;
    case 1://sub
    {
        lbl[0] = new QLabel("减数的个数(不含被减数)");
        lbl[1] = new QLabel("减数的最大位数长度");
        lbl[2] = new QLabel("被减数的位数长度");
        for(int i = 0; i<3; i++)
        {
            edit[i] = new QLineEdit();
            QRegExp rx("^[1-9]$");
            QRegExpValidator *pReg = new QRegExpValidator(rx, this);
            edit[i]->setValidator(pReg);
        }
        for(int i = 0; i<3;i++)
            ui->formLayout->addRow(lbl[i],edit[i]);
    }
        break;
    case 2://mul
    {
        lbl[0] = new QLabel("被乘数的位数长度");
        lbl[1] = new QLabel("乘数的位数长度");
        lbl[2] = new QLabel("积的位数长度");
        for(int i = 0; i<3; i++)
        {
            edit[i] = new QLineEdit();
            QRegExp rx("^[1-9]$");
            QRegExpValidator *pReg = new QRegExpValidator(rx, this);
            edit[i]->setValidator(pReg);
        }
        for(int i = 0; i<3;i++)
            ui->formLayout->addRow(lbl[i],edit[i]);
    }
        break;
    case 3://div
    {
        lbl[0] = new QLabel("被除数的位数长度");
        lbl[1] = new QLabel("除数的位数长度");
        lbl[2] = new QLabel("商的位数长度");
        lbl[3] = new QLabel("余数的位数长度");
        for(int i = 0; i<4; i++)
        {
            edit[i] = new QLineEdit();
            QRegExp rx("^[1-9]$");
            QRegExpValidator *pReg = new QRegExpValidator(rx, this);
            edit[i]->setValidator(pReg);
        }
        for(int i = 0; i<4;i++)
            ui->formLayout->addRow(lbl[i],edit[i]);
    }
        break;
    default:
        break;
    }
}

void EditDialog::on_btnOk_clicked()
{
    bool flag = true;
    int num;
    Cryptarithmetic* p = (Cryptarithmetic*)this->parentWidget();
    switch (this->exp) {
    case 0:
    {
        p->exp = Equation::add;
        if(edit[2]->text().toInt() < edit[1]->text().toInt() || edit[2]->text().toInt() < edit[0]->text().toInt())   //和的长度小于加数
        {
            flag = false;
        }
        num = 3;
    }
        break;
    case 1:
    {
        p->exp = Equation::sub;
        if(edit[0]->text().toInt() < edit[1]->text().toInt())
        {
            flag = false;
        }
        num = 3;
    }

        break;
    case 2:
    {
        p->exp = Equation::mul;
        if(edit[2]->text().toInt() < edit[0]->text().toInt() || edit[2]->text().toInt()<edit[1]->text().toInt())   //积的长度小于乘数
        {
            flag = false;
        }
        num = 3;
    }
        break;
    case 3:
    {
        p->exp = Equation::div;
        if(edit[2]->text().toInt() < edit[1]->text().toInt() || edit[0]->text().toInt()<edit[2]->text().toInt() || edit[1]->text().toInt() < edit[3]->text().toInt())
        {
            flag = false;
        }
        num = 4;
    }

        break;
    default:
        break;
    }
    if(flag)
    {
        p->editList.clear();
        for(int i = 0; i<num; i++)
        {
            p->editList.append(edit[i]->text().toInt());
        }
        while(ui->formLayout->count() > 0)
        {
            QWidget* widget = ui->formLayout->itemAt(0)->widget();
            ui->formLayout->removeWidget(widget);
            delete widget;
            widget = nullptr;
        }
        delete ui->btnCancel;
        delete ui->btnOk;
        delete ui->comboBox;
        delete ui->lbl_pointer;
        myHelper::showMessageBoxError("该功能属次要功能，暂留以后开发，敬请期待。\n可以修改题库，来输入题目哦:)\n");
        this->accept();
    }
    else
    {
        myHelper::showMessageBoxError("输入有误请检查!");
    }
}

void EditDialog::on_btnCancel_clicked()
{
    while(ui->formLayout->count() > 0)
    {
        QWidget* widget = ui->formLayout->itemAt(0)->widget();
        ui->formLayout->removeWidget(widget);
        delete widget;
        widget = nullptr;
    }
    delete ui->btnCancel;
    delete ui->btnOk;
    delete ui->comboBox;
    delete ui->lbl_pointer;
    this->close();
}
