#include "ui/object/circle.hh"

#include <numbers>
#include <ranges>
#include <vector>
#include <cmath>
#include <SDL3/SDL_render.h>
#include "ui/context/exception.hh"
#include "ui/context/context.hh"

using ui::CtxException;
using ui::Circle;


namespace
{
    void
    draw_circle_filled( SDL_Renderer *p_render, SDL_FPoint p_pos, float p_rad )
    {
        float x {   p_rad   };
        float y {     0     };
        float d { 1 - p_rad };
        while (x >= y) {
            SDL_RenderLine(p_render, p_pos.x - x, p_pos.y + y,
                           p_pos.x + x, p_pos.y + y);
            SDL_RenderLine(p_render, p_pos.x - y, p_pos.y + x,
                           p_pos.x + y, p_pos.y + x);
            SDL_RenderLine(p_render, p_pos.x - x, p_pos.y - y,
                           p_pos.x + x, p_pos.y - y);
            SDL_RenderLine(p_render, p_pos.x - y, p_pos.y - x,
                           p_pos.x + y, p_pos.y - x);
            y++;
            if (d < 0)
                d += 2 * y + 1;
            else {
                x--;
                d += 2 * (y - x) + 1;
            }
        }
    }


    void
    draw_circle_outline( SDL_Renderer *p_render, SDL_FPoint p_pos, float p_rad )
    {
        std::vector<SDL_FPoint> points;

        float x {   p_rad   };
        float y {     0     };
        float d { 1 - p_rad };
        while (x >= y) {
            points.emplace_back(p_pos.x + x, p_pos.y + y);
            points.emplace_back(p_pos.x + y, p_pos.y + x);
            points.emplace_back(p_pos.x - y, p_pos.y + x);
            points.emplace_back(p_pos.x - x, p_pos.y + y);
            points.emplace_back(p_pos.x - x, p_pos.y - y);
            points.emplace_back(p_pos.x - y, p_pos.y - x);
            points.emplace_back(p_pos.x + y, p_pos.y - x);
            points.emplace_back(p_pos.x + x, p_pos.y - y);

            y++;
            if (d < 0)
                d += 2 * y + 1;
            else {
                x--;
                d += 2 * (y - x) + 1;
            }
        }

        if (!SDL_RenderPoints(p_render, points.data(),
                              static_cast<int>(points.size())))
            throw CtxException("Failed to render points: {}", SDL_GetError());
    }
}


auto
Circle::create( SDL_FPoint p_pos,
                Color      p_color,
                float      p_radius,
                bool       p_filled ) -> Circle
{
    return {
        .x=p_pos.x,
        .y=p_pos.y,
        .color=p_color,
        .radius=p_radius,
        .filled=p_filled
    };
}


auto
Circle::contains( SDL_FPoint p_point ) const -> bool
{
    const float dx { p_point.x - x };
    const float dy { p_point.y - y };
    return (dx * dx + dy * dy) <= (radius * radius);
}


auto
Circle::intersects( const Circle &p_other ) const -> bool
{
    const float dx    { p_other.x - x };
    const float dy    { p_other.y - y };
    const float dist2 { (dx * dx) + (dy * dy) };
    const float rsum  { radius + p_other.radius };
    return dist2 <= (rsum * rsum);
}


auto
Circle::center() const -> SDL_FPoint
{ return { x, y }; }

auto
Circle::move( SDL_FPoint p_distance ) -> Circle &
{
    x += p_distance.x;
    y += p_distance.y;
    return *this;
}

auto
Circle::resize( float p_radius ) -> Circle &
{
    radius = p_radius;
    return *this;
}

void
Circle::draw( const Context &p_ctx ) const
{
    p_ctx.set_draw_color(color);

    filled ? draw_circle_filled(p_ctx.render, { x, y }, radius)
           : draw_circle_outline(p_ctx.render, { x, y }, radius);
}