#ifndef GALENA_FONT_HPP
#define GALENA_FONT_HPP

#include <string>

struct TTF_Font;

namespace gla
{
class Font final
{
public:
    explicit Font(std::string const& fullPath, float size);
    ~Font() noexcept;

    [[nodiscard]] auto GetFont() const -> TTF_Font*;

    Font(Font&&) = delete;
    auto operator=(Font const&) -> Font& = delete;
    Font(Font const&) = delete;
    auto operator=(Font&&) -> Font& = delete;

private:
    TTF_Font* m_font;
};
}  // namespace gla

#endif  // GALENA_FONT_HPP