#pragma once
#include <functional>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include "core/logger.hh"


namespace event
{
    class Handler
    {
        using _event_signature =
            std::function<SDL_AppResult ( SDL_Event * )>;
    public:
        Handler( shared_logger p_logger );


        template<typename T_Func, typename ...T_Params>
        void
        add_handler( uint32_t p_key, T_Func &&p_func, T_Params &&...p_params )
        {
            _event_signature handler =
                [func  = std::forward<T_Func>(p_func),
                 bound = std::make_tuple(std::forward<T_Params>(p_params)...)]
                ( SDL_Event *p_event ) -> SDL_AppResult
            { return invoke_with_bound(func, p_event, bound); };

            m_handlers[p_key].emplace_back(std::move(handler));
        }


        template<typename T_Instance, typename T_Func, typename ...T_Params>
        void
        add_handler( uint32_t      p_key,
                     T_Instance  &&p_instance,
                     T_Func      &&p_method,
                     T_Params &&...p_params )
        {
            _event_signature handler =
                [&p_instance, p_method,
                  bound = std::make_tuple(std::forward<T_Params>(p_params)...)]
                ( SDL_Event *p_event ) -> SDL_AppResult
            {
                return invoke_with_bound(p_method, p_instance, p_event, bound);
            };

            m_handlers[p_key].emplace_back(std::move(handler));
        }


        [[nodiscard]]
        auto run_handlers( uint32_t   p_key,
                           SDL_Event *p_event
                         ) -> std::vector<SDL_AppResult>;


        [[nodiscard]]
        auto poll_events() -> SDL_AppResult;


    private:
        shared_logger m_logger;
        std::unordered_map<uint32_t, std::vector<_event_signature>> m_handlers;


        template<typename T_Instance,
                 typename T_Method,
                 typename T_Tuple,
                 size_t ...T_I>
        static auto
        invoke_with_bound_impl( T_Method    T_Instance::*p_method,
                                T_Instance &p_instance,
                                SDL_Event  *p_event,
                                T_Tuple    &p_bound,
                                std::index_sequence<T_I...> ) -> SDL_AppResult
        { return (p_instance.*p_method)(p_event, std::get<T_I>(p_bound)...); }


        template<typename T_Func, typename T_Tuple, size_t ...T_I>
        static auto
        invoke_with_bound_impl( T_Func    &p_func,
                                SDL_Event *p_event,
                                T_Tuple   &p_bound,
                                std::index_sequence<T_I...> ) -> SDL_AppResult
        { return p_func(p_event, std::get<T_I>(p_bound)...); }


        template<typename T_Instance, typename T_Method, typename T_Tuple>
        static auto
        invoke_with_bound( T_Method    T_Instance::*p_method,
                           T_Instance &p_instance,
                           SDL_Event  *p_event,
                           T_Tuple    &p_bound ) -> SDL_AppResult
        {
            constexpr auto size {
                std::tuple_size_v<std::remove_reference_t<T_Tuple>> };
            return invoke_with_bound_impl(p_method,
                                          p_instance,
                                          p_event,
                                          p_bound,
                                          std::make_index_sequence<size>{});
        }


        template<typename T_Func, typename T_Tuple>
        static auto
        invoke_with_bound( T_Func    &p_func,
                           SDL_Event *p_event,
                           T_Tuple   &p_bound ) -> SDL_AppResult
        {
            constexpr auto size {
                std::tuple_size_v<std::remove_reference_t<T_Tuple>> };
            return invoke_with_bound_impl(p_func, p_event, p_bound,
                                          std::make_index_sequence<size>{});
        }
    };
}


#define EVENT_SIGNATURE( ... )                \
    []( SDL_Event *p_event ) -> SDL_AppResult \
    {                                         \
        __VA_ARGS__                           \
    }