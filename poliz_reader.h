#ifndef POLIZ_READER_H
#define POLIZ_READER_H

#include <stdexcept>
#include <stack>
#include <string>
#include <iostream>

class ReaderException: public std::exception
{
public:
    ReaderException(const std::string& error) : m_errorText(error)
    {}
    virtual const char* what() const noexcept
    {
        return m_errorText.c_str();
    }
private:
    std::string m_errorText;
};

class UnexpectedLanguageStructureException: public ReaderException
{
public:
    UnexpectedLanguageStructureException(const std::string& structureName): ReaderException("Unexpected language structure: " + structureName)
    {}
};

class UnexpectedSymbolException: public ReaderException
{
public:
    UnexpectedSymbolException(char c): ReaderException("Unexpectred symbol: " + c)
    {}
};

class UnexpectedFileEndException: public ReaderException
{
public:
    UnexpectedFileEndException(): ReaderException("Unexpectred end of file")
    {}
};

class EmptyIdentifierException: public ReaderException
{
public:
    EmptyIdentifierException(): ReaderException("Empty identifier")
    {}
};

class Reader
{
public:
    void Run(const std::string& filename);
private:
    void ReadCommand();
    void ReadFunction();
    void ReadMark();
    void ReadAssignment();
    void ReadLanguageStructure();

    int ReadFunctionArguments();
    void ReadArifmeticExpression();
    void ReadLogicExpression();
    std::string ReadIdentifier();

    bool isSpace()
    {
        return ((m_cur == ' ') || (m_cur == '\t') || (m_cur == '\n'));
    }

    bool isNumber()
    {
        return ((m_cur >= '0') && (m_cur <= '9'));
    }

    bool isLitera()
    {
        return ((m_cur >= 'a') && (m_cur <= 'z') || (m_cur >= 'A') && (m_cur <= 'Z'));
    }

    bool isArifmeticLogic()
    {
        return ((m_cur == '+')
                || (m_cur == '-')
                || (m_cur == '*')
                || (m_cur == '/')
                || (m_cur == '%')
                || (m_cur == '<')
                || (m_cur == '>')
                || (m_cur == '=')
                || (m_cur == ':'));
    }

    void AddFunction(const std::string& value)
    {
        std::cout << value << std::endl;
    }

    void GetNextChar()
    {
        m_currentPos++;
        if (m_currentPos >= m_programText.length())
            throw UnexpectedFileEndException();
        m_cur = m_programText[m_currentPos];
    }

    void ClearSpaces()
    {
        while (isSpace())
            GetNextChar();
    }

private:
    typedef std::stack<char> SymbolStack;

    std::string m_programText;
    unsigned int m_currentPos;
    char m_cur;
};

#endif // POLIZ_READER_H
