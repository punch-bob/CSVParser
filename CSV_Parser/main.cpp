#include <iostream>
#include <fstream>
#include "General.h"
#include "CSVParser.h"

int main()
{
    std::tuple<int, std::string, int> t;
    std::ifstream in("in.txt");
    CSVParser<int, std::string, int> p(in, 0);
    try
    {
        for (std::tuple<int, std::string, int> iter : p)
        {
            std::cout << iter << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}