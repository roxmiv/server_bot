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
    if (m_programText.empty())
        throw ReaderException("Empty program");
    m_currentPos = 0;
    m_cur = m_programText[0];
    ReadCommand();
}

void Reader::ReadCommand()
{
    while (true)
    {
        try
        {
            ClearSpaces();
        }
        catch (UnexpectedFileEndException)
        {
            return;
        }

        if (m_cur == '?')
        {
            ReadFunction();
            ClearSpaces();
            if (m_cur != ';')
            {
                throw UnexpectedSymbolException(m_cur);
            }
        }
        else if (m_cur == '@')
        {
            ReadMark();
        }
        else if (m_cur == '$')
        {
            ReadAssignment();
            ClearSpaces();
            if (m_cur != ';')
            {
                throw UnexpectedSymbolException(m_cur);
            }
        }
        else if (isLitera())
        {
            ReadLanguageStructure();
        }
        else
        {
            throw UnexpectedSymbolException(m_cur);
        }
    }
}

void Reader::ReadLanguageStructure()
{
    //goto @mark_name;
    //if (logic_expression) { command1; command2;} else { command1; command2;}
    //while (logic_expression) { command1; command2;}
    std::string languageStructure = ReadIdentifier();
    ClearSpaces();
    if (languageStructure == "goto")
    {
        if (m_cur != '@')
            throw UnexpectedSymbolException(m_cur);
        GetNextChar();
        std::string markName = ReadIdentifier();
    }
    else if ((languageStructure == "if") or (languageStructure == "while"))
    {
        if (m_cur != '(')
            throw UnexpectedSymbolException(m_cur);
        GetNextChar();
        ReadLogicExpression();
        if (m_cur != ')')
            throw UnexpectedSymbolException(m_cur);
        ClearSpaces();
        if (m_cur != '{')
            throw UnexpectedSymbolException(m_cur);
        GetNextChar();
        ReadCommand();
        if (m_cur != '}')
            throw UnexpectedSymbolException(m_cur);
    }
    else
    {
        throw UnexpectedLanguageStructureException(languageStructure);
    }
}

void Reader::ReadAssignment()
{
    // $variable_name = expression;
    GetNextChar();
    std::string variableName = ReadIdentifier();
    ClearSpaces();
    if (m_cur != '=')
        throw UnexpectedSymbolException(m_cur);
    GetNextChar();
    ReadArifmeticExpression();
}

void Reader::ReadMark()
{
    //  @mark_name:
    GetNextChar();
    std::string markName = ReadIdentifier();
    ClearSpaces();
    if (m_cur != ':')
    {
        throw UnexpectedSymbolException(m_cur);
    }
    GetNextChar();
}

void Reader::ReadFunction()
{
    //  ?func_name (arg_1, arg_2, arg_3);   ?func_name();
    GetNextChar();
    std::string functionName = ReadIdentifier();
    ClearSpaces();
    if (m_cur != '(')
    {
        throw UnexpectedSymbolException(m_cur);
    }
    GetNextChar();
    ReadFunctionArguments();
    GetNextChar();
}

int Reader::ReadFunctionArguments()
{
    int argumentsNumber = 0;
    ClearSpaces();
    while (true)
    {
        ReadArifmeticExpression();
        if (m_cur == ',')
        {
            argumentsNumber++;
        }
        else if (m_cur == ')')
        {
            return argumentsNumber; // Fix numbers of argument
        }
        else
        {
            throw UnexpectedSymbolException(m_cur);
        }
    }
}

void Reader::ReadLogicExpression()
{}

void Reader::ReadArifmeticExpression()
{

}

std::string Reader::ReadIdentifier()
{
    std::string identifier;
    while (true)
    {
        if (isLitera() || isNumber() || (m_cur == '_'))
        {
            identifier += m_cur;
            GetNextChar();
        }
        else
        {
            if (identifier.empty())
                throw EmptyIdentifierException();
            return identifier;
        }
    }
}
