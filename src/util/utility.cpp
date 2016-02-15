/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "utility.h"

string gnut::gmt_datetime() {
    stringstream sstream;
    static mutex gmtime_mutex;
    lock_guard<mutex> gmlock(gmtime_mutex);

    time_t tnow = time(NULL);
    tm *tm = gmtime(&tnow);
    if(!tm) {
        return string();
    }
    sstream << setfill('0') << setw(2) << tm->tm_mon + 1 << "/"
            << setfill('0') << setw(2) << tm->tm_wday << "/"
            << setfill('0') << setw(4) << tm->tm_year + 1900 << " "
            << setfill('0') << setw(2) << tm->tm_hour << ":"
            << setfill('0') << setw(2) << tm->tm_min << ":"
            << setfill('0') << setw(2) << tm->tm_sec << "  ";
    return sstream.str();
}
