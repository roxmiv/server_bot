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

template <class T>
class PolizGenericConst: public PolizElem
{
public:
    PolizGenericConst(const T& value): m_value(value)
    {}
    virtual ~PolizGenericConst(){}
    virtual void Evaluate(PolizStack& stack, PolizItem& currentCommand) const
    {
        Push(stack, Clone());
        ++currentCommand;
    }
    virtual PolizElem* Clone() const
    {
        return new PolizGenericConst<T>(m_value);
    }
    T Get() const
    {
        return m_value;
    }

private:
    T m_value;
};

typedef PolizGenericConst<PolizItem> PolizLabel;
typedef PolizGenericConst<int> PolizInt;
typedef PolizGenericConst<double> PolizDouble;
typedef PolizGenericConst<std::string> PolizString;

template<class T>
PolizGenericConst<T>* GetPolizGenericConst (T value)
{
    return new PolizGenericConst<T>(value);
}

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
