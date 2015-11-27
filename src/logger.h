//
// Created by Kyle on 11/25/2015.
//

#ifndef GFX_LOGGER_H
#define GFX_LOGGER_H

#include <ctime>
#include <mutex>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "macros.h"

using std::setw;
using std::time;
using std::mutex;
using std::vector;
using std::gmtime;
using std::string;
using std::ostream;
using std::setfill;
using std::remove_if;
using std::shared_ptr;
using std::lock_guard;
using std::stringstream;

namespace gfx {
    enum log_level {
        trace = 0,
        info = 1,
        debug = 2,
        warning = 3,
        error = 4,
        fatal = 5
    };

    class logger;

    typedef shared_ptr<logger> plogger;
    typedef shared_ptr<ostream> postream;

    class logger {

    public:

        logger();
        logger(const string & tag);
        ~logger();

        log_level level();
        void level(log_level level);

        void tag(const string & tag);
        string tag();

        void add(postream stream);
        void remove(postream stream);

        void log(const string & level, const string & file, int line, const string & func, const string & message);

    private:

        string m_tag;
        mutex m_mutex;
        log_level m_loglevel;
        vector<postream> m_streams;
        static mutex gmtime_mutex;

    };
/*
#define LOGT(logger, msg) \
    { \
        if(logger->level() >= log_level::trace) {
            logger->log("[Trace]")
        }
    }*/

}
#endif //GFX_LOGGER_H
