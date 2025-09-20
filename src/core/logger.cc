#include <algorithm>
#include <cstring>
#include <chrono>
#include "core/logger.hh"


namespace
{
    [[nodiscard]]
    auto
    to_lower( std::string p_str ) -> std::string
    {
        std::ranges::transform(p_str, p_str.begin(),
        []( char p_c ) { return std::tolower(p_c); });
        return p_str;
    }


    [[nodiscard]]
    auto
    string_to_loglevel( const std::string &p_str ) -> unsigned
    {
        if (p_str == "debug") return 0;
        if (p_str == "info" ) return 1;
        if (p_str == "warn" ) return 2;
        if (p_str == "error") return 3;
        if (p_str == "fatal") return 4;
        return 5;
    }
}


auto
Logger::create( const std::string &p_level,
                const std::string &p_file ) -> shared_logger
{ return std::make_shared<Logger>(p_level, p_file); }


Logger::Logger( const std::string &p_log_level,
                const std::string &p_log_file ) :
    m_threshold_level(2)
{
    if (!p_log_level.empty()) {
        unsigned level;

        try {
            level = std::stoi(p_log_level);
        } catch (...) {
            level = string_to_loglevel(to_lower(p_log_level));
        }

        if (level > 4)
            WARN("Invalid log level '{}', using default 'warn'", p_log_level);
        else
            m_threshold_level = level;
    }

    if (!p_log_file.empty()) {
        m_log_file.open(p_log_file, std::ios_base::app);
        if (m_log_file.fail() && !m_log_file.eof()) {
            FATAL("Failed to open {}: {}", p_log_file, std::strerror(errno));
            throw std::exception();
        }
    }

    DEBUG("Logger instance successfully created with a log-level of {}",
           m_LABELS[m_threshold_level].second);
}


auto
Logger::get_time() -> std::string
{
    using std::chrono::duration;
    using ms = std::chrono::milliseconds;
    using m  = std::chrono::minutes;
    using s  = std::chrono::seconds;

    duration now { std::chrono::system_clock::now().time_since_epoch() };
    ms millis    { std::chrono::duration_cast<ms>(now) % 1000 };
    m  minutes   { std::chrono::duration_cast<m >(now) % 60 };
    s  seconds   { std::chrono::duration_cast<s >(now) % 60 };

    return std::format("{:02}:{:02}.{:03}",
                        minutes.count(), seconds.count(), millis.count());
}