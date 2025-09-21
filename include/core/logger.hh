#pragma once
#include <source_location>
#include <fstream>
#include <format>
#include <memory>
#include <string>

namespace std { extern ostream cerr; }


class Logger
{
    using label_pair = std::pair<std::string_view, std::string_view>;
public:
    Logger( const std::string &p_log_level,
            const std::string &p_log_file = "" );


    [[nodiscard]]
    static auto create( const std::string &p_level,
                        const std::string &p_file = ""
                      ) -> std::shared_ptr<Logger>;

    template<unsigned T_LogLevel, typename... T_Args>
    void
    log( const std::source_location &p_loc,
         std::string_view            p_fmt,
         T_Args                 &&...p_args )
    {
        std::string msg { std::vformat(p_fmt,
                          std::make_format_args(p_args...)) };

        std::string file { p_loc.file_name() };
        unsigned    line { p_loc.line() };

        std::string label {
            std::format("{} {} at \033[1m{}:{}\033[0m",
                         get_time(),
                         m_LABELS.at(T_LogLevel).first,
                         file, line) };

        if (m_log_file.is_open()) {
            std::string file_label {
                std::format("{} at {}:{}",
                             get_time(),
                             m_LABELS.at(T_LogLevel).second,
                             file, line) };

            m_log_file << std::format("[{}]: {}", file_label, msg) << '\n';
            m_log_file.flush();
        }

        if (T_LogLevel < m_threshold_level) return;
        std::cerr << std::format("[{}]: \033[1m{}\033[0m\n", label, msg);

        /* FATAL level */
        if (T_LogLevel == 4) std::exit(1);
    }

#define DEBUG( ... ) log<0>(std::source_location::current(), __VA_ARGS__)
#define INFO( ... )  log<1>(std::source_location::current(), __VA_ARGS__)
#define WARN( ... )  log<2>(std::source_location::current(), __VA_ARGS__)
#define ERROR( ... ) log<3>(std::source_location::current(), __VA_ARGS__)
#define FATAL( ... ) log<4>(std::source_location::current(), __VA_ARGS__)


private:
    static constexpr std::array<label_pair, 5> m_LABELS {{
        { "\033[1;36mdebug\033[0;0;0m", "debug" },
        { "\033[1;32minfo\033[0;0;0m ", "info " },
        { "\033[1;33mwarn\033[0;0;0m ", "warn " },
        { "\033[1;31merror\033[0;0;0m", "error" },
        { "\033[1;31mFATAL\033[0;0;0m", "FATAL" },
    }};

    std::ofstream m_log_file;
    unsigned int  m_threshold_level;


    [[nodiscard]]
    static auto get_time() -> std::string;
};


using shared_logger = std::shared_ptr<Logger>;