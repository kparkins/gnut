//
// Created by Kyle on 11/26/2015.
//

#ifndef GNUT_FILE_STREAM_H
#define GNUT_FILE_STREAM_H

#include <string>
#include <fstream>
#include <stdexcept>

#include "log_stream.h"

using std::string;
using std::ios_base;
using std::ofstream;
using std::shared_ptr;
using std::runtime_error;

namespace gnut {
    class file_stream : public log_stream {

    public:

        file_stream();
        ~file_stream();

        void open(const string &file, ios_base::openmode mode = ios_base::out);
        void close();
        void write(const string & message) override;

    protected:

        ofstream m_ostream;

    };

    typedef shared_ptr<file_stream> pfile_stream;

}

#endif //GNUT_FILE_STREAM_H
