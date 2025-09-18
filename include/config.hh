#pragma once
#include <filesystem>
#include <json/value.h>

namespace fs = std::filesystem;

class Logger;

template<typename T>
concept StringLike = std::same_as<std::decay_t<T>, std::string>
                  || std::same_as<std::decay_t<T>, std::string_view>
                  || std::same_as<std::decay_t<T>, const char*>;


class Config
{
public:
    /**
     * @brief Constructs a Config instance.
     *
     * @param p_path Specify the configuration path, if empty
     *               the constructor will search in
     *               $XDG_CONFIG_HOME and /etc/APP_NAME/.
     */
    Config( fs::path p_path = "" );


    /**
     * @brief Fetch path @p p_keys from the config.
     *
     * @returns A const Json::Value.
     * @note The function can throw a fatal error if a key is not found.
     */
    [[nodiscard]]
    auto get( const std::string &p_keys ) const -> Json::Value;


    /**
     * @brief Fetch path @p p_keys from the config.
     *
     * @returns A const std::string.
     * @note The function can throw a fatal error if a key is not found.
     */
    [[nodiscard]]
    auto get_string( const std::string &p_keys ) const -> std::string;


    /**
     * @brief Fetch path @p p_keys from the config.
     *
     * @returns A const int.
     * @note The function can throw a fatal error if a key is not found.
     */
    [[nodiscard]]
    auto get_int( const std::string &p_keys ) const -> int;

private:
    fs::path    m_config_path;
    Json::Value m_config;


    void parse_config();


    [[nodiscard]]
    static auto get_default_config() -> Json::Value;
};