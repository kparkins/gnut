//
// Created by Kyle on 11/25/2015.
//

#include "shader_program.h"

gfx::shader_program::shader_program() {

}

gfx::shader_program::~shader_program() {
    glDeleteProgram(m_id);
}

gfx::shader_program& gfx::shader_program::attach(GLuint type, const string & shader) {
    int success = 1;
    const char* source = shader.c_str();
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &source, 0);
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
        //TODO a robust logger
    }
    return *this;
}

void gfx::shader_program::link_program() {

}

void gfx::shader_program::enable() {

}

void gfx::shader_program::disable() {

}

GLuint gfx::shader_program::get_uniform(const string &uniform) {
    return 0;
}

GLuint gfx::shader_program::get_attribute(const string &attribute) {
    return 0;
}

int gfx::shader_program::add_uniform(const string &name) {
    return 0;
}

int gfx::shader_program::add_attribute(const string &name) {
    return 0;
}
