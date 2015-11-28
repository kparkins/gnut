//
// Created by Kyle on 11/26/2015.
//

#include "logfile.h"

gnut::log::logfile::logfile(const string & file, ios_base::openmode mode) {
    m_ostream.open(file, mode);
    if(!m_ostream.is_open()) {
        throw runtime_error("Could not open log file for writing.");
    }
}

gnut::log::logfile::~logfile() {
    if(m_ostream.is_open()) {
        m_ostream.close();
    }
}

void gnut::log::logfile::write(const string &message) {
    m_ostream << message;
}
