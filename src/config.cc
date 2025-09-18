#include "config.hh"

#include <algorithm>
#include <utility>

#include "DEFAULT_CONFIG.hh"
#include "logger.hh"
#include "utils.hh"


Config::Config( fs::path p_path )
{
    if (!p_path.empty())
        m_config_path = std::move(p_path);
    else {
        std::vector<fs::path> candidates { 2 };

        const std::string config_home {
            utils::getenv("XDG_CONFIG_HOME",
                           std::format("{}/.config", utils::getenv("HOME")))
        };

        candidates[0] = fs::path{config_home} / APP_NAME;
        candidates[1] = std::format("/etc/{}/", APP_NAME);

        for (const auto &base : candidates) {
            for (const std::string &ext : {".json", ".jsonc"}) {
                fs::path candidate { base / ("config" + ext) };
                if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
                    m_config_path = candidate;
                    break;
                }
            }
            if (!m_config_path.empty()) break;
        }
    }

    if (m_config_path.empty()) {
        logger->WARN("No config file found, using default config");
        m_config = get_default_config();
    } else
        parse_config();
}


void
Config::parse_config()
{
    logger->INFO("Parsing configuration file {}", m_config_path.string());

    std::ifstream file { m_config_path };
    if (!file.is_open()) {
        logger->ERROR("Failed to open config file, using default config: {}",
                       std::strerror(errno));
        m_config = get_default_config();
        return;
    }

    Json::Value obj = *Json::from_stream(file).or_else(
    [this]( const std::string &p_e ) -> std::expected<Json::Value, std::string>
    {
        logger->ERROR("Failed to parse config, using default config: {}.", p_e);

        m_config = get_default_config();
        return Json::nullValue;
    });

    if (obj.isNull()) return;

    Json::Value normalized { Json::objectValue };
    for (const std::string &flat_key : obj.getMemberNames()) {
        if (!flat_key.contains('.')) {
            normalized[flat_key] = obj[flat_key];
            continue;
        }

        std::istringstream iss { flat_key };
        Json::Value *cursor { &normalized };

        std::vector<std::string> parts;
        parts.reserve(std::ranges::count(flat_key, '.'));
        for (std::string part; std::getline(iss, part, '.');)
            parts.push_back(part);

        for (const std::string &k : parts) {
            if (!(*cursor)[k].isObject())
                (*cursor)[k] = Json::Value { Json::objectValue };

            cursor = &(*cursor)[k];
        }

        (*cursor)[parts.back()] = obj[flat_key];
    }

    m_config = normalized;
}


auto
Config::get( const std::string &p_keys ) const -> Json::Value
{
    const Json::Value *current { &m_config };

    std::istringstream iss { p_keys };
    for (std::string segment; std::getline(iss, segment, '.');) {
        if (!current->isMember(segment))
            logger->FATAL("Key {} not found!", p_keys);
        current = &(*current)[segment];
    }

    return *current;
}


auto
Config::get_string( const std::string &p_keys ) const -> std::string
{ return this->get(p_keys).asString(); }


auto
Config::get_int( const std::string &p_keys ) const -> int
{ return this->get(p_keys).asInt(); }


auto
Config::get_default_config() -> Json::Value
{
    return *Json::from_string(DEFAULT_CONFIG_JSON_STRING).or_else(
    []( const std::string &p_err ) -> std::expected<Json::Value, std::string>
    {
        logger->FATAL("Failed to parse default config: {}", p_err);
        return {};
    } );
}