//
// Created by Kyle on 11/25/2015.
//

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

#include "macros.h"
#include "utility.h"
#include "log.h"

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
    namespace log{
            enum log_level {
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

                log_level level();
                void level(log_level level);

                void add(plog stream);
                void remove(plog stream);

                void log(const string &message);

                private:

                string m_tag;
                mutex m_mutex;
                log_level m_loglevel;
                vector<plog> m_streams;

            };

            typedef shared_ptr<logger> plogger;

#define LOGT(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::trace) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [TRACE] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

#define LOGI(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::info) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [INFO] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

#define LOGD(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::debug) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [DEBUG] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

#define LOGW(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::warning) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [WARNING] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

#define LOGE(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::error) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [ERROR] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

#define LOGF(logger, msg) \
    { \
        if(logger->level() >= gnut::log::log_level::fatal) { \
            stringstream sstream; \
            std::thread::id id = std::this_thread::get_id(); \
            sstream << "[" << setfill(' ') << setw(14) << gnut::to_hex(id) << "] " \
                    << gnut::gmt_datetime()                                        \
                    << __FILENAME__ << "(" << __LINE__ << ")" << " [FATAL] " << msg << endl; \
            logger->log(sstream.str()); \
        } \
    }\

    }
}

// global logger
extern gnut::log::plogger logger;

#endif //GNUT_LOGGER_H
