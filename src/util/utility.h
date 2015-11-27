//
// Created by Kyle on 11/26/2015.
//

#ifndef GFX_UTILITY_H
#define GFX_UTILITY_H

#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::hex;
using std::setw;
using std::string;
using std::setfill;
using std::stringstream;

namespace gfx {
    template <typename Tr, typename Tn, typename Tm>
    Tr mod(Tn n, Tm m) {
        Tr r = n - (n / m) * m;
        if(r < 0) {
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
}
#endif //GFX_UTILITY_H
