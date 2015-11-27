//
// Created by Kyle on 11/25/2015.
//

#include <assert.h>
#include "logger.h"

mutex gfx::logger::gmtime_mutex;

gfx::logger::logger() {

}

gfx::logger::logger(const string & tag) {
    this->m_tag = tag;
}

gfx::logger::~logger() {
    m_streams.clear();
}

gfx::log_level gfx::logger::level() {
    return this->m_loglevel;
}

void gfx::logger::level(gfx::log_level level) {
    this->m_loglevel = level;
}

void gfx::logger::add(gfx::postream stream) {
    assert(stream);
    m_streams.push_back(stream);
}

void gfx::logger::remove(gfx::postream stream) {
   remove_if(m_streams.begin(), m_streams.end(),
        [&] (const postream & p) -> bool {
            return stream == p;
   });
}

void gfx::logger::tag(const string &tag) {
    m_tag = tag;
}

string gfx::logger::tag() {
    return m_tag;
}

void gfx::logger::log(const string & level, const string & file, int line, const string & func, const string & message) {
   // lock_guard<mutex> lock(m_mutex);
    //stringstream sstream;
    // time -- level -- tag -- file -- line -- func -- threadId -- message
    {
     //   lock_guard<mutex> lock(gmtime_mutex);
        //time_t tnow = time(NULL);
        //tm* tm = gmtime(&tnow);
        /*sstream << setfill('0') << setw(2) << (tm->tm_mon  + 1) << "/"
                << setfill('0') << setw(2) << tm->tm_wday << "/"
                << setfill('0') << setw(4) << tm->tm_year + 1900 << " "
                << setfill('0') << setw(2) << tm->tm_hour << ":"
                << setfill('0') << setw(2) << tm->tm_min << ":"
                << setfill('0') << setw(2) << tm->tm_sec << " ";
                */
    }

   // sstream << message << "\n";
    std::cout << message << std::endl;
}
