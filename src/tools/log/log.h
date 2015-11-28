//
// Created by Kyle on 11/26/2015.
//

#ifndef GNUT_LOG_STREAM_H
#define GNUT_LOG_STREAM_H

#include <ctime>
#include <string>
#include <memory>
#include <sstream>

using std::string;
using std::shared_ptr;
using std::stringstream;

namespace gnut {
    namespace log {
        class log {
        public:

            virtual ~log() { }
            virtual void write(const string &message) = 0;

        };

        typedef shared_ptr<log> plog;
    }
}
#endif //GNUT_LOG_STREAM_H
