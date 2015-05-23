#ifndef SINTAX_PARSER_H
#define SINTAX_PARSER_H

#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <iostream>

namespace syntax_parser
{

class SyntaxParserException: public std::exception
{
public:
    SyntaxParserException(const std::string& error) : m_errorText(error)
    {}
    virtual const char* what() const noexcept
    {
        return m_errorText.c_str();
    }
private:
    std::string m_errorText;
};

class UnexpectedLanguageStructureException: public SyntaxParserException
{
public:
    UnexpectedLanguageStructureException(const std::string& structureName): SyntaxParserException("Unexpected language structure: " + structureName)
    {}
};

class UnexpectedSymbolException: public SyntaxParserException
{
public:
    UnexpectedSymbolException(char c): SyntaxParserException("Unexpectred symbol: " + c)
    {}
};

class UnexpectedFileEndException: public SyntaxParserException
{
public:
    UnexpectedFileEndException(): SyntaxParserException("Unexpectred end of file")
    {}
};

class EmptyIdentifierException: public SyntaxParserException
{
public:
    EmptyIdentifierException(): SyntaxParserException("Empty identifier")
    {}
};

enum LexemType
{
    LEX_NONE = 0,
    LEX_BEGIN = 1,
    LEX_END,
    LEX_IF,
    LEX_THEN,
    LEX_ELSE,
    LEX_WHILE,
    LEX_DO,

    LEX_ASIGN,
    LEX_PLUS,
    LEX_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_PROC,
    LEX_AND,
    LEX_OR,
    LEX_NOT,
    LEX_EQ,
    LEX_NEQ,
    LEX_LESS,
    LEX_LEQ,
    LEX_GRE,
    LEX_GEQ,
    LEX_COMMA,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_SEMICOLON,
    LEX_COLON,

    LEX_MARK,
    LEX_VAR,
    LEX_FUNC,
    LEX_CONST_STRING,
    LEX_CONST_INT,
    LEX_CONST_DOUBLE
};

class Lexema
{
public:
    Lexema (LexemType type, const std::string& value = ""): m_type(type), m_value(value)
    {}
    LexemType GetType()
    {
        return m_type;
    }
    std::string GetValue()
    {
        return m_value;
    }
private:
    LexemType m_type;
    std::string m_value;
};

typedef std::vector<Lexema> LexemsVector;

class SyntaxParser
{
public:
    SyntaxParser();
    void Run(const std::string& filename);
private:
    void MainCircle();
    void ReadNumber();
    void ReadIdentifier();
    void ReadStringConst();
    void ReadLanguageStructure();
    void ReadSymbols();

    std::string ReadWord();

    bool isSpace()
    {
        return ((m_cur == ' ') || (m_cur == '\t') || (m_cur == '\n') || (m_cur == '\r'));
    }

    bool isDigit()
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
                || (m_cur == ':')
                || (m_cur == ',')
                || (m_cur == ';')
                || (m_cur == '{')
                || (m_cur == '}')
                || (m_cur == '(')
                || (m_cur == ')'));
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

    LexemType GetLexemaByText(const std::string& text);

private:
    typedef std::map<std::string, LexemType> WordLexemMap;

    std::string m_programText;
    unsigned int m_currentPos;
    char m_cur;

    LexemsVector m_lexems;
    WordLexemMap m_words;
};

} //ns
#endif // SINTAX_PARSER_H
