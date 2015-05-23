#ifndef SEMANTIC_PARSER_H
#define SEMANTIC_PARSER_H

#include "sintax_parser.h"
#include <stack>
#include <stdexcept>

namespace semantic_parser
{

class SemanticParserException: public std::exception
{
public:
    SemanticParserException(const std::string& error) : m_errorText(error)
    {}
    virtual const char* what() const noexcept
    {
        return m_errorText.c_str();
    }
private:
    std::string m_errorText;
};

class SemanticParser
{
public:
    void Run(const syntax_parser::LexemsVector& lexems);
private:
    void ReadCommands();
    void ReadFunction();
    void ReadAssignment();
    void ReadIf() {}
    void ReadWhile() {}
    void ReadArifmeticExpression() {}

    void GetNextLexem()
    {
        m_currentPos++;
        if (m_currentPos >= m_lexems.size())
            throw SemanticParserException("Wrong expression");
        m_cur = m_lexems[m_currentPos];
    }

private:
    typedef std::stack<syntax_parser::Lexema> LexemStack;

    LexemStack m_stack;
    syntax_parser::LexemsVector m_lexems;
    int m_currentPos;
    syntax_parser::Lexema m_cur;
};
} // ns

#endif // SEMANTIC_PARSER_H
