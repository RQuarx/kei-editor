#include "ui/context/context.hh"

#include <fontconfig/fontconfig.h>

#include "ui/context/exception.hh"
#include "ui/object/box.hh"

using ui::CtxException;
using ui::WindowData;
using ui::Context;
static constexpr SDL_WindowFlags WINDOW_FLAGS {
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_TRANSPARENT };


auto
WindowData::from_config( shared_config &p_conf ) -> WindowData
{
    return {
        .title=p_conf->get("window.title").asCString(),
        .bg_color=p_conf->get_string("window.color.bg"),
        .w=p_conf->get_int("window.size.width"),
        .h=p_conf->get_int("window.size.height"),
        .transparent=p_conf->get("window.transparent").asBool()
    };
}


auto
Context::create( const WindowData &p_data ) -> shared_ctx
{ return std::make_shared<Context>(p_data); }


Context::Context( const WindowData &p_data ) :
    m_window_bg(p_data.bg_color)
{
    FcInit();

    if (!SDL_Init(SDL_INIT_VIDEO))
        CtxException("Failed to initialize SDL: {}", SDL_GetError());
    if (!TTF_Init())
        CtxException("Failed to initialize SDL_TTF: {}", SDL_GetError());


    if (!SDL_CreateWindowAndRenderer(p_data.title, p_data.w, p_data.h,
                                     WINDOW_FLAGS, &window, &render))
        CtxException("Failed to create window: {}", SDL_GetError());

    SDL_SetRenderVSync(render, 1);
    SDL_SetRenderDrawBlendMode(render, SDL_BLENDMODE_BLEND);
}


Context::~Context()
{
    TTF_Quit();
    SDL_Quit();
}


auto
Context::get_window_size() const -> TPair<float>
{
    int w { 0 };
    int h { 0 };
    SDL_GetWindowSizeInPixels(window, &w, &h);
    return Pair<float, float>::create(w, h);
}


void
Context::set_draw_color( const Color &p_color ) const
{ SDL_SetRenderDrawColor(render, TO_PARAMS(p_color)); }


void
Context::render_window() const
{
    set_draw_color(m_window_bg);
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