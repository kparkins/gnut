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
            pshader_program depth_shader();
            pshader_program debug_shader();
            void light_view(glm::mat4 lview);
            void light_projection(glm::mat4 lproj);
            glm::mat4 light_matrix();
            GLuint framebuffer();
            GLuint depthmap();

        protected:

            pshader_program m_debugshader;
            pshader_program m_depthshader;

            glm::mat4 m_lightview;
            glm::mat4 m_lightprojection;

            uint32_t m_width;
            uint32_t m_height;

            GLuint m_dmap;
            GLuint m_fbo;

            GLuint m_vao;
            GLuint m_vbo;

            int viewport[4];

            float shadow_bordercolor[4] = {
                    1.f, 1.f, 1.f, 1.f
            };

            float quad[30] = {
              -1.f, 1.f, 0.f, 0.f, 1.f,
              -1.f, -1.f, 0.f, 0.f, 0.f,
              1.f, 1.f, 0.f, 1.f, 1.f,

              1.f, 1.f, 0.f, 1.f, 1.f,
              -1.f, -1.f, 0.f, 0.f, 0.f,
              1.f, -1.f, 0.f, 1.f, 0.f
            };
        };
        typedef std::shared_ptr<gnut::gfx::depth_map> pdepth_map;
    }
}

#endif //GNUT_DEPTH_MAP_H
