#include "parseCursor.h"

ParseCursor::ParseCursor(const std::string_view input) noexcept : m_copy(input) {}

bool ParseCursor::empty() const {
    return m_copy.empty();
}

std::size_t ParseCursor::getCurrentPosition() {
    return m_offset;
}

std::string_view ParseCursor::getRemaining() {
    return m_copy.substr(m_offset);
}

std::string_view ParseCursor::getRest() {

}
