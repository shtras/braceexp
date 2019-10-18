#pragma once
#include <string>

namespace Utils
{
class Buffer
{
public:
    Buffer(std::string_view s);
    char Peek();
    char Get();
    void Skip(size_t n = 1);
    bool Eof();
    size_t Pos();

private:
    std::string_view s_;
    size_t idx_ = 0;
};
} // namespace Utils
