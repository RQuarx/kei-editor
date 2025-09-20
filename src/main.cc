#include <memory>

#include "ui/context/context.hh"
#include "ui/object/circle.hh"
#include "ui/object/box.hh"
#include "ui/editor.hh"

#include "core/config.hh"
#include "core/logger.hh"
#include "core/utils.hh"


auto
main( int /* p_argc */, char ** /* p_argv */ ) -> int
{
    auto logger { Logger::create(utils::getenv("LOG_LEVEL"),
                                 utils::getenv("LOG_FILE")) };
    auto config { Config::create(logger, utils::getenv("CONFIG_PATH")) };
    auto ctx    { std::make_shared<ui::Context>(config) };

    event::Handler event { logger };
    event.add_handler(SDL_EVENT_QUIT,
        EVENT_SIGNATURE( (void)p_event; return SDL_APP_SUCCESS; ));

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

        result = editor.render();
        if (result != SDL_APP_CONTINUE) break;

        ctx->render_window();
    }

    return result == SDL_APP_SUCCESS ? 0 : 1;
}