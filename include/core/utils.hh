#pragma once
#include <expected>
#include <locale>
#include <string>
#include <SDL3/SDL_rect.h>
#include <json/value.h>


namespace utils
{
    [[nodiscard]]
    auto getenv( const std::string &p_name ) -> std::string;


    [[nodiscard]]
    auto getenv( const std::string &p_name,
                 const std::string &p_default ) -> std::string;


    [[nodiscard]]
    auto is_point_inside_rect( const SDL_FRect  &p_rect,
                               const SDL_FPoint &p_point ) -> bool;


    namespace str
    {
        [[nodiscard]]
        auto is_equals_case( const std::string &p_a,
                             const std::string &p_b,
                             const std::locale &p_locale = std::locale()
                           ) -> bool;
    }
}


namespace Json
{
    [[nodiscard]]
    auto from_stream( std::istream &p_stream ) -> std::expected<Json::Value,
                                                                std::string>;


    [[nodiscard]]
    auto from_string( const std::string &p_str ) -> std::expected<Json::Value,
                                                                  std::string>;
}