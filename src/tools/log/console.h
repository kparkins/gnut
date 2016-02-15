/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef GNUT_CONSOLE_STREAM_H
#define GNUT_CONSOLE_STREAM_H

#include <mutex>
#include <iostream>

#include "log.h"

using std::cout;
using std::mutex;
using std::lock_guard;

namespace gnut {
    namespace log {
        class console : public log {
        public:

            console();
            ~console();


            void write(const string &message) override;

        protected:

            static mutex m_mutex;

        };
        typedef shared_ptr<console> pconsole;
    }
}

#endif //GNUT_CONSOLE_STREAM_H
