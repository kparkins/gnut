#include <time.h>
#include "mesh.h"

gnut::gfx::quadric gnut::gfx::quadric::operator+(const gfx::quadric & q)  {
    quadric qn;
    qn.m[0][0] = m[0][0] + q.m[0][0];
    qn.m[0][1] = m[0][1] + q.m[0][1];
    qn.m[0][2] = m[0][2] + q.m[0][2];
    qn.m[0][3] = m[0][3] + q.m[0][3];

    qn.m[1][0] = m[0][0] + q.m[1][0];
    qn.m[1][1] = m[0][1] + q.m[1][1];
    qn.m[1][2] = m[0][2] + q.m[1][2];
    qn.m[1][3] = m[0][3] + q.m[1][3];

    qn.m[2][0] = m[0][0] + q.m[2][0];
    qn.m[2][1] = m[0][1] + q.m[2][1];
    qn.m[2][2] = m[0][2] + q.m[2][2];
    qn.m[2][3] = m[0][3] + q.m[2][3];

    qn.m[3][0] = m[3][0] + q.m[3][0];
    qn.m[3][1] = m[3][1] + q.m[3][1];
    qn.m[3][2] = m[3][2] + q.m[3][2];
    qn.m[3][3] = m[3][3] + q.m[3][3];

    return qn;
}


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

bool gnut::gfx::face::operator==(const face &rhs) {
    unsigned int min0 = std::min(v0, std::min(v1, v2));
    unsigned int min1 = std::min(rhs.v0, std::min(rhs.v1, rhs.v2));

    unsigned int max0 = std::max(v0, std::max(v1, v2));
    unsigned int max1 = std::max(rhs.v0, std::max(rhs.v1, rhs.v2));

    unsigned int mid0 = (v0 > min0 && v0 < max0) ? v0 : ((v1 > min0 && v1 < max0) ?  v1 : v2);
    unsigned int mid1 = (rhs.v0 > min1 && rhs.v0 < max1) ? rhs.v0 : ((rhs.v1 > min1 && rhs.v1 < max1) ? rhs.v1 : rhs.v2);

    return min0 == min1 && mid0 == mid1 && max0 == max1;
}

gnut::gfx::mesh::mesh() : m_maxvertice(FLT_MIN),
                          m_max(glm::vec3(FLT_MIN, FLT_MIN, FLT_MIN)),
                          m_min(glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX))
{
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
    for(auto it = m_faces.begin(); it != m_faces.end(); ++it) {
        vec3 color = m_colors[it->first];
        vec3 v0 = m_vertices[it->second.v0];
        vec3 vn0 = m_vnormals[it->second.v0];
        m_bufferdata.push_back(v0.x);
        m_bufferdata.push_back(v0.y);
        m_bufferdata.push_back(v0.z);
        m_bufferdata.push_back(vn0.x);
        m_bufferdata.push_back(vn0.y);
        m_bufferdata.push_back(vn0.z);
        m_bufferdata.push_back(color.x);
        m_bufferdata.push_back(color.y);
        m_bufferdata.push_back(color.z);

        vec3 v1 = m_vertices[it->second.v1];
        vec3 vn1 = m_vnormals[it->second.v1];
        m_bufferdata.push_back(v1.x);
        m_bufferdata.push_back(v1.y);
        m_bufferdata.push_back(v1.z);
        m_bufferdata.push_back(vn1.x);
        m_bufferdata.push_back(vn1.y);
        m_bufferdata.push_back(vn1.z);
        m_bufferdata.push_back(color.x);
        m_bufferdata.push_back(color.y);
        m_bufferdata.push_back(color.z);

        vec3 v2 = m_vertices[it->second.v2];
        vec3 vn2 = m_vnormals[it->second.v2];
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
    for(auto it = m_faces.begin(); it != m_faces.end(); ++it) {
        r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        m_colors[it->first] = vec3(r, g, b);
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
    for(auto it = m_faces.begin(); it != m_faces.end(); ++it) {
        v0 = m_vertices[it->second.v0];
        v1 = m_vertices[it->second.v1];
        v2 = m_vertices[it->second.v2];
        m_fnormals[it->first] = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }
}

void gnut::gfx::mesh::compute_vnormals() {
    if(m_vnormals.size() != m_vertices.size()) {
        m_vnormals.resize(m_vertices.size());
    }
    for(auto & p : m_vfadjacency) {
        this->compute_vnormal(p.first, p.second);
    }
}

void gnut::gfx::mesh::compute_vnormal(unsigned int vi, unordered_set<unsigned int> & neighbors) {
    vec3 a, b, vnormal;
    float total_weight = 0.f;
    unordered_map<unsigned int, float> weights;
    for(unsigned int neighbor : neighbors) {
        face face = m_faces[neighbor];
        if(vi == face.v0) {
            a = m_vertices[face.v1] - m_vertices[face.v0];
            b = m_vertices[face.v2] - m_vertices[face.v0];
        } else if(vi == face.v1) {
            a = m_vertices[face.v2] - m_vertices[face.v1];
            b = m_vertices[face.v0] - m_vertices[face.v1];
        } else {
            a = m_vertices[face.v0] - m_vertices[face.v2];
            b = m_vertices[face.v1] - m_vertices[face.v2];
        }
        weights[neighbor] = glm::acos(glm::dot(a, b) / (glm::length(a) * glm::length(b)));
        total_weight += weights[neighbor];
    }
    for(unsigned int neighbor : neighbors) {
        vnormal += m_fnormals[neighbor] * (weights[neighbor] / total_weight);
    }
    m_vnormals[vi] = normalize(vnormal);
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
    if(m_vfadjacency[face.v0].size() == 0) {
        m_vfadjacency.erase(face.v0);
    }
    if(m_vfadjacency[face.v1].size() == 0) {
        m_vfadjacency.erase(face.v1);
    }
    if(m_vfadjacency[face.v2].size() == 0) {
        m_vfadjacency.erase(face.v2);
    }
    m_faces.erase(f);
    m_fnormals.erase(f);
}


void gnut::gfx::mesh::edge_collapse(unsigned int vi0, unsigned int vi1) {
    assert(vi0 < m_vertices.size() && vi1 < m_vertices.size());

    vec3 v0 = m_vertices[vi0];
    vec3 v1 = m_vertices[vi1];
    vec3 vnew = (v0 + v1) / 2.f;

    unordered_set<unsigned int> adjacent_faces;
    unordered_set<unsigned int> removed_faces;
    unsigned int vinew = static_cast<unsigned int>(m_vertices.size());
    m_vertices.push_back(vnew);

    for(unsigned int neighbor : m_vfadjacency[vi0]) {
        adjacent_faces.insert(neighbor);
    }
    for(unsigned int neighbor : m_vfadjacency[vi1]) {
        adjacent_faces.insert(neighbor);
    }

    for(unsigned int face : adjacent_faces) {
        gfx::face f = m_faces[face];
        if(f.contains(vi0) && f.contains(vi1)) {
            this->remove_face(face);
            removed_faces.insert(face);
        } else {
            if(f.contains(vi1)) {
                f.replace(vi1, vinew);
            } else if(f.contains(vi0)) {
                f.replace(vi0, vinew);
            }
            m_faces[face] = f;
            vec3 a = m_vertices[f.v1] - m_vertices[f.v0];
            vec3 b = m_vertices[f.v2] - m_vertices[f.v0];
            m_fnormals[face] = glm::normalize(glm::cross(a, b));
        }
    }

    for(unsigned int face : adjacent_faces) {
        for(unsigned int f2 : adjacent_faces) {
            if(face != f2 && m_faces[face] == m_faces[f2]) {
                removed_faces.insert(face);
                removed_faces.insert(f2);
            }
        }
    }

    for(auto face : removed_faces) {
        this->remove_face(face);
        adjacent_faces.erase(face);
    }

    for(unsigned int neighbor : adjacent_faces) {
        m_vfadjacency[vinew].insert(neighbor);
    }

    m_vfadjacency.erase(vi0);
    m_vfadjacency.erase(vi1);

    for(unsigned int neighbor : adjacent_faces) {
        gfx::face & f = m_faces[neighbor];
        this->compute_vnormal(f.v0, m_vfadjacency[f.v0]);
        this->compute_vnormal(f.v1, m_vfadjacency[f.v1]);
        this->compute_vnormal(f.v2, m_vfadjacency[f.v2]);
    }
    this->compute_vnormal(vinew, m_vfadjacency[vinew]);
}

float gnut::gfx::mesh::max_vertice() {
    return m_maxvertice;
}

glm::vec3 gnut::gfx::mesh::max() {
    return m_max;
}

glm::vec3 gnut::gfx::mesh::min() {
    return m_min;
}

void gnut::gfx::mesh::compute_vquadrics() {
    for(int i = 0; i < m_vertices.size(); ++i) {
        quadric q;
        glm::vec3 vertex = m_vertices[i];
        unordered_set<unsigned int> & adjacent_faces = m_vfadjacency[i];
        for(auto it = adjacent_faces.begin(); it != adjacent_faces.end(); ++it) {
            glm::vec3 normal = m_fnormals[*it];
            float d = -(normal.x * vertex.x + normal.y * vertex.y + normal.z * vertex.z);
            q = q + this->compute_quadric(glm::vec4(normal.x, normal.y, normal.z, d));
        }
    }
    this->compute_pairs();
}

gnut::gfx::quadric gnut::gfx::mesh::compute_quadric(glm::vec4 abcd) {
    quadric q;
    q.m[0][0] = abcd.x * abcd.x;
    q.m[0][1] = abcd.x * abcd.y;
    q.m[0][2] = abcd.x * abcd.z;
    q.m[0][3] = abcd.x * abcd.w;

    q.m[1][0] = abcd.x * abcd.y;
    q.m[1][1] = abcd.y * abcd.y;
    q.m[1][2] = abcd.y * abcd.z;
    q.m[1][3] = abcd.y * abcd.w;

    q.m[2][0] = abcd.x * abcd.z;
    q.m[2][1] = abcd.y * abcd.z;
    q.m[2][2] = abcd.z * abcd.z;
    q.m[2][3] = abcd.z * abcd.w;

    q.m[3][0] = abcd.x * abcd.w;
    q.m[3][1] = abcd.y * abcd.w;
    q.m[3][2] = abcd.z * abcd.w;
    q.m[3][3] = abcd.w * abcd.w;
    return q;
}

void gnut::gfx::mesh::compute_pairs() {
    for(auto it = m_faces.begin(); it != m_faces.end(); ++it) {
        gfx::face & f = it->second;
        gfx::edge e0;
        unsigned int v0 = std::min(f.v0, f.v1);
        unsigned int v1 = std::max(f.v0, f.v1);
        uint64_t k0 = (static_cast<uint64_t>(v0) << 32) | v1;
        e0.v0 = v0;
        e0.v1 = v1;
        m_edges[k0] = e0;

        v0 = std::min(f.v1, f.v2);
        v1 = std::max(f.v1, f.v2);
        k0 = (static_cast<uint64_t>(v0) << 32) | v1;
        e0.v0 = v0;
        e0.v1 = v1;
        m_edges[k0] = e0;

        v0 = std::min(f.v0, f.v2);
        v1 = std::max(f.v0, f.v2);
        k0 = (static_cast<uint64_t>(v0) << 32) | v1;
        e0.v0 = v0;
        e0.v1 = v1;
        m_edges[k0] = e0;
    }
}