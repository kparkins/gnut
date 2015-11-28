//
// Created by Kyle on 11/25/2015.
//

#include "logger.h"

// global logger
gnut::log::plogger logger = make_shared<gnut::log::logger>();

gnut::log::logger::logger() {
    m_loglevel = level::warning;
}

gnut::log::logger::~logger() {
    m_streams.clear();
}

gnut::log::level gnut::log::logger::log_level() {
    lock_guard<mutex> lock(m_mutex);
    return this->m_loglevel;
}

void gnut::log::logger::log_level(level level) {
    lock_guard<mutex> lock(m_mutex);
    this->m_loglevel = level;
}

void gnut::log::logger::add(plog stream) {
    lock_guard<mutex> lock(m_mutex);
    assert(stream);
    m_streams.push_back(stream);
}

void gnut::log::logger::remove(plog stream) {
    lock_guard<mutex> lock(m_mutex);
    remove_if(m_streams.begin(), m_streams.end(), [&] (const plog & p) -> bool { return stream == p; });
}

void gnut::log::logger::log(const string & message) {
    lock_guard<mutex> lock(m_mutex);
    for(auto stream : m_streams) {
        stream->write(message);
    }
}
