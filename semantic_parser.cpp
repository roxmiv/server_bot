#include "semantic_parser.h"

using namespace syntax_parser;
namespace semantic_parser
{
    void SemanticParser::ReadCommands()
    {
        while (true)
        {
            if (m_cur.GetType() == LEX_VAR)
            {
                ReadAssignment();
            }
            else if (m_cur.GetType() == LEX_FUNC)
            {
                ReadFunction();
                if (m_cur.GetType() != LEX_SEMICOLON)
                    throw SemanticParserException("Wrong expression");
            }
            else if (m_cur.GetType() == LEX_IF)
            {
                ReadIf();
            }
            else if (m_cur.GetType() == LEX_WHILE)
            {
                ReadWhile();
            }
            else if (m_cur.GetType() == LEX_END)
            {
                return;
            }
            GetNextLexem();
        }
    }

    void SemanticParser::ReadFunction()
    {
        Lexema function = m_cur;
        GetNextLexem();
        if (m_cur.GetType() != LEX_LPAREN)
            throw SemanticParserException("Wrong expression");
        GetNextLexem();
        ReadArifmeticExpression();
        while (m_cur.GetType() != LEX_RPAREN)
        {
            if (m_cur.GetType() != LEX_COMMA)
                throw SemanticParserException("Wrong expression");
            GetNextLexem();
            ReadArifmeticExpression();
        }
        //put function to poliz
        GetNextLexem();
    }

    void SemanticParser::ReadAssignment()
    {
        //put cur to poliz
        GetNextLexem();
        if (m_cur.GetType() != LEX_ASIGN)
            throw SemanticParserException("Wrong expression");
        GetNextLexem();
        ReadArifmeticExpression();
        //put assign to poliz
        if (m_cur.GetType() != LEX_SEMICOLON)
            throw SemanticParserException("Wrong expression");
    }

    void SemanticParser::Run(const syntax_parser::LexemsVector& lexems)
    {
        m_lexems = lexems;
        m_currentPos = 0;
        m_cur = m_lexems[0];
        ReadCommands();
    }

} //ns
