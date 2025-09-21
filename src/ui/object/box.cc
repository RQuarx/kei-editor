#include "ui/object/box.hh"

#include "ui/context/context.hh"
#include "ui/context/exception.hh"
#include "ui/object/circle.hh"

using ui::Box;


auto
Box::rounded( const TPair<float> &p_pos,
              const TPair<float> &p_size,
              Color               p_col,
              float               p_radius ) -> Box
{
    return {
        .x=p_pos._0,
        .y=p_pos._1,
        .w=p_size._0,
        .h=p_size._1,
        .color=p_col,
        .rounded_corner=true,
        .radius=p_radius
    };
}


auto
Box::square( const TPair<float> &p_pos,
             const TPair<float> &p_size,
             Color               p_col,
             bool                p_filled ) -> Box
{
    return {
        .x=p_pos._0,
        .y=p_pos._1,
        .w=p_size._0,
        .h=p_size._1,
        .color=p_col,
        .rounded_corner=false,
        .filled=p_filled
    };
}


auto
Box::contains( const TPair<float> &p_pos ) const -> bool
{
    return p_pos._0 >= x && p_pos._0 <= x + w
        && p_pos._1 >= y && p_pos._1 <= y + h;
}


auto
Box::intersects( const Box &p_other ) const -> bool
{
    return p_other.x <= x + w
        && p_other.x + p_other.w >= x
        && p_other.y <= y + h
        && p_other.y + p_other.h >= y;
}


auto
Box::center() const -> TPair<float>
{ return { x + (w * 0.5F), y + (h * 0.5F) }; }


auto
Box::move( const TPair<float> &p_distance ) -> Box &
{
    x += p_distance._0;
    y += p_distance._1;
    return *this;
}


auto
Box::resize( const TPair<float> &p_new ) -> Box &
{
    w = p_new._0;
    h = p_new._1;
    return *this;
}


auto
Box::to_rect() const -> SDL_FRect
{ return { x, y, w, h }; }


void
Box::draw( const Context &p_ctx ) const
{
    p_ctx.set_draw_color(color);

    if (!rounded_corner) {
        SDL_FRect rect { x, y, w, h };
        bool res { filled ? SDL_RenderFillRect(p_ctx.render, &rect)
                          : SDL_RenderRect(p_ctx.render, &rect) };
        if (!res)
            throw CtxException("Failed to render rect: {}", SDL_GetError());
    } else {
        const float x1 { x + radius               };
        const float x2 { x + w - radius - 1 };
        const float y1 { y + radius               };
        const float y2 { y + h - radius - 1 };

        const std::array<SDL_FPoint, 4> corners {{
            { .x=x1, .y=y1 },
            { .x=x2, .y=y1 },
            { .x=x1, .y=y2 },
            { .x=x2, .y=y2 },
        }};

        std::array<SDL_FRect, 3> rects;
        rects[0] = {
            .x=x + radius,
            .y=y,
            .w=w - (radius * 2),
            .h=h
        };
        rects[1] = {
            .x=x,
            .y=y + radius,
            .w=radius,
            .h=h - (radius * 2)
        };
        rects[2] = {
            .x=x + w - radius,
            .y=y + radius,
            .w=radius,
            .h=h - (radius * 2)
        };

        if (!SDL_RenderFillRects(p_ctx.render, rects.data(), rects.size()))
            throw CtxException("Failed to render rect: {}", SDL_GetError());

        for (const auto &corner : corners)
            Circle::create(corner, color, radius).draw(p_ctx);
    }
}