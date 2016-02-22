//
// Created by Kyle on 2/15/2016.
//

#include "shader.h"

gnut::gfx::shader::shader() {

}

gnut::gfx::shader::shader(GLuint type, const string & source) {
    this->load(type, source);
}

gnut::gfx::shader::~shader() {

}

void gnut::gfx::shader::load(GLuint type, const string & file) {
    string source = gnut::read_file(file);
    if(source == "") {
        LOG_ERROR("Error. Could not read shader file " << file);
        return;
    }
    this->compile(type, source);
}

void gnut::gfx::shader::compile(GLuint type, const string & shader) {
    m_type = type;
    int success = 0;
    GLchar error_log[512];
    const char* source = shader.c_str();
    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &source, 0);
    glCompileShader(m_id);
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(m_id, 512, 0, error_log);
        LOG_ERROR(error_log);
    }
}

GLuint gnut::gfx::shader::id() const {
    return m_id;
}

GLuint gnut::gfx::shader::type() const {
    return m_type;
}

