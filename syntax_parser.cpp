#include "sintax_parser.h"

#include <fstream>
#include <iostream>

namespace syntax_parser
{
SyntaxParser::SyntaxParser()
{
    m_words["goto"] = LEX_GOTO;
    m_words["if"] = LEX_IF;
    m_words["then"] = LEX_THEN;
    m_words["else"] = LEX_ELSE;
    m_words["while"] = LEX_WHILE;
    m_words["do"] = LEX_DO;

    m_words["="] = LEX_ASIGN;
    m_words["=="] = LEX_EQ;
    m_words["!="] = LEX_NEQ;
    m_words["<"] = LEX_LESS;
    m_words[">"] = LEX_GRE;
    m_words[">="] = LEX_GEQ;
    m_words["<="] = LEX_LEQ;

    m_words["&&"] = LEX_AND;
    m_words["||"] = LEX_OR;
    m_words["!"] = LEX_NOT;
    m_words["+"] = LEX_PLUS;
    m_words["-"] = LEX_MINUS;
    m_words["/"] = LEX_SLASH;
    m_words["%"] = LEX_PROC;
    m_words["*"] = LEX_TIMES;
    m_words[","] = LEX_COMMA;
    m_words[";"] = LEX_SEMICOLON;
    m_words[":"] = LEX_COLON;
    m_words["{"] = LEX_BEGIN;
    m_words["}"] = LEX_END;
    m_words[")"] = LEX_RPAREN;
    m_words["("] = LEX_LPAREN;
}

LexemType SyntaxParser::GetLexemaByText(const std::string& text)
{
    WordLexemMap::const_iterator it = m_words.find(text);
    if (it == m_words.end())
        return LEX_NONE;
    return (it->second);
}

void SyntaxParser::Run(const std::string& filename)
{
    m_lexems.clear();
    std::ifstream t(filename);

    t.seekg(0, std::ios::end);
    m_programText.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    m_programText.assign((std::istreambuf_iterator<char>(t)),
                            std::istreambuf_iterator<char>());
    if (m_programText.empty())
        throw SyntaxParserException("Empty program");
    m_currentPos = 0;
    m_cur = m_programText[0];
    MainCircle();
}

void SyntaxParser::MainCircle()
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
        if ((m_cur == '@') || (m_cur == '$') || (m_cur == '?'))
        {
            ReadIdentifier();
        }
        else if ((isDigit()) || (m_cur == '-'))
        {
            ReadNumber();
        }
        else if (m_cur == '"')
        {
            ReadStringConst();
        }
        else if (isLitera())
        {
            ReadLanguageStructure();
        }
        else
        {
            ReadSymbols();
        }
    }
}

void SyntaxParser::ReadSymbols()
{
    std::string word(&m_cur, 1);
    if ((m_cur == '<') || (m_cur == '>') || (m_cur == '=') || (m_cur == '!'))
    {
        GetNextChar();
        if (m_cur == '=')
        {
            word += m_cur;
            GetNextChar();
        }
    }
    else
    {
        GetNextChar();
    }
    LexemType type = GetLexemaByText(word);
    if (type != LEX_NONE)
        m_lexems.push_back(Lexema(type));
    else
        throw UnexpectedLanguageStructureException(word);
}

void SyntaxParser::ReadLanguageStructure()
{
    std::string word = ReadWord();
    LexemType type = GetLexemaByText(word);
    if (type != LEX_NONE)
        m_lexems.push_back(Lexema(type));
    else
        throw UnexpectedLanguageStructureException(word);
}

void SyntaxParser::ReadIdentifier()
{
    LexemType type;
    if (m_cur == '@')
        type = LEX_MARK;
    if (m_cur == '?')
        type = LEX_FUNC;
    if (m_cur == '$')
        type = LEX_VAR;
    GetNextChar();
    std::string value = ReadWord();
    m_lexems.push_back(Lexema(type, value));
}

void SyntaxParser::ReadStringConst()
{
    std::string value;
    GetNextChar();
    while (m_cur != '"')
    {
        value += m_cur;
        GetNextChar();
    }
    GetNextChar();
    m_lexems.push_back(Lexema(LEX_CONST_STRING, value));
}

void SyntaxParser::ReadNumber()
{
    std::string value;
    if (m_cur == '-')
    {
        value = "-";
        GetNextChar();
    }
    while (isDigit())
    {
       value += m_cur;
       GetNextChar();
    }
    m_lexems.push_back(Lexema(LEX_CONST_INT, value));
}

std::string SyntaxParser::ReadWord()
{
    std::string word;
    while (true)
    {
        if ((isLitera()) || (isDigit()) || (m_cur == '_'))
        {
            word += m_cur;
            GetNextChar();
        }
        else
        {
            if (word.empty())
                throw EmptyIdentifierException();
            return word;
        }
    }
}

} //ns
