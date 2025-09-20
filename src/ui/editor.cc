#include "ui/editor.hh"

#include "ui/context/context.hh"
#include "event/event.hh"
#include "core/config.hh"

using ui::Editor;


Editor::Editor( shared_logger p_logger,
                shared_config p_config,
                shared_ctx    p_ctx,
                Box           p_area ) :
    m_logger(std::move(p_logger)),
    m_config(std::move(p_config)),
    m_ctx(std::move(p_ctx)),
    m_default_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT)),
    m_beam_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT)),
    m_area(p_area),
    m_cursor_set(false)
{}


Editor::~Editor()
{
    SDL_DestroyCursor(m_beam_cursor);
    SDL_DestroyCursor(m_default_cursor);
}


void
Editor::add_event_handlers( event::Handler &p_handler )
{
    p_handler.add_handler(SDL_EVENT_MOUSE_MOTION,
                         *this, &ui::Editor::on_mouse_hover);

    p_handler.add_handler(SDL_EVENT_TEXT_INPUT,
                         *this, &ui::Editor::on_text_input);
}


auto
Editor::on_mouse_hover( SDL_Event *p_event ) -> SDL_AppResult
{
    SDL_FPoint cursor { .x=p_event->motion.x, .y=p_event->motion.y };
    m_active = m_area.contains(cursor);

    if (m_active && !m_cursor_set) {
        m_logger->DEBUG("editor focussed");
        SDL_SetCursor(m_beam_cursor);
        m_cursor_set = true;

        m_ctx->toggle_text_input();
    } else if (!m_active && m_cursor_set) {
        m_logger->DEBUG("editor unfocussed");
        SDL_SetCursor(m_default_cursor);
        m_cursor_set = false;

        m_ctx->toggle_text_input();
    }

    return SDL_APP_CONTINUE;
}


auto
Editor::render() -> SDL_AppResult
{
    m_ctx->set_draw_color(m_config->get_string("editor.color.bg"));

    try {
        m_area.draw(*m_ctx);
    } catch (const std::exception &exception) {
        m_logger->ERROR("{}", exception.what());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}


auto
Editor::on_text_input( SDL_Event *p_event ) -> SDL_AppResult
{
    std::string text { p_event->text.text };
    m_logger->DEBUG("{}", text);
    return SDL_APP_CONTINUE;
}