#include "poliz.h"
#include <stdexcept>

void PolizConst::Evaluate(PolizStack& stack, PolizItem& currentCommand) const
{
    Push(stack, Clone());
    ++currentCommand;
}

void PolizFunction::Evaluate(PolizStack& stack, PolizItem& currentCommand) const
{
    PolizElem* res = EvaluateFun(stack);
    if (res)
        Push(stack, res);
    currentCommand++;
}

PolizElem* PolizFunPlus::EvaluateFun(PolizStack& stack) const
{
    PolizElem* operand1 = Pop(stack);
    PolizElem* operand2 = Pop(stack);
    PolizGenericConst<int>* i1 = dynamic_cast<PolizGenericConst<int>*>(operand1);
    PolizGenericConst<int>* i2 = dynamic_cast<PolizGenericConst<int>*>(operand2);
    if (!i1)
        throw std::runtime_error("Not an int");
    if (!i2)
        throw std::runtime_error("Not an int");
    int res = i1->Get() + i2->Get();
    return new PolizGenericConst<int>(res);
}

void PolizOpGo::Evaluate(PolizStack& stack, PolizItem& currentCommand) const
{
    PolizElem* operand = Pop(stack);
    PolizLabel* label = dynamic_cast<PolizLabel*>(operand);
    if (!label)
        throw std::runtime_error("Not a label");
    currentCommand = label->Get();
    delete operand;
}
