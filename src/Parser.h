#pragma once

#include <string>
#include <list>
#include <ostream>

#include "Utils/Utils.h"

/*
L: [a-zA-Z]|{A}
A: B,B,B
B: LLL          --- multiply
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

class L : public Token
{
public:
    L(Utils::Buffer& b);
    std::list<std::string> Parse();

private:
};

class B : public Token
{
public:
    B(Utils::Buffer& b);

    std::list<std::string> Parse();

private:
};

class A : public Token
{
public:
    A(Utils::Buffer& b);

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
