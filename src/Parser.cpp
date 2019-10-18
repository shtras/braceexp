#include "Parser.h"

namespace BraceExpand
{
Token::Token(Utils::Buffer& b)
    : b_(b)
{
}

bool Token::HasError()
{
    return error_;
}

L::L(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> L::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        if (b_.Peek() == '{' && res.empty()) {
            b_.Skip();
            A a(b_);
            auto pos = b_.Pos();
            auto alist = a.Parse();
            res.insert(res.begin(), alist.begin(), alist.end());
            if (a.HasError() || b_.Peek() != '}' || b_.Pos() == pos) {
                error_ = true;
            }
            b_.Skip();
        } else if (std::isalpha(b_.Peek())) {
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

B::B(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> B::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        L l(b_);
        auto right = l.Parse();
        if (l.HasError()) {
            error_ = true;
        }
        if (right.empty()) {
            break;
        }
        std::list<std::string> res1;
        if (!res.empty()) {
            for (auto& ls : res) {
                for (auto& rs : right) {
                    res1.push_back(ls + rs);
                }
            }
            std::swap(res, res1);
        } else {
            std::swap(res, right);
        }
    }
    return res;
}

A::A(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> A::Parse()
{
    std::list<std::string> res;
    size_t pos = 0;
    while (!b_.Eof()) {
        B b(b_);
        auto blist = b.Parse();
        res.insert(res.end(), blist.begin(), blist.end());
        if (b.HasError()) {
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
    return res;
}

Parser::Parser(std::string& s)
    : b_(s)
{
}

bool Parser::Parse()
{
    B b(b_);
    res_ = b.Parse();
    error_ = b.HasError() || !b_.Eof();
    return !error_;
}

void Parser::Flush(std::ostream& s)
{
    if (!error_) {
        size_t idx = 0;
        for (auto& t : res_) {
            s << t;
            if (++idx < res_.size()) {
                s << " ";
            }
        }
    }
    s << "\n";
}
} // namespace BraceExpand
