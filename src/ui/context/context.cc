#include "ui/context/context.hh"

#include <fontconfig/fontconfig.h>

#include "ui/context/exception.hh"
#include "ui/object/box.hh"
#include "ui/font.hh"
#include "core/config.hh"

using ui::CtxException;
using ui::Context;
static constexpr SDL_WindowFlags WINDOW_FLAGS {
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT };


Context::Context( const std::shared_ptr<Config> &p_config ) :
    config(p_config)
{
    FcInit();
    if (!SDL_Init(SDL_INIT_VIDEO))
        CtxException("Failed to initialize SDL: {}", SDL_GetError());
    if (!TTF_Init())
        CtxException("Failed to initialize SDL_TTF: {}", SDL_GetError());


    auto w { config->get_int("window.size.width") };
    auto h { config->get_int("window.size.height") };
    const auto title { config->get_string("window.title") };

    if (!SDL_CreateWindowAndRenderer(title.c_str(), w, h, WINDOW_FLAGS,
                                    &window, &render))
        CtxException("Failed to create window: {}", SDL_GetError());

    SDL_SetRenderVSync(render, 1);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);

    fill_fonts();
}


Context::~Context()
{
    TTF_Quit();
    SDL_Quit();
}


auto
Context::operator[]( const std::string &p_key ) -> ttf::Font &
{ return fonts.at(p_key); }


auto
Context::get_renderer() const -> SDL_Renderer *
{ return render; }


void
Context::set_draw_color( const Color &p_color ) const
{ SDL_SetRenderDrawColor(render, TO_PARAMS(p_color)); }


void
Context::render_window() const
{
    set_draw_color(config->get_string("window.color.bg"));
    SDL_RenderPresent(render);
    SDL_RenderClear(render);
}


void
Context::toggle_text_input() const
{
    if (SDL_TextInputActive(window))
        SDL_StopTextInput(window);
    else
        SDL_StartTextInput(window);
}


void
Context::fill_fonts()
{ fonts.emplace("editor.font", ttf::Font(config->get_string("editor.font"))); }