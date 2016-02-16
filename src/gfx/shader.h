//
// Created by Kyle on 2/15/2016.
//

#ifndef GNUT_SHADER_H
#define GNUT_SHADER_H

#include <string>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include "logger.h"

using std::string;
using std::shared_ptr;


class shader_program;

namespace gnut {
    namespace gfx {

        class shader {
        public:

            shader();
            shader(GLuint type, const string & source);
            ~shader();

            void load(GLuint type, const string & file);
            void compile(GLuint type, const string & shader);

            GLuint id() const;
            GLuint type() const;

        protected:

            GLuint m_id;
            GLuint m_type;

            friend class shader_program;

        };

        typedef shared_ptr<shader> pshader;

    }
}

#endif //GNUT_SHADER_H
