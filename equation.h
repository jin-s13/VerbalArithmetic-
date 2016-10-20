#ifndef EQUATION_H
#define EQUATION_H

#include"digit.h"
typedef QList<Digit> LISTDIGIT;
typedef QVector<Digit> VECDIGIT;
#define CARRYKEYBEGIN 0x00A0
#define CARRYKEYEND 0x00FF

struct DigitMap{
    QChar key;
    int value;
};

class Equation{

public:
    enum Expression{add,sub,mul,div};       //标点
    void clear();
    void parsing(QVector<QString> lines);     //分解剥离数字
    void createCarry();                     //初始化Carry-bit
    void createAddCarry(int curr_h, int lshift, int num);
    void createMulCarry(int curr_h, int lshift);
    void Initialize(QVector<QString> lines, Expression ex);

    Coordinate Rule2Loc(int ruleId);

    LISTDIGIT::iterator findDigitByKey(QChar key);    //按照key来查找数字
    QVector<LISTDIGIT::iterator> findDigitByVloc(int v_loc);    //按照vloc来查找数字
    QVector<LISTDIGIT::iterator> findCarryByVloc(int v_loc);    //按照vloc来查找carry-bit
    QVector<LISTDIGIT::iterator> findDigitByRuleId(int ruleId);    //按照ruleId来查找数字
    QVector<LISTDIGIT::iterator> findCarryByRuleId(int ruleId);    //按照ruleId来查找carry-bit
    LISTDIGIT::iterator findDigitByloc(Coordinate loc);    //按照loc来查找数字
    LISTDIGIT::iterator findCarryByloc(Coordinate loc);    //按照loc来查找carry


    QVector<LISTDIGIT::iterator> filterState(QVector<LISTDIGIT::iterator> vset,Digit::State s);

    bool search(int n/*要搜索的digit的总数*/, int m/*搜索过的digit数*/);      //核心搜索函数
    void printResult();
    LISTDIGIT::iterator nextDigit();

    QVector<int> getRuleId(Coordinate loc, QChar key);

    int getDigitNum(){return digitSet.size();}
    int getRuleNum();
    int getSearchNum();
    int getIterNum(){return this->iter_num;}
    Expression getExpression(){return this->_exp;}
    void setExpression(Expression ex){this->_exp = ex;}


    bool firstDigitConstrain();     //如果满足 所有首位非0条件，则返回true

    bool isNull(LISTDIGIT::iterator it);
    bool isSafe(QChar key);
    bool onRuleId(LISTDIGIT::iterator it, int ruleId);

    bool checkRule(int ruleId);
    bool checkAddRule(int start_h, int end_h, int vloc);
    bool checkMulRule(int v1, int v2, int hloc, int vloc);
    bool checkUnique();

public:
    QVector<QVector<DigitMap>> result;
private:
    int _carryNum = 0;
    int iter_num = 0;
    QVector<QString> h_lines;
    LISTDIGIT digitSet;
    LISTDIGIT carrySet;
    Expression _exp = add;

};

#endif // EQUATION_H
