#include "parseCursor.h"

ParseCursor::ParseCursor(const std::string_view input) noexcept : m_string_view(input) {}

bool ParseCursor::empty() const {
    return m_string_view.empty();
}

std::size_t ParseCursor::offset() const {
    return m_offset;
}

std::string_view ParseCursor::remaining() const {
    return m_string_view;
}

std::string_view ParseCursor::takeRemaining() {
    const auto result = m_string_view;
    this->advance(m_string_view.size());
    return result;
}

char ParseCursor::peek(const std::size_t lookahead) const {
    return (lookahead >= m_string_view.size()) ? '\0' : m_string_view[lookahead];
}

bool ParseCursor::startsWith(const char element) const {
    return m_string_view.starts_with(element);
}

bool ParseCursor::startsWith(const std::string_view text) const {
    return m_string_view.starts_with(text);
}

bool ParseCursor::consume(const char element) {
    if (!startsWith(element)) return false;
    this->advance(1);
    return true;
}

bool ParseCursor::consume(const std::string_view text) {
    if (!startsWith(text)) return false;
    this->advance(text.size());
    return true;
}

std::string_view ParseCursor::takeUntil(char delimiter) {
    const auto position = m_string_view.find(delimiter);
    if (position == std::string_view::npos) return takeRemaining();
    const auto result = m_string_view.substr(0, position);
    this->advance(position);
    return result;
}

void ParseCursor::advance(const std::size_t count) {
    m_string_view.remove_prefix(count);
    m_offset += count;
}
