//
// Created by Kyle on 11/26/2015.
//

#ifndef GNUT_FILE_STREAM_H
#define GNUT_FILE_STREAM_H

#include <string>
#include <fstream>
#include <stdexcept>

#include "log.h"

using std::string;
using std::ios_base;
using std::ofstream;
using std::shared_ptr;
using std::runtime_error;

namespace gnut {
    namespace log {
        class logfile : public log {

        public:

            logfile(const string & file, ios_base::openmode mode = ios_base::out);
            ~logfile();

            void write(const string &message) override;

        protected:

            ofstream m_ostream;

        };

        typedef shared_ptr<logfile> plogfile;

    }
}
#endif //GNUT_FILE_STREAM_H
