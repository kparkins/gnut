//
// Created by Kyle on 11/26/2015.
//

#ifndef GFX_CONSOLE_STREAM_H
#define GFX_CONSOLE_STREAM_H

#include <mutex>
#include <iostream>

#include "log_stream.h"

using std::cout;
using std::mutex;
using std::lock_guard;

namespace gfx {
    class console_stream : public log_stream {

    public:

        console_stream();
        ~console_stream();

        void write(const string & message) override;

    protected:

        static mutex m_mutex;

    };
}

#endif //GFX_CONSOLE_STREAM_H
