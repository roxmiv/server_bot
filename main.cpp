#include "poliz_reader.h"
#include <iostream>

int main()
{
    ArifmeticPolizReader reader;
    std::string input;
    std::cin >> input;
    //std::cout << reader.ConvertString(input) << std::endl;
    try
    {
        Reader reader2;
        reader2.Run(input);
    }
    catch (const std::runtime_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return 0;
}
