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
    bool HasError();

protected:
    Utils::Buffer& b_;
    bool error_ = false;
};

class MultToken : public Token
{
public:
    MultToken(Utils::Buffer& b);

    std::list<std::string> Parse();

private:
};

class ExprToken : public Token
{
public:
    ExprToken(Utils::Buffer& b);

    std::list<std::string> Parse();

private:
};

class ListToken : public Token
{
public:
    ListToken(Utils::Buffer& b);

    std::list<std::string> Parse();

private:
};

class Parser
{
public:
    bool Parse(std::string_view s);
    void Flush(std::ostream& s);

private:
    std::list<std::string> res_;
    bool error_ = false;
};
} // namespace BraceExpand
