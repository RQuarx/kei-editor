#include "interface/color.hh"

#define CLAMP01(x) ((x) < 0.0f ? 0.0f : ((x) > 1.0f ? 1.0f : (x)))

using ui::Color;


Color::Color( uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a ) :
    r(p_r), g(p_g), b(p_b), a(p_a)
{}


Color::Color( uint8_t p_r, uint8_t p_g, uint8_t p_b ) :
    r(p_r), g(p_g), b(p_b), a(255)
{}


Color::Color( uint8_t p_gray ) :
    r(p_gray), g(p_gray), b(p_gray), a(p_gray)
{}


Color::Color( SDL_FColor p_color ) :
    r(static_cast<uint8_t>((CLAMP01(p_color.r) * 255.0F))),
    g(static_cast<uint8_t>((CLAMP01(p_color.g) * 255.0F))),
    b(static_cast<uint8_t>((CLAMP01(p_color.b) * 255.0F))),
    a(static_cast<uint8_t>((CLAMP01(p_color.a) * 255.0F)))
{}


Color::Color( SDL_Color p_color ) :
    r(p_color.r), g(p_color.g), b(p_color.b), a(p_color.a)
{}


Color::Color( const std::string &p_hex )
{
    if (p_hex.empty() || p_hex[0] != '#')
        logger->FATAL("Hex must start with '#': {}", p_hex);

    const size_t len { p_hex.length() };
    if (len != 7 && len != 9)
        logger->FATAL("Hex values must be in '#RRGGBB' or '#RRGGBBAA' format");

    r = hex_to_byte(p_hex.substr(1, 2));
    g = hex_to_byte(p_hex.substr(3, 2));
    b = hex_to_byte(p_hex.substr(5, 2));
    a = len == 9 ? hex_to_byte(p_hex.substr(7, 2)) : 255;
}


auto
Color::to_fcolor() const -> SDL_FColor
{
    return { static_cast<float>(r) / 255.0F,
             static_cast<float>(g) / 255.0F,
             static_cast<float>(b) / 255.0F,
             static_cast<float>(a) / 255.0F };
}


auto
Color::to_color() const -> SDL_Color
{ return { r, g, b, a }; }

