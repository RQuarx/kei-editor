#include <memory>

#include "interface/context.hh"
#include "interface/editor.hh"
#include "config.hh"
#include "logger.hh"
#include "utils.hh"


auto
main( int /* p_argc */, char ** /* p_argv */ ) -> int
{
    logger = std::make_unique<Logger>(utils::getenv("LOG_LEVEL"),
                                      utils::getenv("LOG_FILE"));

    auto config { std::make_shared<Config>(utils::getenv("CONFIG_PATH")) };

    ui::Context ctx { config };
    ctx->add_handler(SDL_EVENT_QUIT,
        []( ui::Context &, SDL_Event * ) { return SDL_APP_SUCCESS; });

    ui::Editor editor { config, { .x=20.0F, .y=20.0F, .w=50.0F, .h=50.0F } };

    editor.add_event_handlers(ctx);

    bool running { true };
    SDL_AppResult result;
    while (running) {
        if (result != SDL_APP_CONTINUE) break;

        result = ctx->poll_events(ctx);
        if (result != SDL_APP_CONTINUE) break;

        result = editor.render(ctx);
        ctx.set_draw_color(config->get_string("window.color.bg"));
        ctx.render();
    }

    return result == SDL_APP_SUCCESS ? 0 : 1;
}