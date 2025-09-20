#pragma once
#include <filesystem>
#include <json/value.h>
#include "logger.hh"

namespace fs = std::filesystem;


class Config;
using shared_config = std::shared_ptr<Config>;

class Config
{
public:
    Config( shared_logger p_logger, fs::path p_path = "" );


    /**
     * @brief Constructs a Config instance.
     *
     * @param p_path Specify the configuration path, if empty
     *               the constructor will search in
     *               $XDG_CONFIG_HOME and /etc/APP_NAME/.
     */
    static auto create( shared_logger p_logger,
                        fs::path      p_path = "" ) -> shared_config;


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
    shared_logger m_logger;

    fs::path    m_config_path;
    Json::Value m_config;


    void parse_config();


    [[nodiscard]]
    static auto get_default_config() -> Json::Value;
};