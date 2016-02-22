/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef GNUT_SHADER_PROGRAM_H
#define GNUT_SHADER_PROGRAM_H

#include <string>
#include <memory>
#include <cassert>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger.h"
#include "shader.h"

using std::vector;
using std::string;
using std::shared_ptr;

using namespace glm;

namespace gnut {
    namespace gfx {
        class shader_program {
        public:

            shader_program();
            ~shader_program();

            shader_program& attach(GLuint id);
            shader_program& attach(const gfx::shader & shader);

            bool linked();
            void link_program();
            void enable();
            void disable();

            GLuint id();

            void uniform(const string & uniform, const mat4 & matrix);
            void uniform(const string & uniform, const vec3 & vector);
            void uniform(const string & uniform, float value);

        private:

            bool m_linked;
            bool m_enabled;
            GLuint m_id;
            vector<GLuint> m_shaders;

            unordered_map<string, GLuint> m_uniforms;


        };

        typedef shared_ptr<shader_program> pshader_program;
    }
}
#endif //GNUT_SHADER_PROGRAM_H
