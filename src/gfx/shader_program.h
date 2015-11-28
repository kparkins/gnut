//
// Created by Kyle on 11/25/2015.
//

#ifndef GNUT_SHADER_PROGRAM_H
#define GNUT_SHADER_PROGRAM_H

#include <string>
#include <memory>
#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>

#include "logger.h"

using std::string;
using std::shared_ptr;

namespace gnut {
    class shader_program {
    public:

        shader_program();
        ~shader_program();

        shader_program& attach(GLuint type, const string & shader);
        void link_program();
        void enable();
        void disable();
        GLuint get_uniform(const string &uniform);
        GLuint get_attribute(const string &attribute);
        int add_uniform(const string & name);
        int add_attribute(const string & name);


    private:

        GLuint m_id;

    };

    typedef shared_ptr<shader_program> pshader_program;
}
#endif //GNUT_SHADER_PROGRAM_H
