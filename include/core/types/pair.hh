#pragma once
#include <type_traits>
#include <utility>
#include "SDL3/SDL_rect.h"


template<typename T0, typename T1>
struct Pair
{
    T0 _0;
    T1 _1;


    constexpr Pair() : _0(), _1()
    {}


    constexpr Pair( const Pair &p_other ) :
        _0(p_other._0), _1(p_other._1)
    {}


    constexpr Pair( Pair &&p_other ) noexcept(m_noexcept) :
        _0(std::move(p_other._0)), _1(std::move(p_other._1))
    {}


    constexpr Pair( const T0 &p_0, const T1 &p_1 ) :
        _0(p_0), _1(p_1)
    {}

    constexpr Pair( T1 &&p_0, T0 &&p_1 ) :
        _0(std::move(p_0)), _1(std::move(p_1))
    {}

    template<typename U0, typename U1>
    constexpr Pair( const Pair<U0, U1> &p_other ) :
        _0(p_other._0), _1(p_other._1)
    {}

    template<typename U0, typename U1>
    constexpr Pair( Pair<U0, U1> &&p_other ) :
        _0(std::move(p_other._0)), _1(std::move(p_other._1))
    {}


    [[nodiscard]]
    constexpr auto
    operator=( const Pair &p_other ) -> Pair & = default;


    [[nodiscard]]
    constexpr auto
    operator=( Pair &&p_other ) noexcept(m_noexcept) -> Pair &
    {
        _0 = std::move(p_other._0);
        _1 = std::move(p_other._1);
        return *this;
    }


    [[nodiscard]]
    friend constexpr auto
    operator==( const Pair &p_lhs, const Pair &p_rhs) -> bool
    { return p_lhs._0 == p_rhs._0 && p_lhs._1 == p_rhs._1; }


    [[nodiscard]]
    friend constexpr auto
    operator!=( const Pair &p_lhs, const Pair &p_rhs ) -> bool
    { return !(p_lhs == p_rhs); }


    [[nodiscard]]
    friend constexpr auto
    operator<( const Pair &p_lhs, const Pair &p_rhs ) -> bool
    { return p_lhs._0 < p_rhs._0 && p_lhs._1 < p_rhs._1; }


    [[nodiscard]]
    friend constexpr auto
    operator<=( const Pair &p_lhs, const Pair &p_rhs ) -> bool
    { return !(p_rhs < p_lhs); }


    [[nodiscard]]
    friend constexpr auto
    operator>( const Pair &p_lhs, const Pair &p_rhs ) -> bool
    { return p_rhs < p_lhs; }


    [[nodiscard]]
    friend constexpr auto
    operator>=( const Pair &p_lhs, const Pair &p_rhs ) -> bool
    { return !(p_lhs < p_rhs); }


    [[nodiscard]]
    static constexpr auto
    create( T0 &&p_0, T1 &&p_1 ) -> Pair<std::decay_t<T0>, std::decay_t<T1>>
    {
        return Pair<std::decay_t<T0>, std::decay_t<T1>>(std::forward<T0>(p_0),
                                                        std::forward<T1>(p_1));
    }


    [[nodiscard]]
    static constexpr auto
    create( T0 &p_0, T1 &p_1 ) -> Pair<std::decay_t<T0>, std::decay_t<T1>>
    { return Pair<std::decay_t<T0>, std::decay_t<T1>>(p_0, p_1); }


    template<typename U0, typename U1>
    [[nodiscard]]
    static constexpr auto
    create( U0 &p_0, U1 &p_1 ) -> Pair<std::decay_t<T0>, std::decay_t<T1>>
        requires(std::is_convertible_v<U0, T0>
              && std::is_convertible_v<U1, T1>)
    {
        return Pair<std::decay_t<T0>, std::decay_t<T1>>(
            static_cast<T0>(p_0), static_cast<T1>(p_1));
    }


    constexpr void
    swap( Pair &p_other ) noexcept(
        noexcept(std::swap(std::declval<T0 &>(), std::declval<T0 &>())) &&
        noexcept(std::swap(std::declval<T1 &>(), std::declval<T1 &>())))
    {
        std::swap(_0, p_other._0);
        std::swap(_1, p_other._1);
    }


    template<typename U0 = T0, typename U1 = T1>
    constexpr auto
    to_SDL_Point() -> SDL_Point requires(std::is_convertible_v<U0, int>
                                      && std::is_convertible_v<U1, int>)
    { return { static_cast<int>(_0), static_cast<int>(_1) }; }


    template<typename U0 = T0, typename U1 = T1>
    constexpr auto
    to_SDL_FPoint() -> SDL_FPoint requires(std::is_convertible_v<U0, float>
                                        && std::is_convertible_v<U1, float>)
    { return { static_cast<float>(_0), static_cast<float>(_1) }; }



private:
    const bool m_noexcept { std::is_nothrow_move_assignable_v<T0>
                         && std::is_nothrow_move_assignable_v<T1> };
};


template<typename T_All>
using TPair = Pair<T_All, T_All>;
