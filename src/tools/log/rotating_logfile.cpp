/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "rotating_logfile.h"

gnut::log::rotating_logfile::rotating_logfile(const string &prefix,
        unsigned int max_files,
        unsigned int max_lines,
        ios_base::openmode mode)
   : m_prefix(prefix),
     m_mode(mode),
     m_maxfiles(max_files),
     m_maxlines(max_lines),
     m_fileindex(0),
     m_lineindex(0)
{
    stringstream sstream;
    sstream << prefix << "-" << time(0);
    m_prefix = sstream.str();
    sstream << "-" << m_fileindex++ << ".log";

    m_logfile.open(sstream.str(), mode);
    if(!m_logfile.is_open() || !m_logfile.good()) {
        throw runtime_error("Unable to open rotating logfile " + prefix);
    }
}

gnut::log::rotating_logfile::~rotating_logfile() {
    if(m_logfile.is_open()) {
        m_logfile.close();
    }
}

void gnut::log::rotating_logfile::max_files(unsigned int n) {
    m_maxfiles = n;
}

unsigned int gnut::log::rotating_logfile::max_files() {
    return m_maxfiles;
}

void gnut::log::rotating_logfile::max_lines(unsigned int l) {
    m_maxlines = l;
}

unsigned int gnut::log::rotating_logfile::max_lines() {
    return m_maxlines;
}

string gnut::log::rotating_logfile::prefix() {
    return m_prefix;
}

void gnut::log::rotating_logfile::write(const string & message) {
    if(m_lineindex >= m_maxlines) {
        stringstream sstream;
        sstream << m_prefix  << "-" << m_fileindex++ % m_maxfiles << ".log";
        m_logfile.close();
        m_logfile.open(sstream.str(), m_mode);
        m_lineindex = 0;
    }

    if(m_logfile.is_open() && m_logfile.good()) {
        m_logfile << message;
        ++m_lineindex;
    }
}
