#include "Logger.hpp"
#include "DisplayLog.hpp"
#include "RollingLogFollow.hpp"

#include "../../event/EventBus.hpp"

#include "../../config/ConfigValue.hpp"

using namespace Log;

// Keep the exported CLogger layout unchanged for existing native plugins.
static Tamlinux::CDisplayLog& displayLog() {
    static Tamlinux::CDisplayLog sink;
    return sink;
}

CLogger::CLogger() : m_isTrace(Env::isTrace()) {
    m_logger.setLogLevel(m_isTrace ? Hyprutils::CLI::LOG_TRACE : Hyprutils::CLI::LOG_DEBUG);
}

void CLogger::log(Hyprutils::CLI::eLogLevel level, const std::string_view& str) {
    const int priority = level >= Hyprutils::CLI::LOG_CRIT ? 2 : level >= Hyprutils::CLI::LOG_ERR ? 3 : level >= Hyprutils::CLI::LOG_WARN ? 4 : 7;
    if (!displayLog().write("hyprland", priority, str))
        std::fputs("<3>tamlinux display logging write failed\n", stderr);
    if (!m_logsEnabled)
        return;

    if (level == Hyprutils::CLI::LOG_TRACE && !m_isTrace)
        return;

    if (SRollingLogFollow::get().isRunning())
        SRollingLogFollow::get().addLog(str);

    m_logger.log(level, str);
}

void CLogger::initIS(const std::string_view& IS) {
    // NOLINTNEXTLINE
    m_logger.setOutputFile(std::string{IS} + (ISDEBUG ? "/hyprlandd.log" : "/hyprland.log"));
    if (!displayLog().open(std::string{IS} + "/display.log"))
        std::fputs("<3>tamlinux display log file open failed\n", stderr);
    m_logger.setEnableRolling(true);
    m_logger.setEnableColor(false);
    m_logger.setEnableStdout(true);
    m_logger.setTime(false);
}

void CLogger::initCallbacks() {
    static auto P = Event::bus()->m_events.config.reloaded.listen([this]() { recheckCfg(); });
    recheckCfg();
}

void CLogger::recheckCfg() {
    static auto PDISABLELOGS  = CConfigValue<Config::INTEGER>("debug:disable_logs");
    static auto PDISABLETIME  = CConfigValue<Config::INTEGER>("debug:disable_time");
    static auto PENABLESTDOUT = CConfigValue<Config::INTEGER>("debug:enable_stdout_logs");
    static auto PENABLECOLOR  = CConfigValue<Config::INTEGER>("debug:colored_stdout_logs");

    m_logger.setEnableStdout(!*PDISABLELOGS && *PENABLESTDOUT);
    m_logsEnabled = !*PDISABLELOGS;
    m_logger.setTime(!*PDISABLETIME);
    m_logger.setEnableColor(*PENABLECOLOR);
}

const std::string& CLogger::rolling() {
    return m_logger.rollingLog();
}

Hyprutils::CLI::CLogger& CLogger::hu() {
    return m_logger;
}

void CLogger::backendLog(Hyprutils::CLI::eLogLevel level, const std::string_view& str) {
    const int  priority = level >= Hyprutils::CLI::LOG_CRIT ? 2 : level >= Hyprutils::CLI::LOG_ERR ? 3 : level >= Hyprutils::CLI::LOG_WARN ? 4 : 6;
    const bool transition =
        str.starts_with("drm: Connector ") || str.starts_with("drm: Connecting connector ") || str.starts_with("drm: Disabling output ") || str.starts_with("drm: Modesetting ");
    if (!displayLog().write("aquamarine", priority, str, transition))
        std::fputs("<3>tamlinux backend logging write failed\n", stderr);
}

void CLogger::displayEvent(const std::string_view& str) {
    if (!displayLog().write("hyprland", 6, str, true))
        std::fputs("<3>tamlinux display event write failed\n", stderr);
}
