#include "poliz.h"
#include <stdexcept>

//--------------------------------------------------------------------------------------

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
    PolizInt* i1 = dynamic_cast<PolizInt*>(operand1);
    PolizInt* i2 = dynamic_cast<PolizInt*>(operand2);
    if (!i1)
        throw std::runtime_error("Not an int");
    if (!i2)
        throw std::runtime_error("Not an int");
    return GetPolizGenericConst(i1->Get() + i2->Get());
}

//--------------------------------------------------------------------------------------

void PolizOpGo::Evaluate(PolizStack& stack, PolizItem& currentCommand) const
{
    PolizElem* operand = Pop(stack);
    PolizLabel* label = dynamic_cast<PolizLabel*>(operand);
    if (!label)
        throw std::runtime_error("Not a label");
    currentCommand = label->Get();
    delete operand;
}
