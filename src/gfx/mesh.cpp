#include "mesh.h"

gnut::gfx::mesh::mesh() {
    m_bufferdata.clear();
}

gnut::gfx::mesh::~mesh() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void gnut::gfx::mesh::generate_buffer() {
    for(size_t j = 0; j < m_faces.size(); ++j) {
        vec3 face = m_faces[j];
        for(unsigned int i = 0; i < 3; ++i) {
            m_bufferdata.push_back(m_vertices[face[i]].x);
            m_bufferdata.push_back(m_vertices[face[i]].y);
            m_bufferdata.push_back(m_vertices[face[i]].z);

            /*
            m_bufferdata.push_back(m_fnormals[j].x);
            m_bufferdata.push_back(m_fnormals[j].y);
            m_bufferdata.push_back(m_fnormals[j].z);
            */
            m_bufferdata.push_back(m_vnormals[face[i]].x);
            m_bufferdata.push_back(m_vnormals[face[i]].y);
            m_bufferdata.push_back(m_vnormals[face[i]].z);
        }
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
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(this->m_faces.size() * 3));
    glBindVertexArray(0);
}

void gnut::gfx::mesh::compute_fnormals() {
    vec3 v0, v1, v2;
    m_fnormals.clear();
    m_fnormals.resize(m_faces.size());
    for(size_t i = 0; i < m_faces.size(); ++i) {
        v0 = m_vertices[m_faces[i].x];
        v1 = m_vertices[m_faces[i].y];
        v2 = m_vertices[m_faces[i].z];
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
            vec3 face = m_faces[neighbor];
            if(p.first == face.x) {
                a = m_vertices[face.y] - m_vertices[face.x];
                b = m_vertices[face.z] - m_vertices[face.x];
            } else if(p.first == face.y) {
                a = m_vertices[face.z] - m_vertices[face.y];
                b = m_vertices[face.x] - m_vertices[face.y];
            } else {
                a = m_vertices[face.x] - m_vertices[face.z];
                b = m_vertices[face.y] - m_vertices[face.z];
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
        m_vfadjacency[m_faces[i].x].insert(i);
        m_vfadjacency[m_faces[i].y].insert(i);
        m_vfadjacency[m_faces[i].z].insert(i);
    }
}


