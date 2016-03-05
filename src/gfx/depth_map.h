//
// Created by Kyle on 3/4/2016.
//

#ifndef GNUT_DEPTH_MAP_H
#define GNUT_DEPTH_MAP_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "shader_program.h"

namespace gnut {
    namespace gfx {
        class depth_map {
        public:

            depth_map(uint32_t w, uint32_t h);
            ~depth_map();

            uint32_t width();
            uint32_t height();

            void enable();
            void disable();
            void render();
            GLuint framebuffer();
            GLuint depthmap();

        protected:

            pshader_program m_debugshader;
            pshader_program m_depthshader;

            uint32_t m_width;
            uint32_t m_height;

            GLuint m_dmap;
            GLuint m_fbo;

            GLuint m_vao;
            GLuint m_vbo;

            int viewport[4];

            float quad[30] = {
              -1.f, 1.f, 0.f, 0.f, 1.f,
              -1.f, -1.f, 0.f, 0.f, 0.f,
              1.f, 1.f, 0.f, 1.f, 1.f,

              1.f, 1.f, 0.f, 1.f, 1.f,
              -1.f, -1.f, 0.f, 0.f, 0.f,
              1.f, -1.f, 0.f, 1.f, 0.f
            };
        };
    }
}

#endif //GNUT_DEPTH_MAP_H
