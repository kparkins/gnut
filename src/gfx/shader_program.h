//
// Created by Kyle on 11/25/2015.
//

#ifndef GNUT_SHADER_PROGRAM_H
#define GNUT_SHADER_PROGRAM_H

#include <string>
#include <memory>
#include <cassert>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include "logger.h"

using std::vector;
using std::string;
using std::shared_ptr;

namespace gnut {
    namespace gfx {
        class shader_program {
        public:

            shader_program();
            ~shader_program();

            shader_program& attach(GLuint type, const string &shader);

            bool linked();
            void link_program();
            void enable();
            void disable();

            GLuint id();

        private:

            bool m_linked;
            bool m_enabled;
            GLuint m_id;
            vector<GLuint> m_shaders;

        };

        typedef shared_ptr<shader_program> pshader_program;
    }
}
#endif //GNUT_SHADER_PROGRAM_H
