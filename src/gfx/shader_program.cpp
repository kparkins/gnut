//
// Created by Kyle on 11/25/2015.
//

#include "shader_program.h"

gnut::shader_program::shader_program() {

}

gnut::shader_program::~shader_program() {
    glDeleteProgram(m_id);
}

gnut::shader_program& gnut::shader_program::attach(GLuint type, const string & shader) {
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

void gnut::shader_program::link_program() {

}

void gnut::shader_program::enable() {

}

void gnut::shader_program::disable() {

}

GLuint gnut::shader_program::get_uniform(const string &uniform) {
    return 0;
}

GLuint gnut::shader_program::get_attribute(const string &attribute) {
    return 0;
}

int gnut::shader_program::add_uniform(const string &name) {
    return 0;
}

int gnut::shader_program::add_attribute(const string &name) {
    return 0;
}
