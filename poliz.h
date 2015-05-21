#ifndef POLIZ_H
#define POLIZ_H

#include <stack>
#include <list>
#include <string>

class PolizElem;

typedef std::list<PolizElem*> PolizList;
typedef PolizList::iterator PolizItem;
typedef std::stack<PolizItem> PolizStack;

class PolizElem
{
public:
    virtual void Evaluate(PolizStack& stack, PolizItem& currentCommand) const = 0;
    virtual ~PolizElem() {}
protected:
    static void Push(PolizStack& stack, const PolizElem* elem);
    static PolizElem* Pop(PolizStack& stack);
};

//--------------------------------------------------------------------------------------------------

class PolizConst: public PolizElem
{
public:
    virtual PolizElem* Clone() const = 0;
    virtual ~PolizConst() {}
    virtual void Evaluate(PolizStack& stack, PolizItem& currentCommand) const;
};

template <class T>
class PolizGenericConst: public PolizConst
{
public:
    PolizGenericConst(const T& value): m_value(value)
    {}
    virtual ~PolizGenericConst(){}
    virtual PolizElem* Clone() const
    {
        return new PolizGenericConst(m_value);
    }
    T Get() const
    {
        return m_value;
    }
private:
    T m_value;
};

typedef PolizGenericConst<PolizItem> PolizLabel;

//--------------------------------------------------------------------------------------------------

class PolizFunction: public PolizElem
{
public:
    virtual ~PolizFunction() {}
    virtual PolizElem* EvaluateFun(PolizStack& stack) const = 0;
private:
    virtual void Evaluate(PolizStack& stack, PolizItem& currentCommand) const;
};

class PolizFunPlus: public PolizFunction
{
public:
    PolizFunPlus() {}
    virtual ~PolizFunPlus() {}
    virtual PolizElem* EvaluateFun(PolizStack& stack) const;
};

//--------------------------------------------------------------------------------------------------

class PolizOpGo: public PolizElem
{
public:
    PolizOpGo() {}
    virtual ~PolizOpGo() {}
    virtual void Evaluate(PolizStack& stack, PolizItem& currentCommand) const;
};

#endif // POLIZ_H
