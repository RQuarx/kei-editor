#include "ui/font.hh"

#include <filesystem>

#include <fontconfig/fontconfig.h>

using ui::ttf::Font;


Font::Font( const std::string &p_name ) :
    m_font(TTF_OpenFont(get_path_from_name(p_name).c_str(), DEFAULT_FONT_SIZE))
{
    if (m_font == nullptr)
        throw FontException("Failed to open font {}: {}",
                             p_name, SDL_GetError());
}


Font::Font( Font &&p_other ) noexcept :
    m_font(p_other.m_font)
{ p_other.m_font = nullptr; }


Font::~Font()
{
    if (m_font != nullptr) {
        // TTF_CloseFont(m_font);
        // m_font = nullptr;
    }
}


auto
Font::operator=( Font &&p_other ) noexcept -> Font &
{
    if (this != &p_other) {
        if (m_font != nullptr) TTF_CloseFont(m_font);
        m_font = p_other.m_font;
        p_other.m_font = nullptr;
    }

    return *this;
}


auto
Font::get_path_from_name( const std::string &p_name ) -> fs::path
{
    if (fs::exists(p_name) && fs::is_regular_file(p_name))
        return p_name;

    FcConfig *config { FcConfigGetCurrent() };
    const auto *name { reinterpret_cast<const FcChar8 *>(p_name.c_str()) };

    FcPattern *pattern { FcNameParse(name) };
    if (pattern == nullptr)
        throw FontException("Failed to parse font name {}", p_name);

    FcConfigSubstitute(config, pattern, FcMatchPattern);
    FcDefaultSubstitute(pattern);

    FcResult result { FcResult::FcResultNoId };
    FcPattern *font { FcFontMatch(config, pattern, &result) };
    if (font != nullptr) {
        FcChar8 *file { nullptr };

        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch) {
            fs::path result { reinterpret_cast<char *>(file) };
            FcPatternDestroy(font);
            FcPatternDestroy(pattern);
            return result;
        }
        FcPatternDestroy(font);
    }
    FcPatternDestroy(pattern);
    return {};
}


auto
Font::get() const noexcept -> TTF_Font *
{ return m_font; }