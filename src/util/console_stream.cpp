//
// Created by Kyle on 11/26/2015.
//

#include "console_stream.h"

mutex gfx::console_stream::m_mutex;

gfx::console_stream::console_stream() {

}

gfx::console_stream::~console_stream() {

}

void gfx::console_stream::write(const string &message) {
    lock_guard<mutex> lock(m_mutex);
    cout << message;
}
