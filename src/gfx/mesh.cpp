#include "mesh.h"

gnut::gfx::mesh::mesh() {}

gnut::gfx::mesh::~mesh() {}

void gnut::gfx::mesh::compute_fnormals() {
    vec3 v0, v1, v2;
    for(size_t i = 0; i < m_faces.size(); ++i) {
        v0 = m_vertices[m_faces[i].x];
        v1 = m_vertices[m_faces[i].y];
        v2 = m_vertices[m_faces[i].z];
        vec3 normal = normalize(cross(v1 - v0, v2 - v0));
        m_fnormals.push_back(normal);
    }
}

void gnut::gfx::mesh::compute_vnormals() {
    vec3 a, b, face;
    vec3 vnormal(0, 0, 0);
    vector<float> weights;
    float total_weight = 0.f;
    m_vnormals.resize(m_vertices.size());
    for(auto & p : m_vfadjacency) {
        for(unsigned int neighbor : p.second) {
            face = m_faces[neighbor];

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
            weights.push_back(glm::acos(glm::dot(a, b) / (glm::length(a) * glm::length(b))));
            total_weight += weights[weights.size() - 1];
        }
        unsigned int i = 0;
        for(unsigned int neighbor : p.second) {
            vnormal += m_fnormals[neighbor] * (weights[i++] / total_weight);
            m_vnormals[p.first] = vnormal;
        }
    }
}

void gnut::gfx::mesh::compute_vfadjacency() {
    for(size_t i = 0; i < m_faces.size(); ++i) {
        m_vfadjacency[m_faces[i].x].insert(i);
        m_vfadjacency[m_faces[i].y].insert(i);
        m_vfadjacency[m_faces[i].z].insert(i);
    }
}


