#include "digit.h"

void Digit::addNum(Coordinate location)
{
    _locList.push_back(location);
    _num++;
}

bool Digit::shrinkDomain(int del)
{
    if(del>=0 && del<_domain.size())
    {
        _domain[del] = false;
        return true;
    }
    return false;   //del not in original domain
}

bool Digit::recoverDomain(int rec)
{
    if(rec>=0 && rec<_domain.size())
    {
        _domain[rec] = true;
        return true;
    }
    return false;   //rec not in original domain
}

int Digit::getDomainSize()
{
    int num = 0;
    for(int i = 0; i<_domain.size(); i++)
    {
        if(_domain[i] == true)
            num++;
    }
    return num;
}

void Digit::setDomain(QVector<int> dom)
{
    _domain = QVector<bool>(10,false);
    QVector<int>::iterator i;
    for (i = dom.begin(); i != dom.end(); ++i)
    {
        if(*i >= 0 && *i<10)
        {
            _domain[*i] = true;
        }
    }
}

bool Digit::onVloc(const int vloc)
{
    QVector<Coordinate>::iterator i;
    for (i = this->_locList.begin(); i != this->_locList.end(); ++i)
    {
        if(i->v_loc == vloc)
            return true;
    }
    return false;
}

int Digit::findCandidate()
{
    for(int i = 0; i<_domain.size(); i++)
    {
        if(_domain[i] == true)
            return i;
    }
    return -1;      //定义域为空
}
