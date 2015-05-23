#include "sintax_parser.h"
#include "semantic_parser.h"
#include "server_api.h"
#include <iostream>

int main()
{
    std::string input;
    std::cin >> input;
    try
    {
        syntax_parser::SyntaxParser parser;
        parser.Run(input);
    }
    catch (const syntax_parser::SyntaxParserException& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
