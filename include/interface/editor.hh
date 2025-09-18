#pragma once
#include <memory>
#include <SDL3/SDL_init.h>

class Config;


namespace ui
{
    class Context;


    struct Area
    {
        float x { 0.0F };
        float y { 0.0F };
        float w { 0.0F };
        float h { 0.0F };


        [[nodiscard]]
        auto is_within( const Area &p_pos ) const -> bool;


        [[nodiscard]]
        auto is_inside( const Area &p_rect ) const -> bool;


        [[nodiscard]]
        auto to_rect() const -> SDL_FRect;
    };


    class Editor
    {
    public:
        Editor( const std::shared_ptr<Config> &p_conf, SDL_FRect p_area );
        ~Editor();


        void add_event_handlers( Context &p_ctx );


        [[nodiscard]]
        auto render( Context &p_ctx ) -> SDL_AppResult;

    private:
        SDL_Cursor *m_default_cursor;
        SDL_Cursor *m_beam_cursor;

        std::shared_ptr<Config> m_config;
        SDL_FRect m_area;

        bool m_active;
        bool m_cursor_set;


        auto on_mouse_hover( Context   &p_ctx,
                             SDL_Event *p_event ) -> SDL_AppResult;


        auto on_text_input( Context   &p_ctx,
                            SDL_Event *p_event ) -> SDL_AppResult;
    };
}