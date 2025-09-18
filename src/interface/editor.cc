#include "interface/editor.hh"

#include "interface/context.hh"
#include "interface/event.hh"
#include "config.hh"
#include "utils.hh"

using ui::Editor;


Editor::Editor( const std::shared_ptr<Config> &p_conf, SDL_FRect p_area ) :
    m_default_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT)),
    m_beam_cursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT)),
    m_config(p_conf),
    m_area(p_area),
    m_cursor_set(false)
{}


Editor::~Editor()
{
    SDL_DestroyCursor(m_beam_cursor);
    SDL_DestroyCursor(m_default_cursor);
}


void
Editor::add_event_handlers( Context &p_ctx )
{
    p_ctx->add_handler(SDL_EVENT_MOUSE_MOTION,
                      *this, &Editor::on_mouse_hover);

    p_ctx->add_handler(SDL_EVENT_TEXT_INPUT,
                      *this, &Editor::on_text_input);
}


auto
Editor::on_mouse_hover( Context   &p_ctx,
                        SDL_Event *p_event ) -> SDL_AppResult
{
    SDL_FPoint cursor { .x=p_event->motion.x, .y=p_event->motion.y };
    m_active = utils::is_point_inside_rect(m_area, cursor);

    if (m_active && !m_cursor_set) {
        SDL_SetCursor(m_beam_cursor);
        m_cursor_set = true;

        p_ctx.toggle_text_input();
    } else if (!m_active && m_cursor_set) {
        SDL_SetCursor(m_default_cursor);
        m_cursor_set = false;

        p_ctx.toggle_text_input();
    }

    return SDL_APP_CONTINUE;
}


auto
Editor::render( Context &p_ctx ) -> SDL_AppResult
{
    p_ctx.set_draw_color(m_config->get_string("editor.color.bg"));

    if (!SDL_RenderFillRect(p_ctx.get_renderer(), &m_area))
        return SDL_APP_FAILURE;
    return SDL_APP_CONTINUE;
}


auto
Editor::on_text_input( Context   &p_ctx,
                       SDL_Event *p_event ) -> SDL_AppResult
{
    std::string text { p_event->text.text };
    logger->DEBUG("{}", text);
    return SDL_APP_CONTINUE;
}