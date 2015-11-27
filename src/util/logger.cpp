//
// Created by Kyle on 11/25/2015.
//

#include "logger.h"

mutex gfx::logger::gmtime_mutex;

gfx::logger::logger() {
    m_loglevel = log_level::warning;
}

gfx::logger::~logger() {
    m_streams.clear();
}

gfx::log_level gfx::logger::level() {
    return this->m_loglevel;
}

void gfx::logger::level(gfx::log_level level) {
    lock_guard<mutex> lock(m_mutex);
    this->m_loglevel = level;
}

void gfx::logger::add(gfx::plog_stream stream) {
    lock_guard<mutex> lock(m_mutex);
    assert(stream);
    m_streams.push_back(stream);
}

void gfx::logger::remove(gfx::plog_stream stream) {
    lock_guard<mutex> lock(m_mutex);
    remove_if(m_streams.begin(), m_streams.end(), [&] (const plog_stream & p) -> bool { return stream == p; });
}

void gfx::logger::log(const string & level, const string & file, int line, const string & func, const string & message) {
    // threadid -- time --- file -- line -- level -- message
    stringstream sstream;
    {
        lock_guard<mutex> gmlock(gmtime_mutex);
        time_t tnow = time(NULL);
        tm* tm = gmtime(&tnow);
        std::thread::id id = std::this_thread::get_id();
        sstream << "[" << setfill(' ') << setw(14) << gfx::to_hex(id)  << "] "
                << setfill('0') << setw(2) << (tm->tm_mon  + 1) << "/"
                << setfill('0') << setw(2) << tm->tm_wday << "/"
                << setfill('0') << setw(4) << tm->tm_year + 1900 << " "
                << setfill('0') << setw(2) << tm->tm_hour << ":"
                << setfill('0') << setw(2) << tm->tm_min << ":"
                << setfill('0') << setw(2) << tm->tm_sec << "  ";
    }
    sstream << file << "(" << line << ")" << " " << setfill(' ')
            << setw(7) <<  level << " ~ " << message << "\n";
    string log_message = sstream.str();

    lock_guard<mutex> lock(m_mutex);
    for(auto stream : m_streams) {
        stream->write(log_message);
    }
}
