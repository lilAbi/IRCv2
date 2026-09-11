#pragma once

#include <string>

/*
 *  Parses a string
 */

class ParseCursor {
public:
    explicit ParseCursor(std::string_view input) noexcept;

    bool empty() const;

    std::size_t offset() const;

    std::string_view remaining() const;

    std::string_view takeRemaining();

    char peek(std::size_t lookahead = 0) const;

    bool startsWith(char element) const;

    bool startsWith(std::string_view text) const;

    bool consume(char element);

    bool consume(std::string_view text);

    std::string_view takeUntil(char delimiter);

    template<typename Func>
    std::string_view takeWhile(Func predicate);

    template<typename Func>
    void skipWhile(Func predicate);

private:
    void advance(std::size_t count);

private:
    std::string_view    m_string_view{};
    std::size_t         m_offset{0};
};

template<typename Func>
std::string_view ParseCursor::takeWhile(Func predicate) {
    std::size_t count = 0;
    while ( count < m_string_view.size() && predicate(m_string_view[count]) ) {
        ++count;
    }
    const auto result = m_string_view.substr(0, count);
    this->advance(count);
    return result;
}

template<typename Func>
void ParseCursor::skipWhile(Func predicate) {
    std::size_t count = 0;
    while ( count < m_string_view.size() && predicate(m_string_view[count]) ) {
        ++count;
    }
    this->advance(count);
}
