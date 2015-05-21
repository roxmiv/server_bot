#ifndef POLIZ_READER_H
#define POLIZ_READER_H

#include <stdexcept>
#include <stack>
#include <string>

class ArifmeticPolizReader
{
public:
    ArifmeticPolizReader(){}

    std::string ConvertString(const std::string& input)
    {
        std::string result;
        SymbolStack stack;
        unsigned int size = input.length();
        char value;

        stack.push('(');

        for (unsigned int i = 0; i < size; i++)
        {
            if (input[i] == '(')
                m_stack.push('(');
            else if (input[i] == ')')
            {
                while ((stack.size() > 0) && ((value = stack.top()) != '('))
                {
                    result += value;
                    stack.pop();
                }
                if (stack.size() == 0)
                    raise std::runtime_error("Wrong expression");
                stack.pop();
            }
            else if ((input[i] == '*') || (input[i] == '/'))
            {

            }
        }
    }

private:
    typedef std::stack<char> SymbolStack;
};

#endif // POLIZ_READER_H
