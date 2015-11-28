//
// Created by Kyle on 11/27/2015.
//

#ifndef GNUT_ROTATING_LOGFILE_H
#define GNUT_ROTATING_LOGFILE_H

#include <fstream>
#include <stdexcept>

#include "log.h"

using std::ios_base;
using std::fstream;
using std::runtime_error;

namespace gnut {
    namespace log {
        class rotating_logfile : public log {
        public:

            rotating_logfile(const string & prefix, unsigned int max_files, unsigned int max_lines,
                             ios_base::openmode mode = ios_base::out);
            ~rotating_logfile();

            void max_files(unsigned int n);
            unsigned int max_files();

            void max_liles(unsigned int l);
            unsigned int max_lines();

            string prefix();

            void write(const string & message) override;

        protected:

            string m_prefix;
            fstream m_logfile;
            ios_base::openmode m_mode;

            unsigned int m_maxfiles;
            unsigned int m_maxlines;
            unsigned int m_fileindex;
            unsigned int m_lineindex;

        };

        typedef shared_ptr<rotating_logfile> protating_logfile;
    }
}

#endif //GNUT_ROTATING_LOGFILE_H
