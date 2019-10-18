#pragma once

#include <string>
#include <list>
#include <ostream>

/*
L: [a-zA-Z]|{A}
A: B,B,B
B: LLL          --- multiply
*/

bool isChar(char c);

class Buffer
{
public:
    Buffer(std::string& s);
    char Peek();
    char Get();
    void Skip(size_t n = 1);
    bool Eof();
    size_t Pos();

private:
    std::string_view s_;
    size_t idx_ = 0;
};

class Token
{
public:
    Token(Buffer& b);
    virtual ~Token() = default;
    bool HasError();

protected:
    Buffer& b_;
    bool error_ = false;
};

class L : public Token
{
public:
    L(Buffer& b);
    std::list<std::string> Parse();

private:
};

class B : public Token
{
public:
    B(Buffer& b);

    std::list<std::string> Parse();

private:
};

class A : public Token
{
public:
    A(Buffer& b);

    std::list<std::string> Parse();

private:
};

class Parser
{
public:
    Parser(std::string& s);
    bool Parse();
    void Flush(std::ostream& s);

private:
    Buffer b_;
    std::list<std::string> res_;
    bool error_ = false;
};

bool test3();
