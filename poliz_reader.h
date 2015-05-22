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
                stack.push('(');
            else if (input[i] == ')')
            {
                while ((!stack.empty()) && ((value = stack.top()) != '('))
                {
                    result += value;
                    stack.pop();
                }
                if (stack.empty())
                    throw std::runtime_error("Wrong expression");
                stack.pop();
            }
            else if ((input[i] == '*') || (input[i] == '/'))
            {
                while ((!stack.empty()) && ((stack.top() == '*') || (input[i] == '/')))
                {
                    value = stack.top();
                    result += value;
                    stack.pop();
                }
                stack.push(input[i]);
            }
            else if ((input[i] == '+') || (input[i] == '-'))
            {
                while ((!stack.empty()) && (stack.top() != '('))
                {
                    value = stack.top();
                    result += value;
                    stack.pop();
                }
                stack.push(input[i]);
            }
            else if (isSpace(input[i]))
            {
                continue;
            }
            else
            {
                result += input[i];
            }
        }
        while ((!stack.empty()) && (stack.top() != '('))
        {
            result += stack.top();
            stack.pop();
        }
        if (stack.empty())
            throw std::runtime_error("Wrong expression");
        return result;
    }

private:
    typedef std::stack<char> SymbolStack;

    bool isSpace(char c)
    {
        return ((c == ' ') || (c == '\n') || (c == '\t'));
    }
};

class Reader
{
public:
    void Run(const std::string& filename);
private:
    void ReadMain();
    void ReadNumber();
    void ReadIdentifier();
    void ReadKeyWord();
    void ReadAssign();
    void ReadString();

    bool isDelimeter(char cur)
    {
        return ((cur == ' ') || (cur == '\t') || (cur == '\n') || (cur == ';'));
    }

    bool isNumber(char cur)
    {
        return ((cur >= '0') && (cur <= '9'));
    }

    bool isLitera(char cur)
    {
        return ((cur >= 'a') && (cur <= 'z') || (cur >= 'A') && (cur <= 'Z'));
    }

    bool isArifmeticLogic(char cur)
    {
        return ((cur == '+')
                || (cur == '-')
                || (cur == '*')
                || (cur == '/')
                || (cur == '%')
                || (cur == '<')
                || (cur == '>')
                || (cur == '=')
                || (cur == ':'));
    }

private:
    std::string m_programText;
    unsigned int m_currentPos;
};

#endif // POLIZ_READER_H
