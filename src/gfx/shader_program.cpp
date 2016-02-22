/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "shader_program.h"

gnut::gfx::shader_program::shader_program() : m_linked(false), m_enabled(false) {
}

gnut::gfx::shader_program::~shader_program() {
    glDeleteProgram(m_id);
}

gnut::gfx::shader_program& gnut::gfx::shader_program::attach(GLuint id) {
    assert(!m_linked);
    m_shaders.push_back(id);
    return *this;
}

gnut::gfx::shader_program& gnut::gfx::shader_program::attach(const gnut::gfx::shader & shader) {
    assert(!m_linked);
    m_shaders.push_back(shader.id());
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

void gnut::gfx::shader_program::uniform(const string & uniform, const mat4 & matrix) {
    glUseProgram(m_id);
    if(m_uniforms.find(uniform) != m_uniforms.end()) {
        glUniformMatrix4fv(m_uniforms[uniform], 1, GL_FALSE, glm::value_ptr(matrix));
        glUseProgram(0);
        return;
    }
    GLint location = glGetUniformLocation(m_id, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on shader program with id -- " << m_id);
        glUseProgram(0);
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    m_uniforms[uniform] = location;
    glUseProgram(0);
}

void gnut::gfx::shader_program::uniform(const string & uniform, const vec3 & vector) {
    glUseProgram(m_id);
    if(m_uniforms.find(uniform) != m_uniforms.end()) {
        glUniform3fv(m_uniforms[uniform], 1, glm::value_ptr(vector));
        glUseProgram(0);
        return;
    }
    GLint location = glGetUniformLocation(m_id, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on shader program with id -- " << m_id);
        glUseProgram(0);
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vector));
    m_uniforms[uniform] = location;
    glUseProgram(0);
}

void gnut::gfx::shader_program::uniform(const string & uniform, float value) {
    glUseProgram(m_id);
    if(m_uniforms.find(uniform) != m_uniforms.end()) {
        glUniform1f(m_uniforms[uniform], value);
        glUseProgram(0);
        return;
    }
    GLint location = glGetUniformLocation(m_id, uniform.c_str());
    if(location == -1) {
        LOG_ERROR("Unable to locate uniform -- " << uniform << " on shader program with id -- " << m_id);
        glUseProgram(0);
        return;
    }
    glUniform1f(location, value);
    m_uniforms[uniform] = location;
    glUseProgram(0);
}
