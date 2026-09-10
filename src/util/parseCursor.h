#pragma once

#include <string>

/*
 *  Parses a string
 */

class ParseCursor {
public:
    explicit ParseCursor(std::string_view input) noexcept;

    bool empty() const;

    std::size_t getCurrentPosition();

    std::string_view getRemaining();

    std::string_view getRest();

    char peek(std::size_t lookahead = 0);

    bool startsWith(char element);

    bool consume(char element);

    std::string_view takeUntil(char delimiter);

    template<typename Func>
    std::string_view takeUntil(Func func);

    template<typename Func>
    void takeWhile(Func func);

    template<typename Func>
    void skipWhile(Func func);

private:
    void advance(std::size_t count);

private:
    std::string_view    m_copy{};
    std::size_t         m_offset{0};

};
