//
// Created by Kyle on 11/26/2015.
//

#include "file_stream.h"

gnut::file_stream::file_stream() {

}

gnut::file_stream::~file_stream() {
    if(m_ostream.is_open()) {
        m_ostream.close();
    }
}

void gnut::file_stream::open(const string &file, ios_base::openmode mode) {
    m_ostream.open(file, mode);
    if(!m_ostream.is_open()) {
        throw runtime_error("Could not open log file for writing.");
    }
}

void gnut::file_stream::close() {
    m_ostream.close();
}

void gnut::file_stream::write(const string &message) {
    m_ostream << message;
}
