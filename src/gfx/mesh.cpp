#include <time.h>
#include "mesh.h"

gnut::gfx::face::face() {
}

gnut::gfx::face::face(unsigned int v0, unsigned int v1, unsigned int v2) : v0(v0), v1(v1), v2(v2) {

}

gnut::gfx::face::~face() {

}

bool gnut::gfx::face::contains(unsigned int v) {
    return v == v0 || v == v1 || v == v2;
}

void gnut::gfx::face::replace(unsigned int vold, unsigned int vnew) {
    if(vold == v0) {
        v0 = vnew;
    } else if(vold == v1) {
        v1 = vnew;
    } else if(vold == v2) {
        v2 = vnew;
    } else {
        assert(0);
    }
}

gnut::gfx::mesh::mesh() : m_debug(false) {
    m_bufferdata.clear();
}

gnut::gfx::mesh::~mesh() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void gnut::gfx::mesh::generate_buffer() {
    if(m_bufferdata.size() != 0) {
        m_bufferdata.clear();
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }
    if(m_debug) {
        return this->generate_dbuffer();
    }
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

    size_t size = (2 * sizeof(float) * 3 ) * 3 * m_faces.size();;
    GLsizei stride = 6 * sizeof(GL_FLOAT);

    glBufferData(GL_ARRAY_BUFFER, size, &m_bufferdata[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(3 * sizeof(GL_FLOAT)));

    glBindVertexArray(0);
}

void gnut::gfx::mesh::generate_dbuffer() {
    for(size_t j = 0; j < m_faces.size(); ++j) {
        vec3 color = m_colors[j];
        vec3 v0 = m_vertices[m_faces[j].v0];
        vec3 vn0 = m_vnormals[m_faces[j].v0];
        m_bufferdata.push_back(v0.x);
        m_bufferdata.push_back(v0.y);
        m_bufferdata.push_back(v0.z);
        m_bufferdata.push_back(vn0.x);
        m_bufferdata.push_back(vn0.y);
        m_bufferdata.push_back(vn0.z);
        m_bufferdata.push_back(color.x);
        m_bufferdata.push_back(color.y);
        m_bufferdata.push_back(color.z);

        vec3 v1 = m_vertices[m_faces[j].v1];
        vec3 vn1 = m_vnormals[m_faces[j].v1];
        m_bufferdata.push_back(v1.x);
        m_bufferdata.push_back(v1.y);
        m_bufferdata.push_back(v1.z);
        m_bufferdata.push_back(vn1.x);
        m_bufferdata.push_back(vn1.y);
        m_bufferdata.push_back(vn1.z);
        m_bufferdata.push_back(color.x);
        m_bufferdata.push_back(color.y);
        m_bufferdata.push_back(color.z);

        vec3 v2 = m_vertices[m_faces[j].v2];
        vec3 vn2 = m_vnormals[m_faces[j].v2];
        m_bufferdata.push_back(v2.x);
        m_bufferdata.push_back(v2.y);
        m_bufferdata.push_back(v2.z);
        m_bufferdata.push_back(vn2.x);
        m_bufferdata.push_back(vn2.y);
        m_bufferdata.push_back(vn2.z);
        m_bufferdata.push_back(color.x);
        m_bufferdata.push_back(color.y);
        m_bufferdata.push_back(color.z);
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    size_t size = (3 * sizeof(float) * 3 ) * 3 * m_faces.size();;
    GLsizei stride = 9 * sizeof(GL_FLOAT);

    glBufferData(GL_ARRAY_BUFFER, size, &m_bufferdata[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(3 * sizeof(GL_FLOAT)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid*>(6 * sizeof(GL_FLOAT)));

    glBindVertexArray(0);
}

void gnut::gfx::mesh::generate_colors() {
    srand(time(NULL));
    float r, g, b;
    for(size_t i = 0; i < m_faces.size(); ++i) {
        r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        m_colors.push_back(vec3(r, g, b));
    }
}

void gnut::gfx::mesh::debug(bool on) {
    m_debug = on;
    if(m_colors.size() == 0) {
        this->generate_colors();
    }
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
}

void gnut::gfx::mesh::print_adjacency() {
    std::cout << "Adjacency.. " << std::endl;
    for(auto & p : m_vfadjacency) {
        std::cout << p.first << " -- ";
        for(unsigned int neighbor : p.second) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
}

void gnut::gfx::mesh::print() {
    std::cout << "vertices -- " << m_vertices.size() << std::endl;
    for(int i = 0; i < m_vertices.size(); ++i) {
        std::cout << "x -- " << m_vertices[i].x << " y -- " << m_vertices[i].y << " z -- " << m_vertices[i].z << std::endl;
    }
}

void gnut::gfx::mesh::remove_face(unsigned int f) {
    gfx::face & face = m_faces[f];

    m_vfadjacency[face.v0].erase(f);
    m_vfadjacency[face.v1].erase(f);
    m_vfadjacency[face.v2].erase(f);

    m_faces[f] = m_faces[m_faces.size() - 1];
    m_fnormals[f] = m_fnormals[m_fnormals.size() - 1];

    m_vfadjacency[m_faces[f].v0].erase(m_faces.size() - 1);
    m_vfadjacency[m_faces[f].v1].erase(m_faces.size() - 1);
    m_vfadjacency[m_faces[f].v2].erase(m_faces.size() - 1);
    m_vfadjacency[m_faces[f].v0].insert(f);
    m_vfadjacency[m_faces[f].v1].insert(f);
    m_vfadjacency[m_faces[f].v2].insert(f);

    m_faces.resize(m_faces.size() - 1);
    m_fnormals.resize(m_fnormals.size() - 1);
}


void gnut::gfx::mesh::edge_collapse(unsigned int vi0, unsigned int vi1) {
    assert(vi0 < m_vertices.size() && vi1 < m_vertices.size());

    vec3 v0 = m_vertices[vi0];
    vec3 v1 = m_vertices[vi1];
    vec3 vnew = (v0 + v1) / 2.f;

    unordered_set<unsigned int> adjFaces;
    unsigned int vinew = static_cast<unsigned int>(m_vertices.size());
    m_vertices.push_back(vnew);

    for(unsigned int neighbor : m_vfadjacency[vi0]) {
        adjFaces.insert(neighbor);
    }
    for(unsigned int neighbor : m_vfadjacency[vi1]) {
        adjFaces.insert(neighbor);
    }

    std::cout << "Ajd faces -- ";
    for(unsigned int n : adjFaces) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    std::cout << "Rm         -- ";
    for(unsigned int face : adjFaces) {
        std::cout << face << " ";
        gfx::face & f = m_faces[face];
        if(f.contains(vi0) && f.contains(vi1)) {
            this->remove_face(face);
        } else {
            if(f.contains(vi1)) {
                f.replace(vi1, vinew);
            } else if(f.contains(vi0)) {
                f.replace(vi0, vinew);
            }
            vec3 a = m_vertices[f.v1] - m_vertices[f.v0];
            vec3 b = m_vertices[f.v2] - m_vertices[f.v0];
            m_fnormals[face] = glm::normalize(glm::cross(a, b));
        }
    }
    std::cout << std::endl;
    // recalculate vertex normal
    vec3 vnormal(0,0,0);
    for(unsigned int face : adjFaces) {

    }
}


