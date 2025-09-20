#pragma once
#include <filesystem>
#include <string>
#include <SDL3_ttf/SDL_ttf.h>
#include "core/exception.hh"


namespace ui::ttf
{
    class FontException : public Exception
    { public: using Exception::Exception; };


    namespace fs = std::filesystem;


    static constexpr float DEFAULT_FONT_SIZE { 12.0F };


    class Font
    {
    public:
        Font( const std::string &p_name );
        Font( const Font & ) = delete;
        Font( Font &&p_other ) noexcept;

        ~Font();

        auto operator=( const Font & ) -> Font & = delete;
        auto operator=( Font &&p_other ) noexcept -> Font &;


        [[nodiscard]]
        auto get() const noexcept -> TTF_Font *;


        [[nodiscard]]
        static auto get_path_from_name( const std::string &p_name ) -> fs::path;

    private:
        TTF_Font *m_font { nullptr };
    };
}