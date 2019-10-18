#include "Utils.h"

namespace Utils
{
Buffer::Buffer(std::string_view s)
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
} // namespace Utils
