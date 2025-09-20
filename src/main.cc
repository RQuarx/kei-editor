#include <memory>

#include "ui/context/context.hh"
#include "ui/object/circle.hh"
#include "ui/object/box.hh"
#include "ui/editor.hh"

#include "core/config.hh"
#include "core/logger.hh"
#include "core/utils.hh"


namespace
{
    inline bool is_focussed { true };


    auto
    render( const ui::shared_ctx &p_ctx,
            ui::Editor           &p_editor ) -> SDL_AppResult
    {
        SDL_AppResult res { p_editor.render() };
        if (res != SDL_APP_CONTINUE) return res;

        p_ctx->render_window();
        return SDL_APP_CONTINUE;
    }


    auto
    on_window_focus( SDL_Event * ) -> SDL_AppResult
    { is_focussed = true; return SDL_APP_CONTINUE; }

    auto
    on_window_unfocus( SDL_Event * ) -> SDL_AppResult
    { is_focussed = false; return SDL_APP_CONTINUE; }
}


auto
main( int /* p_argc */, char ** /* p_argv */ ) -> int
{
    auto logger { Logger::create(utils::getenv("LOG_LEVEL"),
                                 utils::getenv("LOG_FILE")) };
    auto config { Config::create(logger, utils::getenv("CONFIG_PATH")) };
    auto ctx    { std::make_shared<ui::Context>(config) };


    event::Handler event { logger };
    event.add_handler(SDL_EVENT_QUIT,
        EVENT_LAMB_SIGNATURE( (void)p_event; return SDL_APP_SUCCESS; ));

    event.add_handler(SDL_EVENT_WINDOW_SHOWN, on_window_focus);
    event.add_handler(SDL_EVENT_WINDOW_FOCUS_GAINED, on_window_focus);
    event.add_handler(SDL_EVENT_WINDOW_ENTER_FULLSCREEN, on_window_focus);
    event.add_handler(SDL_EVENT_WINDOW_RESIZED, on_window_focus);

    event.add_handler(SDL_EVENT_WINDOW_FOCUS_LOST, on_window_unfocus);

    int w;
    int h;
    SDL_GetWindowSizeInPixels(ctx->window, &w, &h);

    auto editor_box { ui::Box::rounded({ 5, 5 },
                    { static_cast<float>(w - 10), static_cast<float>(h - 10) },
                    0xffffff_rgb, 15) };
    ui::Editor editor { logger, config, ctx, editor_box };
    editor.add_event_handlers(event);

    bool running { true };
    SDL_AppResult result;
    while (running) {
        result = event.poll_events();
        if (result != SDL_APP_CONTINUE) break;

        if (is_focussed) {
            logger->DEBUG("Render");
            render(ctx, editor);
        }
    }

    return result == SDL_APP_SUCCESS ? 0 : 1;
}