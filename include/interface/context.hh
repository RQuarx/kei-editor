#pragma once
#include <unordered_map>
#include <memory>
#include <string>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>

#include "interface/color.hh"
#include "interface/event.hh"

class Config;

namespace ui
{
    namespace ttf { class Font; }

    /**
     * @brief Contains datas necessary to render stuff.
     */
    class Context
    {
    public:

        Context( const std::shared_ptr<Config> &p_config );
        ~Context();


        [[nodiscard]]
        auto operator->() -> EventHandler *;


        [[nodiscard]]
        auto get_renderer() const -> SDL_Renderer *;


        void toggle_text_input();


        void set_draw_color( const Color &p_color );
        void render();

    private:
        std::shared_ptr<Config> m_config;

        SDL_Renderer *m_render;
        SDL_Window   *m_window;

        std::unordered_map<std::string, ttf::Font> m_fonts;
        EventHandler m_event_handler;


        void fill_fonts();
    };
}