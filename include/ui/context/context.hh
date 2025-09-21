#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>

#include <core/config.hh>
#include <core/types.hh>
#include "ui/color.hh"
#include "ui/font.hh"

class Config;

namespace ui
{
    namespace ttf { class Font; }
    struct Circle;
    struct Box;

    struct Context;
    using shared_ctx = std::shared_ptr<Context>;


    struct WindowData
    {
        const char *title;
        Color bg_color;
        int w;
        int h;
        bool transparent;


        [[nodiscard]]
        static auto from_config( shared_config &p_conf ) -> WindowData;
    };


    /** Rendering context. */
    struct Context
    {
        SDL_Renderer *render;
        SDL_Window   *window;

        std::unordered_map<std::string, ttf::Font> fonts;


        [[nodiscard]]
        static auto create( const WindowData &p_data ) -> shared_ctx;


        Context( const WindowData &p_data );
        ~Context();


        [[nodiscard]]
        auto get_window_size() const -> TPair<float>;


        void set_draw_color( const Color &p_color ) const;
        void render_window() const;
        void toggle_text_input() const;

    private:
        Color m_window_bg;
    };
}