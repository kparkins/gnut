//
// Created by Kyle on 11/25/2015.
//

#include "logger.h"

// global logger
gnut::plogger logger = make_shared<gnut::logger>();

gnut::logger::logger() {
    m_loglevel = log_level::warning;
}

gnut::logger::~logger() {
    m_streams.clear();
}

gnut::log_level gnut::logger::level() {
    lock_guard<mutex> lock(m_mutex);
    return this->m_loglevel;
}

void gnut::logger::level(gnut::log_level level) {
    lock_guard<mutex> lock(m_mutex);
    this->m_loglevel = level;
}

void gnut::logger::add(gnut::plog_stream stream) {
    lock_guard<mutex> lock(m_mutex);
    assert(stream);
    m_streams.push_back(stream);
}

void gnut::logger::remove(gnut::plog_stream stream) {
    lock_guard<mutex> lock(m_mutex);
    remove_if(m_streams.begin(), m_streams.end(), [&] (const plog_stream & p) -> bool { return stream == p; });
}

void gnut::logger::log(const string & message) {
    lock_guard<mutex> lock(m_mutex);
    for(auto stream : m_streams) {
        stream->write(message);
    }
}
