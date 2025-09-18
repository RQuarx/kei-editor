#pragma once
#include <sstream>
#include <cstdint>
#include <string>
#include <SDL3/SDL_pixels.h>
#include "logger.hh"


namespace ui
{
    struct alignas(alignof(uint32_t)) Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;


        Color( uint8_t p_r, uint8_t p_g, uint8_t p_b, uint8_t p_a );
        Color( uint8_t p_r, uint8_t p_g, uint8_t p_b );
        Color( uint8_t p_gray );
        Color( SDL_FColor p_color );
        Color( SDL_Color p_color );
        Color( const std::string &p_hex );


        [[nodiscard]]
        auto to_fcolor() const -> SDL_FColor;


        [[nodiscard]]
        auto to_color() const -> SDL_Color;
    };
}


constexpr auto
operator""_rgb( unsigned long long p_val ) -> ui::Color
{
    return ui::Color {
        static_cast<uint8_t>((p_val >> 16) & 0xFF),
        static_cast<uint8_t>((p_val >> 8) & 0xFF),
        static_cast<uint8_t>(p_val & 0xFF)
    };
}


constexpr auto
operator""_rgba( unsigned long long p_val ) -> ui::Color
{
    return ui::Color {
        static_cast<uint8_t>((p_val >> 24) & 0xFF),
        static_cast<uint8_t>((p_val >> 16) & 0xFF),
        static_cast<uint8_t>((p_val >> 8) & 0xFF),
        static_cast<uint8_t>(p_val & 0xFF)
    };
}


constexpr auto
hex_to_byte( const std::string &p_str ) -> uint8_t
{
    unsigned int x;
    std::istringstream iss { p_str };
    iss >> std::hex >> x;
    if (iss.fail()) logger->FATAL("Invalid hex value in color: {}", p_str);
    return static_cast<uint8_t>(x);
}


constexpr auto
operator""_rgb( const char *p_hex ) -> ui::Color
{
    const std::string hex { p_hex };
    if (hex.empty() || hex[0] != '#')
        logger->FATAL("Color must start with '#': {}", hex);

    const size_t len { hex.length() };
    if (len != 7)
        logger->FATAL("Color values must be in '#RRGGBB' format");

    return ui::Color {
        hex_to_byte(hex.substr(1, 2)),
        hex_to_byte(hex.substr(3, 2)),
        hex_to_byte(hex.substr(5, 2))
    };
}


constexpr auto
operator""_rgba( const char *p_hex ) -> ui::Color
{
    const std::string hex { p_hex };
    if (hex.empty() || hex[0] != '#')
        logger->FATAL("Color must start with '#': {}", hex);

    const size_t len { hex.length() };
    if (len != 9)
        logger->FATAL("Color values must be in '#RRGGBBAA' format");

    return ui::Color {
        hex_to_byte(hex.substr(1, 2)),
        hex_to_byte(hex.substr(3, 2)),
        hex_to_byte(hex.substr(5, 2)),
        hex_to_byte(hex.substr(7, 2))
    };
}


//NOLINTBEGIN
#define TO_PARAMS( p_color ) \
    p_color.r, p_color.g, p_color.b, p_color.a
//NOLINTEND
