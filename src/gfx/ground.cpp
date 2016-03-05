//
// Created by Kyle on 3/4/2016.
//

#include <SOIL.h>
#include <glm/gtc/type_ptr.hpp>
#include "ground.h"

gnut::gfx::ground::ground() : m_textured(false) {
    m_model = glm::mat4();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), &data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

gnut::gfx::ground::~ground() {
    if(m_textured) {
        glDeleteTextures(1, &m_texid);
    }
}

void gnut::gfx::ground::texture(const std::string &image) {
    if(m_textured) {
        glDeleteTextures(1, &m_texid);
    }
    int width, height;
    unsigned char* texture = SOIL_load_image(image.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    assert(texture != nullptr);
    glGenTextures(1, &m_texid);
    glBindTexture(GL_TEXTURE_2D, m_texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(texture);
    m_textured = true;
}

glm::mat4 gnut::gfx::ground::model_matrix() {
    return m_model;
}

void gnut::gfx::ground::draw(gnut::gfx::pshader_program shader) {
    assert(m_textured);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texid);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
