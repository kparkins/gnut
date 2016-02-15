/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "shader_program.h"

gnut::gfx::shader_program::shader_program() : m_linked(false), m_enabled(false) {
}

gnut::gfx::shader_program::~shader_program() {
    glDeleteProgram(m_id);
}

gnut::gfx::shader_program& gnut::gfx::shader_program::attach(GLuint type, const string & shader) {
    assert(!m_linked);
    int success = 0;
    GLchar error_log[512];
    const char* source = shader.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(id, 512, 0, error_log);
        LOG_ERROR(error_log);
    }
    m_shaders.push_back(id);
    return *this;
}

void gnut::gfx::shader_program::link_program() {
    assert(!m_linked);
    m_id = glCreateProgram();
    for(GLuint shader : m_shaders) {
        glAttachShader(m_id, shader);
    }
    GLint link_success = GL_TRUE;
    glLinkProgram(m_id);
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &link_success);
    if(!link_success) {
        GLchar error_log[512];
        glGetShaderInfoLog(m_id, 512, 0, error_log);
        LOG_ERROR(error_log);
        return;
    }
    for(GLuint shader : m_shaders) {
        glDetachShader(m_id, shader);
        glDeleteShader(shader);
    }
    m_shaders.clear();
    m_linked = true;
}

void gnut::gfx::shader_program::enable() {
    assert(m_linked);
    glUseProgram(m_id);
    m_enabled = true;
}

void gnut::gfx::shader_program::disable() {
    assert(m_linked);
    assert(m_enabled);
    glUseProgram(0);
    m_enabled = false;
}

GLuint gnut::gfx::shader_program::id() {
    return m_id;
}

bool gnut::gfx::shader_program::linked() {
    return m_linked;
}

