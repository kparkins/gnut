/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef GNUT_UTILITY_H
#define GNUT_UTILITY_H

#include <mutex>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>

using std::hex;
using std::setw;
using std::mutex;
using std::string;
using std::setfill;
using std::lock_guard;
using std::stringstream;

namespace gnut {
    template<typename Tr, typename Tn, typename Tm>
    Tr mod(Tn n, Tm m) {
        Tr r = n - (n / m) * m;
        if (r < 0) {
            return r + m;
        }
        return r;
    };


    template<typename T>
    string to_hex(T i) {
        stringstream sstream;
        sstream << "0x" << setfill('0') << setw(sizeof(T) * 2) << hex << i;
        return sstream.str();
    }

    string gmt_datetime();
}
#endif //GNUT_UTILITY_H
