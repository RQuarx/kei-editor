#pragma once
#include <SDL3/SDL_rect.h>
#include "core/types.hh"
#include "ui/color.hh"


namespace ui
{
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
        static auto rounded( const TPair<float> &p_pos,
                             const TPair<float> &p_size,
                             Color               p_col,
                             float               p_radius ) -> Box;


        /** Creates a new squared box. */
        [[nodiscard]]
        static auto square( const TPair<float> &p_pos,
                            const TPair<float> &p_size,
                            Color               p_col,
                            bool                p_filled ) -> Box;


        [[nodiscard]]
        auto contains( const TPair<float> &p_pos ) const -> bool;


        [[nodiscard]]
        auto intersects( const Box &p_other ) const -> bool;


        [[nodiscard]]
        auto center() const -> TPair<float>;


        auto move  ( const TPair<float> &p_distance ) -> Box &;
        auto resize( const TPair<float> &p_new      ) -> Box &;


        [[nodiscard]]
        auto to_rect() const -> SDL_FRect;


        void draw( const Context &p_ctx ) const;
    };
}