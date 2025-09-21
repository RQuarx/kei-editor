#include "event/event.hh"

#include <utility>

using event::Handler;


Handler::Handler( shared_logger p_logger ) :
    m_logger(std::move(p_logger))
{
}


auto
Handler::run_handlers( uint32_t   p_key,
                       SDL_Event *p_event ) -> std::vector<SDL_AppResult>
{
    auto funcs { m_handlers.at(p_key) };

    std::vector<SDL_AppResult> res;
    res.reserve(funcs.size());

    for (const _signature &func : funcs)
        res.emplace_back(func(p_event));
    return res;
}


auto
Handler::poll_events() -> SDL_AppResult
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        auto it { m_handlers.find(event.type) };
        if (it != m_handlers.end())
            for (const auto &func : it->second) {
                auto res { func(&event) };
                if (res != SDL_APP_CONTINUE) return res;
            }
    }
    return SDL_APP_CONTINUE;
}