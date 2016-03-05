//
// Created by Kyle on 3/4/2016.
//

#include "depth_map.h"

gnut::gfx::depth_map::depth_map(uint32_t w, uint32_t h) {
    m_width = w;
    m_height = h;

    shader depth_vertex(GL_VERTEX_SHADER, "res/shaders/depth.vert");
    shader depth_fragment(GL_FRAGMENT_SHADER, "res/shaders/depth.frag");
    m_depthshader = make_shared<gfx::shader_program>();
    m_depthshader->attach(depth_vertex);
    m_depthshader->attach(depth_fragment);
    m_depthshader->link_program();

    shader debug_vertex(GL_VERTEX_SHADER, "res/shaders/render_depth.vert");
    shader debug_fragment(GL_FRAGMENT_SHADER, "res/shaders/render_depth.frag");
    m_debugshader = make_shared<gfx::shader_program>();
    m_debugshader->attach(debug_vertex);
    m_debugshader->attach(debug_fragment);
    m_debugshader->link_program();

    glGenFramebuffers(1, &m_fbo);
    glGenTextures(1, &m_dmap);
    glBindTexture(GL_TEXTURE_2D, m_dmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fbo, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
}

gnut::gfx::depth_map::~depth_map() {
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

uint32_t gnut::gfx::depth_map::width() {
    return m_width;
}

uint32_t gnut::gfx::depth_map::height() {
    return m_height;
}

GLuint gnut::gfx::depth_map::framebuffer() {
    return m_fbo;
}

GLuint gnut::gfx::depth_map::depthmap() {
    return m_dmap;
}


void gnut::gfx::depth_map::enable() {
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, m_width, m_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);

}

void gnut::gfx::depth_map::disable() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gnut::gfx::depth_map::render() {
    m_debugshader->uniform("texture_sampler",3);
    m_debugshader->enable();
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_dmap);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    m_depthshader->disable();
}
