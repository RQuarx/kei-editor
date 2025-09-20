#pragma once
#include <SDL3/SDL_rect.h>
#include "ui/color.hh"


namespace ui
{
    using f_pair = SDL_FPoint;

    struct Context;


    struct Box
    {
        float x;
        float y;
        float w;
        float h;

        Color color;
        bool rounded_corner;

        union
        {
            bool  filled;
            float radius;
        };


        /** Creates a new rounded box. */
        [[nodiscard]]
        static auto rounded( f_pair p_pos,
                             f_pair p_size,
                             Color  p_col,
                             float  p_radius ) -> Box;


        /** Creates a new squared box. */
        [[nodiscard]]
        static auto square( f_pair p_pos,
                            f_pair p_size,
                            Color  p_col,
                            bool   p_filled ) -> Box;


        [[nodiscard]]
        auto contains( f_pair p_pos ) const -> bool;


        [[nodiscard]]
        auto intersects( const Box &p_other ) const -> bool;


        [[nodiscard]]
        auto center() const -> f_pair;


        auto move  ( f_pair p_distance ) -> Box &;
        auto resize( f_pair p_new      ) -> Box &;


        [[nodiscard]]
        auto to_rect() const -> SDL_FRect;


        void draw( const Context &p_ctx ) const;
    };
}