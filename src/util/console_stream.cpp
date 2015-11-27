//
// Created by Kyle on 11/26/2015.
//

#include "console_stream.h"

mutex gnut::console_stream::m_mutex;

gnut::console_stream::console_stream() {

}

gnut::console_stream::~console_stream() {

}

void gnut::console_stream::write(const string &message) {
    lock_guard<mutex> lock(m_mutex);
    cout << message;
}
