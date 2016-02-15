/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef GNUT_LOGGER_H
#define GNUT_LOGGER_H

#include <ctime>
#include <mutex>
#include <thread>
#include <string>
#include <memory>
#include <vector>
#include <cstring>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "log.h"
#include "macros.h"
#include "utility.h"
#include "compile_flags.h"

using std::setw;
using std::left;
using std::time;
using std::endl;
using std::mutex;
using std::vector;
using std::gmtime;
using std::string;
using std::ostream;
using std::setfill;
using std::remove_if;
using std::shared_ptr;
using std::lock_guard;
using std::make_shared;
using std::stringstream;

namespace gnut {
    namespace log {

        enum level {
            trace = 5,
            info = 4,
            debug = 3,
            warning = 2,
            error = 1,
            fatal = 0
        };

        class logger {
        public:

            logger();
            ~logger();

            level log_level();
            void log_level(level level);

            void add(plog stream);
            void remove(plog stream);

            void log(const string &message);

        private:

            mutex m_mutex;
            level m_loglevel;
            vector<plog> m_streams;

        };

        typedef shared_ptr<logger> plogger;

#if GNUT_ENABLE_LOGGING
#define LOG(level, tag, msg) \
    { \
        if(logger->log_level() >= level) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << tag << msg << endl; \
            logger->log(sstream.str()); \
        } \
    } \

#define LOG_TRACE(msg) LOG(log::level::trace, " [TRACE] ", msg)
#define LOG_INFO(msg) LOG(log::level::info, " [INFO] ", msg);
#define LOG_DEBUG(msg) LOG(log::level::debug, " [DEBUG] ", msg)
#define LOG_WARNING(msg) LOG(log::level::warning, " [WARNING] ", msg)
#define LOG_ERROR(msg) LOG(log::level::error, " [ERROR] ", msg)
#define LOG_FATAL(msg) LOG(log::level::fatal, " [FATAL] ", msg)

#else

#define LOG_TRACE(msg)
#define LOG_INFO(msg)
#define LOG_DEBUG(msg)
#define LOG_WARNING(msg)
#define LOG_ERROR(msg)
#define LOG_FATAL(msg)
#endif

    }
}

// global logger
extern gnut::log::plogger logger;

#endif //GNUT_LOGGER_H
