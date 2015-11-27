//
// Created by Kyle on 11/26/2015.
//

#ifndef GFX_LOG_STREAM_H
#define GFX_LOG_STREAM_H

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

namespace gfx {
    class log_stream {

    public:

        virtual ~log_stream() { }
        virtual void write(const string &message) = 0;

    };

    typedef shared_ptr<log_stream> plog_stream;
}
#endif //GFX_LOG_STREAM_H
