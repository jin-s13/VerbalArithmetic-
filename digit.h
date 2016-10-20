#ifndef DIGIT_H
#define DIGIT_H
#include <QVector>
#include <QList>
#include <iostream>
using namespace std;
typedef QList<int> LISTINT;

class Coordinate
{
public:
    int h_loc;
    int v_loc;
    Coordinate()
    {
        this->h_loc = 0;
        this->v_loc = 0;
    }
    Coordinate(int h,int v)
    {
        this->h_loc = h;
        this->v_loc = v;
    }

    //对象==int
     bool operator==(const Coordinate &t1)
     {
         return (this->h_loc == t1.h_loc && this->v_loc == t1.v_loc);
     }
};

class Digit{

public:
    enum State{UNSEARCHED,SEARCHED,CONSTANT};       //
    Digit()
    {
        this->_key = 0;
        this->_state = UNSEARCHED;
        this->_currentValue = -1;
        this->_num = 1;
        this->_domain = QVector<bool> (10,true);
    }

    Digit(QChar key, Coordinate location)
    {
        this->_key = key;
        this->_state = UNSEARCHED;
        this->_currentValue = -1;
        this->_num = 1;
        this->_locList.push_back(location);
        this->_domain = QVector<bool> (10,true);
    }

    void addNum(Coordinate location);             //新增一个数字
    bool shrinkDomain(int del);                   //从定义域中删去
    bool recoverDomain(int rec);                   //恢复定义域

    int findCandidate();                          //从定义域中找到一个可行数字
    //判别函数
    bool onVloc(const int vloc);      //是否位于 vloc上
    bool inDomain(int dom){return this->_domain[dom];}         //是否在定义域中

    //get函数
    int getCurrentValue(){return this->_currentValue;}
    State getState(){return this->_state;}
    QChar getKey(){return this->_key;}
    int getNum(){return this->_num;}
    QVector<bool> getDomain(){return this->_domain;}
    int getDomainSize();
    QVector<Coordinate> getLocation(){return this->_locList;}

    //set函数
    void setCurrentValue(const int v){this->_currentValue = v;}
    void setState(const State s){this->_state = s;}
    void setKey(const QChar k){this->_key = k;}
    void setDomain(QVector<bool> dom){this->_domain = dom;}
    void setDomain(QVector<int> dom);
    void setNum(const int n){this->_num = n;}

    //设置状态
    void setUnsearched(){this->_state = UNSEARCHED;}
    void setSearched(){this->_state = SEARCHED;}
    void setConstant(){this->_state = CONSTANT;}

private:

    QChar _key;               //代表的文本
    State _state;            //是否被扩展
    int _currentValue;       //当前值
    int _num;                //记录个数

    QVector<Coordinate> _locList;         //记录位置
    QVector<bool> _domain;                //定义域

};
#endif // DIGIT_H
