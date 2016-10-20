#ifndef CRYPTARITHMETIC_H
#define CRYPTARITHMETIC_H
#include "editdialog.h"
#include "pooldialog.h"
#include "equation.h"
#include "gendialog.h"
#include "resultdialog.h"
#include "waitingdialog.h"
#include "api/myhelper.h"
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QFileDialog>
#include <QGridLayout>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QDialog>
#include <QTime>
#include <QFont>
#include <QtMath>

namespace Ui {
class Cryptarithmetic;
}

class Cryptarithmetic : public QWidget
{
    Q_OBJECT

public:
    enum DifficultyClass{EASY,MEDIUM,HARD};
    enum GameState{ready,input,begin,end};
    explicit Cryptarithmetic(QWidget *parent = 0);
    ~Cryptarithmetic();
private:
    Ui::Cryptarithmetic * ui;
    QGraphicsScene scene;
    Equation eq;
    GameState state = ready;  //游戏状态控制
private:
    void paintEvent(QPaintEvent *);

    QVector<QString> IntToEquation(QVector<int> vec);    //
    bool genAddQuestion(int num, int len, int types);  //加数数目，加数长度，字母种类
    bool genSubQuestion(int num, int len, int types);  //减数数目，加数长度，字母种类
    bool genMulQuestion(int len_up, int len_down, int types);  //被乘数长度，乘数长度，字母种类
    bool genDivQuestion(bool residual, int len_up, int len_down, int types);  //余数，乘数，字母种类
public:

    void initStyle();   //初始化无边框窗体
    EditDialog * dlg_Edit;
    GenDialog * dlg_Gen;
    PoolDialog * dlg_Pool;
    Equation::Expression exp;   //加减乘除
    QVector<int> editList;   //Edit返回的设置
    QVector<QString> equ_lines; //保存算式
    int qstNum; //选择经典题目题号
    DifficultyClass difficulty; //题目难度
    int qstAssess;  //实际难度评估
    void ShowEquation(bool readFlag);

private slots:
    void on_btn_GenQuestion_clicked();
    void on_btn_Custom_clicked();
    void on_btn_QuestionPool_clicked();
    void on_btn_ShowResult_clicked();
};

#endif // CRYPTARITHMETIC_H
