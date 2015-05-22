#include "poliz_reader.h"
#include <fstream>
#include <iostream>

void Reader::Run(const std::string& filename)
{
    std::ifstream t(filename);

    t.seekg(0, std::ios::end);
    m_programText.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    m_programText.assign((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
    m_currentPos = 0;
    ReadMain();
}

void Reader::ReadMain()
{
    char cur;
    while (m_currentPos < m_programText.length())
    {
        cur = m_programText[m_currentPos];
        if (isDelimeter(cur))
        {
            m_currentPos++;
            continue;
        }
        else if (isArifmeticLogic(cur))
        {
            m_currentPos++;
            continue;
        }
        else if (cur == '"')
        {
            ReadString();
        }
        else if (cur == ':')
        {
            ReadAssign();
        }
        else if (isLitera(cur))
        {
            ReadKeyWord();
        }
        else if ((isNumber(cur)) || cur=='-')
        {
            ReadNumber();
        }
        else if ((cur == '?') || (cur == '@') || (cur == '$'))
        {
            ReadIdentifier();
        }
        else
        {
            throw std::runtime_error("Unexpectred symbol: " + cur);
        }
    }
}

void Reader::ReadNumber()
{
    int sign = 1;
    int value = 0;
    char cur = m_programText[m_currentPos];
    if (cur == '-')
    {
        sign = -1;
        m_currentPos++;
    }
    while (m_currentPos < m_programText.length())
    {
        cur = m_programText[m_currentPos];
        if (isNumber(cur))
        {
            value = 10 * value + (cur - '0');
        }
        else if ((isDelimeter(cur)) || (isArifmeticLogic(cur)))
        {
            break;
        }
        else
        {
            throw std::runtime_error("Unexpectred symbol: " + cur);
        }
        m_currentPos++;
    }
    value *= sign;
    std::cout << value << std::endl;
}

void Reader::ReadIdentifier()
{
    int type;
    std::string identifier;
    char cur = m_programText[m_currentPos];
    if (cur == '?')
    {
        type = 1;
    }
    else if (cur == '@')
    {
        type = 2;
    }
    else if (cur == '$')
    {
        type = 3;
    }
    else
    {
        throw std::runtime_error("Unexpectred symbol: " + cur);
    }
    m_currentPos++;
    while (m_currentPos < m_programText.length())
    {
        cur = m_programText[m_currentPos];
        if ((isNumber(cur)) || (isLitera(cur)) || (cur == '_'))
        {
            identifier += cur;
        }
        else if ((isDelimeter(cur)) || (isArifmeticLogic(cur)))
        {
            break;
        }
        else
        {
            throw std::runtime_error("Unexpectred symbol: " + cur);
        }
        m_currentPos++;
    }
    std::cout << identifier << std::endl;
}

void Reader::ReadKeyWord()
{
    std::string keyWord;
    char cur;
    while (m_currentPos < m_programText.length())
    {
        cur = m_programText[m_currentPos];
        if (isLitera(cur))
        {
            keyWord += cur;
        }
        else if ((isDelimeter(cur)) || (isArifmeticLogic(cur)))
        {
            break;
        }
        else
        {
            throw std::runtime_error("Unexpectred symbol: " + cur);
        }
        m_currentPos++;
    }
    std::cout << keyWord << std::endl;
}

void Reader::ReadAssign()
{
    char cur = m_programText[m_currentPos];
    m_currentPos++;
    if ((m_currentPos < m_programText.length()) && ((cur = m_programText[m_currentPos]) == '='))
        return;
    throw std::runtime_error("Unexpectred symbol: " + cur);
}

void Reader::ReadString()
{
    char cur;
    std::string stringConstant;
    m_currentPos++;
    while (m_currentPos < m_programText.length())
    {
        cur = m_programText[m_currentPos];
        m_currentPos++;
        if (cur == '"')
        {
            std::cout << stringConstant << std::endl;
            return;
        }
        stringConstant += cur;
    }
    throw std::runtime_error("Unexpectred end of string constant");
}
