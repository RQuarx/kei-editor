#pragma once
#include <memory>
#include <SDL3/SDL_init.h>
#include "core/config.hh"
#include "core/logger.hh"
#include "ui/object/box.hh"

namespace event { class Handler; }


namespace ui
{
    struct Context;
    using shared_ctx = std::shared_ptr<Context>;


    class Editor
    {
    public:
        Editor( shared_logger p_logger,
                shared_config p_config,
                shared_ctx    p_ctx,
                Box           p_area );
        ~Editor();


        void add_event_handlers( event::Handler &p_handler );


        [[nodiscard]]
        auto render() -> SDL_AppResult;

    private:
        shared_logger m_logger;
        shared_config m_config;

        shared_ctx m_ctx;

        SDL_Cursor *m_default_cursor;
        SDL_Cursor *m_beam_cursor;

        Box m_area;

        bool m_active;
        bool m_cursor_set;

        std::string m_text;


        auto on_mouse_hover( SDL_Event *p_event ) -> SDL_AppResult;
        auto on_text_input ( SDL_Event *p_event ) -> SDL_AppResult;
    };
}