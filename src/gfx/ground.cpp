//
// Created by Kyle on 3/4/2016.
//

#include <SOIL.h>
#include <glm/gtc/type_ptr.hpp>
#include "ground.h"

gnut::gfx::ground::ground() : m_textured(false) {
    this->build_buffer();
    m_model = glm::mat4();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), &m_data[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(11 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

gnut::gfx::ground::~ground() {
    if(m_textured) {
        glDeleteTextures(1, &m_texid);
    }
}

void gnut::gfx::ground::build_buffer() {
    vec3 tan0, tan1, bit0, bit1;

    vec3 v3(data[0], data[1], data[2]);
    vec3 v1(data[8], data[9], data[10]);
    vec3 v2(data[16], data[17], data[18]);
    vec3 v0(data[32], data[33], data[34]);

    vec2 t0(data[38], data[39]);
    vec2 t1(data[14], data[15]);
    vec2 t2(data[22], data[23]);
    vec2 t3(data[6], data[7]);

    vec3 e0 = v1 - v0;
    vec3 e1 = v2 - v0;

    vec2 duv0 = t1 - t0;
    vec2 duv1 = t2 - t0;

    float c = 1.f / (duv0.x * duv1.y - duv1.x * duv0.y);

    tan0.x = c * (duv1.y * e0.x - duv0.y * e1.x);
    tan0.y = c * (duv1.y * e0.y - duv0.y * e1.y);
    tan0.z = c * (duv1.y * e0.z - duv0.y * e1.z);
    tan0 = normalize(tan0);

    bit0.x = c * (-duv1.x * e0.x - duv0.x * e1.x);
    bit0.y = c * (-duv1.x * e0.y - duv0.x * e1.y);
    bit0.z = c * (-duv1.x * e0.z - duv0.x * e1.z);
    bit0 = normalize(bit0);

    e0 = v3 - v0;
    e1 = v1 - v0;

    duv0 = t3 - t0;
    duv1 = t1 - t0;

    c = 1.f / (duv0.x * duv1.y - duv1.x * duv0.y);

    tan1.x = c * (duv1.y * e0.x - duv0.y * e1.x);
    tan1.y = c * (duv1.y * e0.y - duv0.y * e1.y);
    tan1.z = c * (duv1.y * e0.z - duv0.y * e1.z);
    tan1 = normalize(tan0);

    bit1.x = c * (-duv1.x * e0.x - duv0.x * e1.x);
    bit1.y = c * (-duv1.x * e0.y - duv0.x * e1.y);
    bit1.z = c * (-duv1.x * e0.z - duv0.x * e1.z);
    bit1 = normalize(bit0);

    for(int i = 0; i < 24; i += 8) {
        m_data.push_back(data[i]);
        m_data.push_back(data[i + 1]);
        m_data.push_back(data[i + 2]);
        m_data.push_back(data[i + 3]);
        m_data.push_back(data[i + 4]);
        m_data.push_back(data[i + 5]);
        m_data.push_back(data[i + 6]);
        m_data.push_back(data[i + 7]);
        m_data.push_back(tan0.x);
        m_data.push_back(tan0.y);
        m_data.push_back(tan0.z);
        m_data.push_back(bit0.x);
        m_data.push_back(bit0.y);
        m_data.push_back(bit0.z);
    }

    for(int i = 24; i < 48; i += 8) {
        m_data.push_back(data[i]);
        m_data.push_back(data[i + 1]);
        m_data.push_back(data[i + 2]);
        m_data.push_back(data[i + 3]);
        m_data.push_back(data[i + 4]);
        m_data.push_back(data[i + 5]);
        m_data.push_back(data[i + 6]);
        m_data.push_back(data[i + 7]);
        m_data.push_back(tan1.x);
        m_data.push_back(tan1.y);
        m_data.push_back(tan1.z);
        m_data.push_back(bit1.x);
        m_data.push_back(bit1.y);
        m_data.push_back(bit1.z);
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

void gnut::gfx::ground::normal_map(const std::string & image) {
    int width, height;
    unsigned char* texture = SOIL_load_image(image.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    assert(texture != nullptr);
    glGenTextures(1, &m_nmap);
    glBindTexture(GL_TEXTURE_2D, m_nmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(texture);
}

glm::mat4 gnut::gfx::ground::model_matrix() {
    return m_model;
}

void gnut::gfx::ground::draw() {
    assert(m_textured);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texid);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_nmap);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
