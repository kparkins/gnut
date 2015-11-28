//
// Created by Kyle on 11/26/2015.
//

#ifndef GNUT_LOG_STREAM_H
#define GNUT_LOG_STREAM_H

#include <string>
#include <memory>
#include <sstream>

using std::string;
using std::shared_ptr;

namespace gnut {
    class log_stream {

    public:

        virtual ~log_stream() { }
        virtual void write(const string &message) = 0;

    };

    typedef shared_ptr<log_stream> plog_stream;
}
#endif //GNUT_LOG_STREAM_H
