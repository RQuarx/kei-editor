#include "interface/event.hh"

using ui::EventHandler;


EventHandler::EventHandler( const std::shared_ptr<Config> &p_config ) :
    m_config(p_config)
{}


auto
EventHandler::run_handlers( uint32_t   p_key,
                            Context   &p_context,
                            SDL_Event *p_event
                          ) -> std::vector<SDL_AppResult>
{
    auto funcs { m_handlers.at(p_key) };

    std::vector<SDL_AppResult> res;
    res.reserve(funcs.size());

    for (const _event_signature &func : funcs)
        res.emplace_back(func(p_context, p_event));
    return res;
}


auto
EventHandler::poll_events( Context &p_ctx ) -> SDL_AppResult
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        auto it { m_handlers.find(event.type) };
        if (it != m_handlers.end())
            for (const auto &func : it->second) {
                auto res { func(p_ctx, &event)};
                if (res != SDL_APP_CONTINUE) return res;
            }
    }
    return SDL_APP_CONTINUE;
}