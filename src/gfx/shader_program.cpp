/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#include "shader_program.h"

gnut::gfx::shader_program::shader_program() {

}

gnut::gfx::shader_program::~shader_program() {
    glDeleteProgram(m_id);
}

gnut::gfx::shader_program& gnut::gfx::shader_program::attach(GLuint type, const string & shader) {
    int success = 1;
    const char* source = shader.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
    }
    return *this;
}

void gnut::gfx::shader_program::link_program() {

}

void gnut::gfx::shader_program::enable() {

}

void gnut::gfx::shader_program::disable() {

}

GLuint gnut::gfx::shader_program::get_uniform(const string &uniform) {
    return 0;
}

GLuint gnut::gfx::shader_program::get_attribute(const string &attribute) {
    return 0;
}

int gnut::gfx::shader_program::add_uniform(const string &name) {
    return 0;
}

int gnut::gfx::shader_program::add_attribute(const string &name) {
    return 0;
}
