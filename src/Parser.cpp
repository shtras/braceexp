#include "Parser.h"

bool isChar(char c)
{
    if (c >= 'a' && c <= 'z') {
        return true;
    }
    if (c >= 'A' && c <= 'Z') {
        return true;
    }
    return false;
}

Buffer::Buffer(std::string& s)
    : s_(s)
{
}

char Buffer::Peek()
{
    if (Eof()) {
        return '\n';
    }
    return s_[idx_];
}

char Buffer::Get()
{
    auto res = Peek();
    Skip();
    return res;
}

void Buffer::Skip(size_t n /*= 1*/)
{
    idx_ += n;
}

bool Buffer::Eof()
{
    return idx_ >= s_.size();
}

size_t Buffer::Pos()
{
    return idx_;
}

Token::Token(Buffer& b)
    : b_(b)
{
}

bool Token::HasError()
{
    return error_;
}

L::L(Buffer& b)
    : Token(b)
{
}

std::list<std::string> L::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        if (b_.Peek() == '{') {
            b_.Skip();
            A l(b_);
            auto pos = b_.Pos();
            l.Parse();
            if (l.HasError() || b_.Peek() != '}' || b_.Pos() == pos) {
                error_ = true;
            }
            b_.Skip();
        } else if (isChar(b_.Peek())) {
            if (res.size() == 0) {
                res.emplace_back();
            }
            res.back() += b_.Get();
        } else {
            break;
        }
    }
    return res;
}

B::B(Buffer& b)
    : Token(b)
{
}

void B::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        L b(b_);
        auto l = b.Parse();
        if (b.HasError()) {
            error_ = true;
        }
        res.insert(res.end(), l.begin(), l.end());
        if (l.empty()) {
            break;
        }
    }
    if (res.empty()) {
        //error_ = true;
    }
}

A::A(Buffer& b)
    : Token(b)
{
}

void A::Parse()
{
    size_t pos = 0;
    while (!b_.Eof()) {
        B a(b_);
        a.Parse();
        if (a.HasError()) {
            error_ = true;
        }
        if (b_.Peek() != ',') {
            if (pos == b_.Pos()) {
                error_ = true;
            }
            break;
        }
        b_.Skip();
        pos = b_.Pos();
    }
}

Parser::Parser(std::string& s)
    : b_(s)
{
}

bool Parser::Parse()
{
    L l(b_);
    l.Parse();
    bool error = l.HasError() || !b_.Eof();
    return !error;
}

void test3()
{
    std::string s{"{A}"};
    Parser p(s);
    bool res = p.Parse();
}
