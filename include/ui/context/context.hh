#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>

#include "ui/color.hh"
#include "event/event.hh"

class Config;

namespace ui
{
    namespace ttf { class Font; }
    struct Circle;
    struct Box;


    /** Rendering context. */
    struct Context
    {
        std::shared_ptr<Config> config;

        SDL_Renderer *render;
        SDL_Window   *window;

        std::unordered_map<std::string, ttf::Font> fonts;


        Context( const std::shared_ptr<Config> &p_config );
        ~Context();


        [[nodiscard]]
        auto operator[]( const std::string &p_key ) -> ttf::Font &;


        [[nodiscard]]
        auto get_renderer() const -> SDL_Renderer *;


        void set_draw_color( const Color &p_color ) const;
        void render_window() const;
        void toggle_text_input() const;

    private:
        void fill_fonts();
    };


    using shared_ctx = std::shared_ptr<Context>;
}