#include "mesh.h"

gnut::gfx::face::face() {

}

gnut::gfx::face::face(unsigned int v0, unsigned int v1, unsigned int v2) : v0(v0), v1(v1), v2(v2) {

}

gnut::gfx::face::~face() {

}

gnut::gfx::mesh::mesh() {
    m_bufferdata.clear();
}

gnut::gfx::mesh::~mesh() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void gnut::gfx::mesh::generate_buffer() {
    m_bufferdata.clear();
    for(size_t j = 0; j < m_faces.size(); ++j) {
        vec3 v0 = m_vertices[m_faces[j].v0];
        vec3 vn0 = m_vnormals[m_faces[j].v0];
        m_bufferdata.push_back(v0.x);
        m_bufferdata.push_back(v0.y);
        m_bufferdata.push_back(v0.z);
        m_bufferdata.push_back(vn0.x);
        m_bufferdata.push_back(vn0.y);
        m_bufferdata.push_back(vn0.z);

        vec3 v1 = m_vertices[m_faces[j].v1];
        vec3 vn1 = m_vnormals[m_faces[j].v1];
        m_bufferdata.push_back(v1.x);
        m_bufferdata.push_back(v1.y);
        m_bufferdata.push_back(v1.z);
        m_bufferdata.push_back(vn1.x);
        m_bufferdata.push_back(vn1.y);
        m_bufferdata.push_back(vn1.z);

        vec3 v2 = m_vertices[m_faces[j].v2];
        vec3 vn2 = m_vnormals[m_faces[j].v2];
        m_bufferdata.push_back(v2.x);
        m_bufferdata.push_back(v2.y);
        m_bufferdata.push_back(v2.z);
        m_bufferdata.push_back(vn2.x);
        m_bufferdata.push_back(vn2.y);
        m_bufferdata.push_back(vn2.z);
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    size_t size = (2 * sizeof(float) * 3) * 3 * m_faces.size();
    glBufferData(GL_ARRAY_BUFFER, size, &m_bufferdata[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<GLvoid*>(3 * sizeof(GL_FLOAT)));

    glBindVertexArray(0);
}

void gnut::gfx::mesh::draw() {
    glBindVertexArray(this->m_vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_bufferdata.size() / 2));
    glBindVertexArray(0);
}

void gnut::gfx::mesh::compute_fnormals() {
    vec3 v0, v1, v2;
    m_fnormals.clear();
    m_fnormals.resize(m_faces.size());
    for(size_t i = 0; i < m_faces.size(); ++i) {
        v0 = m_vertices[m_faces[i].v0];
        v1 = m_vertices[m_faces[i].v1];
        v2 = m_vertices[m_faces[i].v2];
        m_fnormals[i] = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }
}

void gnut::gfx::mesh::compute_vnormals() {
    vec3 v0, v1, v2, a, b;
    unordered_map<unsigned int, float> weights;
    m_vnormals.resize(m_vertices.size());
    for(auto & p : m_vfadjacency) {
        vec3 vnormal(0,0,0);
        float total_weight = 0.f;
        weights.clear();
        for(unsigned int neighbor : p.second) {
            face face = m_faces[neighbor];
            if(p.first == face.v0) {
                a = m_vertices[face.v1] - m_vertices[face.v0];
                b = m_vertices[face.v2] - m_vertices[face.v0];
            } else if(p.first == face.v1) {
                a = m_vertices[face.v2] - m_vertices[face.v1];
                b = m_vertices[face.v0] - m_vertices[face.v1];
            } else {
                a = m_vertices[face.v0] - m_vertices[face.v2];
                b = m_vertices[face.v1] - m_vertices[face.v2];
            }
            weights[neighbor] = glm::acos(glm::dot(a, b) / (glm::length(a) * glm::length(b)));
            total_weight += weights[neighbor];
        }
        for(unsigned int neighbor : p.second) {
            vnormal += m_fnormals[neighbor] * (weights[neighbor] / total_weight);
        }
        m_vnormals[p.first] = normalize(vnormal);
    }
}

void gnut::gfx::mesh::compute_vfadjacency() {
    for(size_t i = 0; i < m_faces.size(); ++i) {
        m_vfadjacency[m_faces[i].v0].insert(i);
        m_vfadjacency[m_faces[i].v1].insert(i);
        m_vfadjacency[m_faces[i].v2].insert(i);
    }
    /*
    for(auto & p : m_vfadjacency) {
        std::cout << p.first;
        for(unsigned int neighbor : p.second) {
            std::cout << " - " << neighbor;
        }
        std::cout << std::endl;
    }*/
}


