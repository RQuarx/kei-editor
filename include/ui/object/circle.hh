#pragma once
#include <SDL3/SDL_rect.h>
#include "ui/color.hh"


namespace ui
{
    struct Context;


    struct Circle
    {
        float x;
        float y;

        Color color;
        float radius;
        bool  filled;


        [[nodiscard]]
        static auto create( SDL_FPoint p_pos,
                            Color      p_color,
                            float      p_radius,
                            bool       p_filled = true ) -> Circle;


        [[nodiscard]]
        auto contains( SDL_FPoint p_point ) const -> bool;


        [[nodiscard]]
        auto intersects( const Circle &p_other ) const -> bool;


        [[nodiscard]]
        auto center() const -> SDL_FPoint;


        auto move  ( SDL_FPoint p_distance ) -> Circle &;
        auto resize( float      p_radius   ) -> Circle &;


        void draw( const Context &p_ctx ) const;
    };
}