#include <iostream>
#include "Parser.h"
int main()
{
    std::string in;
    std::cin >> in;
    BraceExpand::Parser p(in);
    p.Parse();
    p.Flush(std::cout);
    return 0;
}
