#pragma once

#include <string>
#include <list>
#include <ostream>

#include "Utils/Utils.h"

/*
Grammar:
L: A+
A: [a-zA-Z]+|{B}
B: L(,L)*

ExprToken: MultToken+ // multiply those
MultToken: [a-zA-Z]|{ListToken}
ListToken: ExprToken,ExprToken,ExprToken,...
*/
namespace BraceExpand
{
class Token
{
public:
    Token(Utils::Buffer& b);
    virtual ~Token() = default;
    std::list<std::string>& GetParts();

protected:
    Utils::Buffer& b_;
    std::list<std::string> parts_;
};

class MultToken : public Token
{
public:
    MultToken(Utils::Buffer& b);

    bool Parse();

private:
};

class ExprToken : public Token
{
public:
    ExprToken(Utils::Buffer& b);

    bool Parse();

private:
};

class ListToken : public Token
{
public:
    ListToken(Utils::Buffer& b);

    bool Parse();

private:
};

class Parser
{
public:
    bool Parse(std::string_view s);
    void Flush(std::ostream& s);

private:
    std::list<std::string> res_;
    bool success_ = false;
};
} // namespace BraceExpand
