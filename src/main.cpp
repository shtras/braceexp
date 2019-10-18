#include <iostream>
#include "Parser.h"
int main()
{
    std::string in;
    std::getline(std::cin, in);
    BraceExpand::Parser p;
    p.Parse(in);
    p.Flush(std::cout);
    return 0;
}
