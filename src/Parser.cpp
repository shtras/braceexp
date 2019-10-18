#include "Parser.h"

#include <cctype>

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

MultToken::MultToken(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> MultToken::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        if (b_.Peek() == '{' && res.empty()) {
            b_.Skip();
            ListToken listToken(b_);
            auto pos = b_.Pos();
            auto alist = listToken.Parse();
            res.insert(res.begin(), alist.begin(), alist.end());
            if (listToken.HasError() || b_.Peek() != '}' || b_.Pos() == pos) {
                error_ = true;
            }
            b_.Skip();
            break;
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

ExprToken::ExprToken(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> ExprToken::Parse()
{
    std::list<std::string> res;
    while (!b_.Eof()) {
        MultToken multToken(b_);
        auto right = multToken.Parse();
        if (multToken.HasError()) {
            error_ = true;
        }
        if (right.empty()) {
            break;
        }
        std::list<std::string> res1;
        if (!res.empty()) {
            for (const auto& ls : res) {
                for (const auto& rs : right) {
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

ListToken::ListToken(Utils::Buffer& b)
    : Token(b)
{
}

std::list<std::string> ListToken::Parse()
{
    std::list<std::string> res;
    size_t pos = 0;
    while (!b_.Eof()) {
        ExprToken expToken(b_);
        auto blist = expToken.Parse();
        res.insert(res.end(), blist.begin(), blist.end());
        if (expToken.HasError()) {
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

bool Parser::Parse(std::string_view s)
{
    Utils::Buffer buf(s);
    ExprToken expression(buf);
    res_ = expression.Parse();
    error_ = expression.HasError() || !buf.Eof();
    return !error_;
}

void Parser::Flush(std::ostream& s)
{
    if (!error_) {
        size_t idx = 0;
        for (const auto& t : res_) {
            s << t;
            if (++idx < res_.size()) {
                s << " ";
            }
        }
        s << "\n";
    }
}
} // namespace BraceExpand
