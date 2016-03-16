//
// Created by Kyle on 3/4/2016.
//

#ifndef GNUT_GROUND_H
#define GNUT_GROUND_H

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader_program.h"

namespace gnut {
    namespace gfx {
        class ground {
        public:

            ground();
            ~ground();

            void texture(const std::string & image);
            void normal_map(const std::string & image);
            glm::mat4 model_matrix();
            void draw();

        protected:

            void build_buffer();

            GLuint m_vao;
            GLuint m_vbo;
            GLuint m_texid;
            GLuint m_nmap;
            bool m_textured;
            glm::mat4 m_model;


            std::vector<float> m_data;

            float data[48] = {
                25.0f, 0.f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
                -25.0f, 0.f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
                -25.0f, 0.f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                25.0f, 0.f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
                25.0f, 0.f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
                -25.0f, 0.f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
            };
        };
    }
}

#endif //GNUT_GROUND_H
