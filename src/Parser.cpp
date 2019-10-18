#include "Parser.h"

#include <cctype>

namespace BraceExpand
{
Token::Token(Utils::Buffer& b)
    : b_(b)
{
}

std::list<std::string>& Token::GetParts()
{
    return parts_;
}

MultToken::MultToken(Utils::Buffer& b)
    : Token(b)
{
}

bool MultToken::Parse()
{
    while (!b_.Eof()) {
        if (b_.Peek() == '{' && parts_.empty()) {
            b_.Skip();
            ListToken listToken(b_);
            auto pos = b_.Pos();
            bool res = listToken.Parse();
            std::swap(parts_, listToken.GetParts());
            if (!res || b_.Peek() != '}' || b_.Pos() == pos) {
                return false;
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
    return true;
}

ExprToken::ExprToken(Utils::Buffer& b)
    : Token(b)
{
}

bool ExprToken::Parse()
{
    while (!b_.Eof()) {
        MultToken multToken(b_);
        bool res = multToken.Parse();
        if (!res) {
            return false;
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
    return true;
}

ListToken::ListToken(Utils::Buffer& b)
    : Token(b)
{
}

bool ListToken::Parse()
{
    size_t pos = 0;
    while (!b_.Eof()) {
        ExprToken expToken(b_);
        bool res = expToken.Parse();
        if (!res) {
            return false;
        }
        auto& nextList = expToken.GetParts();
        parts_.splice(parts_.end(), nextList);
        if (b_.Peek() != ',') {
            if (pos == b_.Pos()) {
                return false;
            }
            break;
        }
        b_.Skip();
        pos = b_.Pos();
    }
    return true;
}

bool Parser::Parse(std::string_view s)
{
    Utils::Buffer buf(s);
    ExprToken expression(buf);
    bool res = expression.Parse();
    std::swap(res_, expression.GetParts());
    success_ = res && buf.Eof();
    return success_;
}

void Parser::Flush(std::ostream& s)
{
    if (success_) {
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
