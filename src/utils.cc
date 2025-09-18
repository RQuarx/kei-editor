#include "utils.hh"

#include <json/json.h>


namespace utils
{
    auto
    getenv( const std::string &p_name ) -> std::string
    {
        const char *value { std::getenv(p_name.c_str()) };
        return value == nullptr ? "" : value;
    }


    auto
    getenv( const std::string &p_name,
            const std::string &p_default ) -> std::string
    {
        const char *value { std::getenv(p_name.c_str()) };
        return value == nullptr ? p_default : value;
    }


    auto
    is_point_inside_rect( const SDL_FRect  &p_rect,
                          const SDL_FPoint &p_point ) -> bool
    {
        return p_point.x >= p_rect.x && p_point.y >= p_rect.y
            && p_point.x <= p_rect.x + p_rect.w
            && p_point.y <= p_rect.y + p_rect.h;
    }


    namespace str
    {
        auto
        is_equals_case( const std::string &p_a,
                        const std::string &p_b,
                        const std::locale &p_locale ) -> bool
        {
            if (p_a.length() != p_b.length()) return false;

            for (size_t i { 0 }; i < p_a.length(); i++)
                if (std::tolower(p_a[i], p_locale)
                 != std::tolower(p_b[i], p_locale))
                    return false;
            return true;
        }
    }
}


namespace Json
{
    auto
    from_stream( std::istream &p_stream ) -> std::expected<Json::Value,
                                                           std::string>
    {
        Value root;

        CharReaderBuilder builder;
        builder["collectComments"] = true;
        std::string err;
        if (!parseFromStream(builder, p_stream, &root, &err))
            return std::unexpected(err);
        return root;
    }


    auto
    from_string( const std::string &p_str ) -> std::expected<Json::Value,
                                                             std::string>
    {
        Value root;

        CharReaderBuilder builder;
        const std::unique_ptr<CharReader> reader { builder.newCharReader() };
        std::string err;
        if (!reader->parse(p_str.c_str(), p_str.c_str() + p_str.length(),
                          &root, &err))
            return std::unexpected(err);

        return root;
    }
}