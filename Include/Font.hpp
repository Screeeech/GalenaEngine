#ifndef GALENA_FONT_HPP
#define GALENA_FONT_HPP

#include <string>

struct TTF_Font;

namespace gla
{
class Font final
{
public:
    explicit Font(const std::string& fullPath, float size);
    ~Font() noexcept;

    [[nodiscard]] TTF_Font* GetFont() const;

    Font(const Font&) = delete;
    Font(Font&&) = delete;
    Font& operator=(const Font&) = delete;
    Font& operator=(const Font&&) = delete;

private:
    TTF_Font* m_font;
};
}  // namespace gla

#endif  // GALENA_FONT_HPP