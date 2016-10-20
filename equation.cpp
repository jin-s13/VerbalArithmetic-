#include "equation.h"
#include"digit.h"
#include <iostream>
#include <QList>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QFile>
using namespace std;

void Equation::clear()
{
    _carryNum = 0;
    h_lines.clear();
    digitSet.clear();
    carrySet.clear();
    result.clear();
    _exp = add;
}
LISTDIGIT::iterator Equation::findDigitByKey(QChar key)         //find same key in digitSet
{
    LISTDIGIT::iterator i;
    for (i = digitSet.begin(); i != digitSet.end(); ++i)
    {
        if(i->getKey() == key)
            return i;
    }
    for (i = carrySet.begin(); i != carrySet.end(); ++i)
    {
        if(i->getKey() == key)
            return i;
    }
    return digitSet.end();  //此处有错误
}
QVector<LISTDIGIT::iterator> Equation::findDigitByVloc(int v_loc)
{
    QVector<LISTDIGIT::iterator> vset;
    LISTDIGIT::iterator i;
    for (i = digitSet.begin(); i != digitSet.end(); ++i)
    {
        if(i->onVloc(v_loc))
            vset.push_back(i);
    }
    return vset;
}
QVector<LISTDIGIT::iterator> Equation::findCarryByVloc(int v_loc)
{
    QVector<LISTDIGIT::iterator> vset;
    LISTDIGIT::iterator i;
    for (i = carrySet.begin(); i != carrySet.end(); ++i)
    {
        if(i->onVloc(v_loc))
            vset.push_back(i);
    }
    return vset;
}
QVector<LISTDIGIT::iterator> Equation::findDigitByRuleId(int ruleId)
{
    QVector<LISTDIGIT::iterator> vset;
    LISTDIGIT::iterator i;
    for (i = digitSet.begin(); i != digitSet.end(); ++i)
    {
        if(onRuleId(i,ruleId))
            vset.push_back(i);
    }
    return vset;
}
QVector<LISTDIGIT::iterator> Equation::findCarryByRuleId(int ruleId)
{
    QVector<LISTDIGIT::iterator> vset;
    LISTDIGIT::iterator i;
    for (i = carrySet.begin(); i != carrySet.end(); ++i)
    {
        if(onRuleId(i,ruleId))
            vset.push_back(i);
    }
    return vset;
}
LISTDIGIT::iterator Equation::findDigitByloc(Coordinate loc)
{
    LISTDIGIT::iterator i;
    for (i = digitSet.begin(); i != digitSet.end(); ++i)
    {
        for(int j = 0; j<i->getLocation().size(); j++)
        {
            if(i->getLocation()[j] == loc)
                return i;
        }
    }
    return digitSet.end();
}
LISTDIGIT::iterator Equation::findCarryByloc(Coordinate loc)
{
    LISTDIGIT::iterator i;
    for (i = carrySet.begin(); i != carrySet.end(); ++i)
    {
        for(int j = 0; j<i->getLocation().size(); j++)
        {
            if(i->getLocation()[j] == loc)
                return i;
        }
    }
    return carrySet.end();
}

Coordinate Equation::Rule2Loc(int ruleId)
{
    //transform ruleId to [base]location
    switch (_exp) {
    case add:
    case sub:
    {
        Coordinate co;          //locate
        co.h_loc = h_lines.size() - 1;
        co.v_loc = ruleId;
        return co;
    }
        break;
    case mul:
    {
        int mulRule = getRuleNum() - h_lines.back().size();
        if(ruleId < mulRule)    //乘法规则
        {
            int vloc = ruleId;
            int hloc = 2;
            for(int i = 2; i<h_lines.size() - 1; i++)
            {
                if(vloc >= h_lines[i].size())
                {
                    vloc -= h_lines[i].size();
                    hloc++;
                }
                else
                    break;
            }
            Coordinate co;
            co.h_loc = hloc;
            co.v_loc = vloc + hloc - 2;
            return co;
        }
        else    //加法规则
        {
            Coordinate co;          //locate
            co.h_loc = h_lines.size() - 1;
            co.v_loc = ruleId - mulRule ;
            return co;
        }
    }
        break;
    case div:
    {
        int vloc = ruleId;
        int hloc = 3;
        for(int i = 3; i<h_lines.size() - 1; i++)
        {
            if(vloc >= h_lines[i].size())
            {
                vloc -= h_lines[i].size();
                hloc++;
            }
            else
                break;
        }
        Coordinate co;
        co.h_loc = hloc;
        co.v_loc = vloc + (hloc-1)/2 - 1;
        return co;
    }
        break;
    default:
        break;
    }
}

QVector<LISTDIGIT::iterator> Equation::filterState(QVector<LISTDIGIT::iterator> vset,Digit::State s)
{
    QVector<LISTDIGIT::iterator> filt;
    for(int i = 0; i<vset.size(); i++)
    {
      if(vset[i]->getState() == s)
          filt.push_back(vset[i]);
    }
    return filt;
}


void Equation::parsing(QVector<QString> lines)
{
    for(int i = 0; i<(int)lines.size(); i++)              //each QString
    {
        for(int j = 0; j<(int)lines[i].length(); j++)    //each character
        {
            if(_exp == div && i == lines.size() - 1 && j == lines.back().size() - lines[1].size() + 1)
                break;
            LISTDIGIT::iterator pnt = findDigitByKey(lines[i][j]);
            if(pnt != digitSet.end())     //already in digitSet
            {
                Coordinate co;          //locate digit
                co.h_loc = i;
                co.v_loc = lines[i].length()-1-j;
                if(getExpression() == mul && i<h_lines.size()-1 && i>=2) //乘积左移
                    co.v_loc += i - 2;
                else if(getExpression() == div && i<h_lines.size()-1 && i>=4) //div乘积
                    co.v_loc += (i - 3)/2;
                pnt->addNum(co);
            }
            else   //not found
            {
                Coordinate co;          //locate digit
                co.h_loc = i;
                co.v_loc = lines[i].length()-1-j;
                if(getExpression() == mul && i<h_lines.size()-1 && i>=2) //乘积左移
                    co.v_loc += i - 2;
                else if(getExpression() == div && i<h_lines.size()-1 && i>=4) //div乘积
                    co.v_loc += (i - 3)/2;
                Digit newDigit(lines[i][j],co);    //new
                if(lines[i][j].isNumber())  //已知数字
                {
                    newDigit.setConstant();
                    int val = (int)lines[i][j].unicode() - 48;
                    newDigit.setCurrentValue(val);
                    newDigit.setDomain(QVector<int>(1,val));
                }
                digitSet.push_back(newDigit);
            }
        }
    }
}
int Equation::getSearchNum()
{
    int search_num = 0;
    search_num += carrySet.size();
    for(int i = 0; i<digitSet.size(); i++)
    {
        if(digitSet[i].getState() != Digit::CONSTANT)
            search_num++;
    }
    return search_num;
}

int Equation::getRuleNum()  //每一个算式横线下面的字母，就是一个rule
{
    int rule_num = 0;

    switch(_exp)
    {
    case add:
    case sub:
        rule_num = h_lines.back().size();
        break;

    case mul:
    {
        for(int i = 2; i<h_lines.size(); i++)
        {
            rule_num += h_lines[i].size();
        }
        break;
    }

    case div:
    {
        for(int i = 3; i<h_lines.size(); i++)
        {
            rule_num += h_lines[i].size();
        }
        break;
    }
    }

    return rule_num;
}

bool Equation::isNull(LISTDIGIT::iterator it)
{
    if(it == digitSet.end() || it==carrySet.end())
        return true;
    else
        return false;
}

bool Equation::onRuleId(LISTDIGIT::iterator it, int ruleId)
{
    QVector<Coordinate> CoVec;
    CoVec = it->getLocation();
    for(int i = 0; i<CoVec.size(); i++)
    {
        QVector<int> rules = getRuleId(CoVec[i],it->getKey());
        for(int i = 0; i<rules.size(); i++)
        if(rules[i] == ruleId)
            return true;
    }
    return false;
}

bool Equation::checkUnique()
{
    //只对searched的数字进行判断。
    QVector<LISTDIGIT::iterator> vset,vfilt;
    for (LISTDIGIT::iterator i = digitSet.begin(); i != digitSet.end(); i++)
    {
        vset.push_back(i);
    }
    vfilt = filterState(vset,Digit::SEARCHED);
    for (int i = 0; i < vfilt.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            if(vfilt[i]->getCurrentValue() == vfilt[j]->getCurrentValue())
                return false;
        }
    }
    return true;
}
QVector<int> Equation::getRuleId(Coordinate loc,QChar key)
{
    //通过 数字所在的位置，得到 该位置需要满足的规则序号
    switch(_exp)
    {
    case add:
    case sub:
    {
        QVector<int> rules;
        rules.append(loc.v_loc);
        if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
        {
            rules.append(loc.v_loc - 1);
        }
        return rules;
    }
        break;
    case mul:
    {
        QVector<int> rules;
        if(loc.h_loc == 0)
        {
            int base = 0;   //计数基础
            for(int i = 2; i<h_lines.size() - 1; i++)   //被乘数
            {
                rules.append(loc.v_loc + base); //需要加上每一行的数字个数
                base += h_lines[i].size();
            }
        }
        else if(loc.h_loc == 1) //乘数
        {
            int base = 0;   //计数基础
            for(int i = 2; i<2+loc.v_loc; i++)
            {
                base += h_lines[i].size();
            }
            for(int i = 0; i<h_lines[2+loc.v_loc].size(); i++)
            {
                rules.append(i + base);
            }
        }
        else if(loc.h_loc == h_lines.size() - 1)    //在加法结果部分
        {
            if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
            {
                int base = 0;   //计数基础
                for(int i = 2; i<h_lines.size() - 1; i++)
                {
                    base += h_lines[i].size();
                }
                rules.append(base + loc.v_loc);
                rules.append(base + loc.v_loc - 1);
            }
            else
            {
                int base = 0;   //计数基础
                for(int i = 2; i<h_lines.size() - 1; i++)
                {
                    base += h_lines[i].size();
                }
                rules.append(base + loc.v_loc);
            }
        }
        else    //在乘积中间结果部分
        {
            if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
            {
                int base = 0;   //计数基础
                for(int i = 2; i<h_lines.size() - 1; i++)
                {
                    if(i == loc.h_loc)
                    {
                        rules.append(base + loc.v_loc - loc.h_loc + 1); //产生进位
                        rules.append(base + loc.v_loc - loc.h_loc + 2); //进位 与 乘积结果相加
                        break;
                    }
                    base+=h_lines[i].size();
                }
            }
            else    //digit
            {
                int base = 0;   //计数基础
                for(int i = 2; i<h_lines.size() - 1; i++)
                {
                    if(i == loc.h_loc)
                    {
                        rules.append(base + loc.v_loc - loc.h_loc + 2);
                    }
                    base+=h_lines[i].size();
                }
                rules.append(base + loc.v_loc);
            }

        }
        return rules;
    }
        break;
    case div:
    {
        QVector<int> rules;
        if(loc.h_loc == 0)//被乘数
        {
            int base = 0;   //计数基础
            for(int i = 3; i<h_lines.size() - 1; i+=2)
            {
                rules.append(loc.v_loc + base); //需要加上每一行的数字个数
                base += h_lines[i].size();
                base += h_lines[i+1].size();
            }
        }
        else if(loc.h_loc == 1) //乘数
        {
            int base = 0;   //计数基础
            for(int i = 3; i<3 + loc.v_loc * 2; i++)
            {
                base += h_lines[i].size();
            }
            for(int i = 0; i<h_lines[3 + loc.v_loc * 2].size(); i++)
            {
                rules.append(i + base);
            }
        }
        else if(loc.h_loc % 2 == 0)    //在加法规则部分
        {
            if(loc.h_loc == 2)  //余数
            {
                rules.append(loc.v_loc);
            }
            else if(loc.h_loc == h_lines.size() - 1)    //最终结果
            {
                int base = 0;
                for(int i = 3; i<h_lines.size() - 1; i++)
                    base += h_lines[i].size();
                rules.append(base + loc.v_loc - h_lines[1].size() + 1);
                if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
                {
                    rules.append(base + loc.v_loc - h_lines[1].size());
                }
            }
            else    //中间结果
            {
                int base = 0;
                for(int i = 3; i<loc.h_loc; i++)
                    base += h_lines[i].size();
                rules.append(base + loc.v_loc - loc.h_loc / 2 + 2);     //上加和
                if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
                {
                    rules.append(base + loc.v_loc - loc.h_loc / 2 + 1);
                }
                else
                {
                    if(loc.v_loc != loc.h_loc / 2 - 2)
                    {
                        base += h_lines[loc.h_loc].size();
                        base += h_lines[loc.h_loc + 1].size();
                        rules.append(base + loc.v_loc - loc.h_loc / 2 + 1); //下加和
                    }
                }

            }
        }
        else    //在乘积中间结果部分
        {
            if(key >= CARRYKEYBEGIN && key<=CARRYKEYEND)    //carrybit需要独立考虑
            {
                int base = 0;   //计数基础
                for(int i = 3; i<loc.h_loc; i++)
                    base += h_lines[i].size();
                rules.append(base + loc.v_loc - loc.h_loc / 2 + 1);
                rules.append(base + loc.v_loc - loc.h_loc / 2);
            }
            else    //digit
            {
                int base = 0;   //计数基础
                for(int i = 3; i<loc.h_loc; i++)
                    base += h_lines[i].size();
                rules.append(base + loc.v_loc - loc.h_loc / 2 + 1);
                base += h_lines[loc.h_loc].size();
                rules.append(base + loc.v_loc - loc.h_loc / 2 + 1);
            }

        }
        return rules;
    }
        break;

    }
}

bool Equation::checkAddRule(int start_h, int end_h, int vloc)
{
    //start_h 表示加式中 第一个加数 所在的行，end_h 表示加式中 最后一个加数 所在的行
    //vloc代表加式的列
    QVector<int> addTerms;
    QVector<LISTDIGIT::iterator> add_its;
    for(int j = start_h; j < end_h + 1; j++)
    {
        add_its.append(findDigitByloc(Coordinate(j,vloc)));
    }
    for(int j = 0; j<add_its.size(); j++)
    {
        if(isNull(add_its[j]))
            addTerms.append(0);
        else
        {
            if(add_its[j]->getState() == Digit::UNSEARCHED)
                return true;
            addTerms.append(add_its[j]->getCurrentValue());
        }
    }
    int carry0,carry1,sum;
    LISTDIGIT::iterator carry0_it = findCarryByloc(Coordinate(end_h + 1,vloc));
    LISTDIGIT::iterator sum_it = findDigitByloc(Coordinate(end_h + 1,vloc));
    LISTDIGIT::iterator carry1_it = findCarryByloc(Coordinate(end_h + 1,vloc + 1));
    if(isNull(carry0_it)) carry0 = 0; else {if(carry0_it->getState() == Digit::UNSEARCHED) return true; carry0 = carry0_it->getCurrentValue();}
    if(isNull(sum_it)) sum = 0; else {if(sum_it->getState() == Digit::UNSEARCHED) return true; sum = sum_it->getCurrentValue();}

    int adds = 0;
    for(int j = 0; j<addTerms.size(); j++)
    {
        adds += addTerms[j];
    }

    int res = adds + carry0 - sum;
    if(res >= 10)
    {
        if(res%10 != 0)
            return false;
        if(isNull(carry1_it))
            return false;
        else if(carry1_it->getState() == Digit::UNSEARCHED)
            return true;
        else
            carry1 = carry1_it->getCurrentValue();
        return res == 10*carry1;
    }
    else    //不应进位
    {
        if(res != 0)
            return false;
        else
        {
            if(isNull(carry1_it))
                return res == 0;
            else if(carry1_it->getState() != Digit::UNSEARCHED)  //已知
            {
                carry1 = carry1_it->getCurrentValue();
                return carry1 == 0;
            }
        }
        return true;
    }
}
bool Equation::checkMulRule(int v1, int v2, int hloc, int vloc)
{
    //v1表示被乘数的列，v2表示乘数的列
    //hloc,vloc表示乘积结果的位置坐标
    QVector<int> mulTerms;
    QVector<LISTDIGIT::iterator> mul_its;
    mul_its.append(findDigitByloc(Coordinate(0,v1))); //被乘数
    mul_its.append(findDigitByloc(Coordinate(1,v2))); //乘数

    for(int j = 0; j<mul_its.size(); j++)
    {
        if(isNull(mul_its[j]))
            mulTerms.append(0);
        else
        {
            if(mul_its[j]->getState() == Digit::UNSEARCHED)
                return true;
            mulTerms.append(mul_its[j]->getCurrentValue());
        }
    }
    int carry0 = 0,carry1 = 0,product = 0;
    LISTDIGIT::iterator carry0_it = findCarryByloc(Coordinate(hloc,vloc));
    LISTDIGIT::iterator product_it = findDigitByloc(Coordinate(hloc,vloc));
    LISTDIGIT::iterator carry1_it = findCarryByloc(Coordinate(hloc,vloc + 1));
    if(isNull(carry0_it)) carry0 = 0; else {if(carry0_it->getState() == Digit::UNSEARCHED) return true; carry0 = carry0_it->getCurrentValue();}
    if(isNull(product_it)) product = 0; else {if(product_it->getState() == Digit::UNSEARCHED) return true; product = product_it->getCurrentValue();}

    int muls = 1;
    for(int j = 0; j<mulTerms.size(); j++)
    {
        muls *= mulTerms[j];
    }
    int res = muls + carry0 - product;
    //即便carry1未搜索，也能判断true or false
    if(res >= 10)   //应当进位
    {
        if(res%10 != 0)
            return false;
        if(isNull(carry1_it))   //没有进位
            return false;
        else if(carry1_it->getState() == Digit::UNSEARCHED)
            return true;
        else
            carry1 = carry1_it->getCurrentValue();
        return res == 10*carry1;
    }
    else    //不应进位
    {
        if(res != 0)    //提前剪枝（即使carry1未曾搜索）
            return false;
        else
        {
            if(isNull(carry1_it))
                return res == 0;
            else if(carry1_it->getState() != Digit::UNSEARCHED)  //已知
            {
                carry1 = carry1_it->getCurrentValue();
                return carry1 == 0;
            }
        }
        return true;
    }
}
bool Equation::checkRule(int ruleId)
{
    switch(_exp)
    {
    case add:
    case sub:
    {
        return checkAddRule(0,h_lines.size() - 2,ruleId);
        break;
    }
    case mul:
    {
        Coordinate co = Rule2Loc(ruleId);
        if(co.h_loc == h_lines.size() - 1)  //最后一行
        {
            return checkAddRule(2,co.h_loc - 1,co.v_loc);
        }
        else    //乘式行
        {
            return checkMulRule(co.v_loc - co.h_loc + 2,co.h_loc - 2,co.h_loc,co.v_loc);
        }
        break;
    }

    case div:
    {
        Coordinate co = Rule2Loc(ruleId);
        if(co.h_loc % 2 == 0)  //加式行
        {
            return checkAddRule(co.h_loc - 2,co.h_loc - 1,co.v_loc);
        }
        else    //乘式行
        {
            return checkMulRule(co.v_loc - co.h_loc/2 + 1,(co.h_loc-3)/2,co.h_loc,co.v_loc);
        }
        break;
    }

    }
}

bool Equation::isSafe(QChar key)
{
    if(checkUnique() == false)
        return false;
     LISTDIGIT::iterator digit_it = findDigitByKey(key);
     QVector<Coordinate> CoVec;
     CoVec = digit_it->getLocation();
     QVector<int> rules;
     for(int i = 0; i<CoVec.size(); i++)
     {
         QVector<int>candRule = getRuleId(CoVec[i],key);
         for(int j=0; j<candRule.size(); j++)
             if(rules.contains(candRule[j]) == false)
             {
                 rules.push_back(candRule[j]);
             }
     }
     for(int i = 0; i<rules.size(); i++)
     {
         if(checkRule(rules[i]) == false)
             return false;
     }
     return true;
}

bool Equation::firstDigitConstrain()
{
    bool srkFlag = false;
    for(int i = 0; i<(int)h_lines.size(); i++)
    {
        if(h_lines[i].size() > 1)
        {
            QChar key = h_lines[i][0];
            LISTDIGIT::iterator pList = findDigitByKey(key);
            if(pList != digitSet.end() && pList->inDomain(0))
            {
                pList->shrinkDomain(0);
                srkFlag = true;
            }
        }
    }
    return srkFlag;
}

void Equation::createAddCarry(int curr_h, int lshift, int num)
{
    //num 代表参与 加和 的数字个数
    for(int j = 0; j<h_lines[curr_h].size() - 1; j++)
    {
        Coordinate co;          //locate digit
        co.h_loc = curr_h;
        co.v_loc = j + 1 + lshift;
        Digit newCarry(QChar(_carryNum + CARRYKEYBEGIN),co);
        _carryNum++;
        QVector<bool> dom;   //为了处理多行加法;dom为carry的可取值
        for(int k = 0; k<num; k++)
        {
            dom.append(true);
        }
        newCarry.setDomain(dom);
        carrySet.push_back(newCarry);
    }
}
void Equation::createMulCarry(int curr_h, int lshift)
{
    for(int j = 0; j<h_lines[curr_h].size() - 1; j++)
    {
        Coordinate co;          //locate digit
        co.h_loc = curr_h;
        co.v_loc = j + 1 + lshift;   //结果左移
        Digit newCarry(QChar(_carryNum + CARRYKEYBEGIN),co);
        _carryNum++;
        carrySet.push_back(newCarry);
    }
}
void Equation::createCarry()
{
    switch(_exp)
    {
        case add:
        case sub:
        {
            createAddCarry(h_lines.size() - 1, 0, h_lines.size() - 1);
            break;
        }
        case mul:
        {
            for(int i = 2; i<2+h_lines[1].size(); i++)
                createMulCarry(i,i-2);
            if(h_lines.size()>3)
                createAddCarry(h_lines.size() - 1, 0, h_lines.size() - 3);
//            for(int i = 2; i<h_lines.size() - 1; i++)
//                createMulCarry(i,i-2);
//            createAddCarry(h_lines.size() - 1, 0, h_lines.size() - 3);
            break;
        }
        case div:
        {
            for(int i = 3; i<h_lines.size(); i++)
            {
                if((i - 3)%2 == 0)  //乘法结果
                {
                    createMulCarry(i,(i-3)/2);
                }
                else
                {
                    createAddCarry(i,(i-4)/2,2);
                }
            }
            break;
        }
    }

}

void Equation::Initialize(QVector<QString> lines,Expression ex)
{
    iter_num = 0;
    setExpression(ex);
    switch (ex) {
        case add:
        {
            h_lines = lines;
            parsing(h_lines);
            createCarry();
            firstDigitConstrain();  //设置首位非0
            break;
        }
        case sub:
        {
            h_lines = lines;
            QString str = h_lines[0];   //交换第一行和倒数第一行
            h_lines[0] = h_lines.back();
            h_lines.back() = str;
            parsing(h_lines);
            createCarry();
            firstDigitConstrain();  //设置首位非0
            break;
        }
        case mul:
        {
            h_lines = lines;
            if(lines.size() == 3)
                h_lines.append(lines.back());
            parsing(h_lines);
            createCarry();
            firstDigitConstrain();  //设置首位非0
            break;
        }
        case div:
        {
            h_lines = lines;
            parsing(h_lines);
            //把被除数的后几位不参与计算的数字删掉，减少复杂度。
            h_lines.back().remove(h_lines.back().size() - h_lines[1].size() + 1,h_lines[1].size() - 1);
            createCarry();
            firstDigitConstrain();
            break;
        }
    }
}

bool CompareDigitNum(Digit a, Digit b)
{
    return a.getNum() > b.getNum();
}
LISTDIGIT::iterator Equation::nextDigit()
{

    int rule_num = getRuleNum();
    int candRule = -1;  //the candidate rule with least unsearched digits
    QVector<LISTDIGIT::iterator> candList;  //the digit&carry list with least unsearched digits
    int minDiff = 10000;
    QVector<int> diff(rule_num,0);  //save the number of unsearched digits
    for(int i = 0; i<rule_num; i++)
    {
        QVector<LISTDIGIT::iterator> vset_d = findDigitByRuleId(i);
        QVector<LISTDIGIT::iterator> vset_c = findCarryByRuleId(i);
        QVector<LISTDIGIT::iterator> filt_d = filterState(vset_d,Digit::UNSEARCHED);
        QVector<LISTDIGIT::iterator> filt_c = filterState(vset_c,Digit::UNSEARCHED);
        diff[i] = filt_d.size() + filt_c.size();    //the number of unsearched digits&carries
        if(diff[i] < minDiff && diff[i] != 0)
        {
            minDiff = diff[i];
            candRule = i;   //找到了一个未知数最少的Rule
            filt_d.append(filt_c);
            candList.clear();
            candList.append(filt_d);
        }
    }

    if(candRule == -1)     //every digit&carry is searched
    {
        return digitSet.end();
    }
    LISTDIGIT::iterator candDigit;
    int minDomain = 10000;
    for (int i = 0; i < candList.size(); ++i)
    {
        if(candList[i]->getDomainSize() < minDomain)
        {
            minDomain = candList[i]->getDomainSize();
            candDigit = candList[i];
        }
    }
    return candDigit;
}

void Equation::printResult()
{
    LISTDIGIT::iterator i;
    QFile fConsol;
    // 打开控制台输出
    fConsol.open(stdout, QIODevice::WriteOnly);
    QTextStream writeTextConsol(&fConsol);

    for (i = digitSet.begin(); i != digitSet.end(); ++i)
    {
       writeTextConsol<<i->getKey()<<"----"<<i->getCurrentValue()<<endl;
    }
    cout<<"success!"<<endl;
}

bool Equation::search(int n/*要搜索的digit+carry的总数*/, int m/*搜索过的数*/)
{
    if(result.size() == 0)
        iter_num++;
    if(m == n)  //搜索结束
    {
        QVector<DigitMap> temp;
        for(int i = 0; i<digitSet.size(); i++)
        {
            if(digitSet[i].getState() == Digit::CONSTANT)
                continue;
            DigitMap map;
            map.key = digitSet[i].getKey();
            map.value = digitSet[i].getCurrentValue();
            temp.append(map);
        }
        result.append(temp);
        printResult();
        return true;
    }
    else
    {
        LISTDIGIT::iterator currDigit = nextDigit();
        QVector<bool> domain = currDigit->getDomain();
        for(int i = 0; i<domain.size(); i++)  //遍历
        {
            if(domain[i] == false)
                continue;
            //find a safe digit
            int cand = i;
            currDigit->setSearched();   //设置查找标记
            currDigit->setCurrentValue(cand);
            if(!isSafe(currDigit->getKey())) //不满足要求
                continue;
            //递归搜索
                search(n,m+1);
        }
        currDigit->setUnsearched();     //恢复未查找标记
        return false;
    }
}
