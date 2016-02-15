/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "console.h"

mutex gnut::log::console::m_mutex;

gnut::log::console::console() {

}

gnut::log::console::~console() {

}

void gnut::log::console::write(const string &message) {
    lock_guard<mutex> lock(m_mutex);
    cout << message;
}
