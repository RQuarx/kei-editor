#include "interface/context.hh"

#include <fontconfig/fontconfig.h>

#include "interface/font.hh"
#include "config.hh"
#include "logger.hh"

using ui::Context;
static constexpr SDL_WindowFlags WINDOW_FLAGS {
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT };


Context::Context( const std::shared_ptr<Config> &p_config ) :
    m_config(p_config),
    m_event_handler(p_config)
{
    FcInit();
    logger->INFO("Initializing SDL");
    if (!SDL_Init(SDL_INIT_VIDEO))
        logger->FATAL("Failed to initialize SDL: {}", SDL_GetError());
    if (!TTF_Init())
        logger->FATAL("Failed to initialize SDL_TTF: {}", SDL_GetError());


    auto w { m_config->get_int("window.size.width") };
    auto h { m_config->get_int("window.size.height") };
    const auto title { m_config->get_string("window.title") };

    logger->INFO("Creating window");
    if (!SDL_CreateWindowAndRenderer(title.c_str(), w, h, WINDOW_FLAGS,
                                    &m_window, &m_render))
        logger->FATAL("Failed to create window: {}", SDL_GetError());

    SDL_SetRenderVSync(m_render, 1);
    SDL_SetRenderDrawBlendMode(m_render, SDL_BLENDMODE_BLEND);

    fill_fonts();
}


Context::~Context()
{
    TTF_Quit();
    SDL_Quit();
}


auto
Context::operator->() -> EventHandler *
{ return &m_event_handler; }


auto
Context::get_renderer() const -> SDL_Renderer *
{ return m_render; }


void
Context::toggle_text_input()
{
    if (SDL_TextInputActive(m_window))
        SDL_StopTextInput(m_window);
    else
        SDL_StartTextInput(m_window);
}


void
Context::set_draw_color( const Color &p_color )
{ SDL_SetRenderDrawColor(m_render, TO_PARAMS(p_color)); }


void
Context::render()
{
    SDL_RenderPresent(m_render);
    SDL_RenderClear(m_render);
}


void
Context::fill_fonts()
{
    m_fonts.emplace("editor.font",
                     ttf::Font(m_config->get_string("editor.font")));
}