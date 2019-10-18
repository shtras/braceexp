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

std::list<std::string>& Token::GetParts()
{
    return parts_;
}

MultToken::MultToken(Utils::Buffer& b)
    : Token(b)
{
}

void MultToken::Parse()
{
    while (!b_.Eof()) {
        if (b_.Peek() == '{' && parts_.empty()) {
            b_.Skip();
            ListToken listToken(b_);
            auto pos = b_.Pos();
            listToken.Parse();
            std::swap(parts_, listToken.GetParts());
            if (listToken.HasError() || b_.Peek() != '}' || b_.Pos() == pos) {
                error_ = true;
            }
            b_.Skip();
            break;
        } else if (std::isalpha(b_.Peek())) {
            if (parts_.empty()) {
                parts_.emplace_back();
            }
            parts_.back() += b_.Get();
        } else {
            break;
        }
    }
}

ExprToken::ExprToken(Utils::Buffer& b)
    : Token(b)
{
}

void ExprToken::Parse()
{
    while (!b_.Eof()) {
        MultToken multToken(b_);
        multToken.Parse();
        if (multToken.HasError()) {
            error_ = true;
        }
        auto& right = multToken.GetParts();
        if (right.empty()) {
            break;
        }
        std::list<std::string> newParts;
        if (!parts_.empty()) {
            for (const auto& ls : parts_) {
                for (const auto& rs : right) {
                    newParts.push_back(ls + rs);
                }
            }
            std::swap(parts_, newParts);
        } else {
            std::swap(parts_, right);
        }
    }
}

ListToken::ListToken(Utils::Buffer& b)
    : Token(b)
{
}

void ListToken::Parse()
{
    size_t pos = 0;
    while (!b_.Eof()) {
        ExprToken expToken(b_);
        expToken.Parse();
        auto& nextList = expToken.GetParts();
        parts_.splice(parts_.end(), nextList);
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
}

bool Parser::Parse(std::string_view s)
{
    Utils::Buffer buf(s);
    ExprToken expression(buf);
    expression.Parse();
    std::swap(res_, expression.GetParts());
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
