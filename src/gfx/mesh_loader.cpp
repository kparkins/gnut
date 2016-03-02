//
// Created by Kyle J Parkinson on 2/11/16.
//

#include "mesh_loader.h"

unordered_map<string, loader_function> mesh_loader::m_loaders = {
        {"off", mesh_loader::load_off}
};

gnut::gfx::pmesh gnut::gfx::mesh_loader::load(const string & file) {
    size_t index = file.find_last_of(".");
    string file_format = file.substr(index + 1);
    transform(file_format.begin(), file_format.end(), file_format.begin(), ::tolower);
    if(m_loaders.find(file_format) != m_loaders.end()) {
        return m_loaders[file_format](file);
    }
    return nullptr;
}

gnut::gfx::pmesh gnut::gfx::mesh_loader::load_off(const string & file) {
    string line;
    ifstream file_stream(file);

    if(!file_stream.is_open() || !file_stream.good()) {
        LOG_ERROR("Error. Problem opening file -- " << file);
        return nullptr;
    }
    // check header
    getline(file_stream, line);
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    if(line != "off") {
        LOG_ERROR("Error. File header did not match expected for OFF file for " << file);
        return nullptr;
    }

    // get number of vertices/faces/edges
    getline(file_stream, line);
    vector<string> values = gnut::split(line, ' ');
    if(values.size() < 2) {
        LOG_ERROR("Error. Missing vertex, face, or edge size information in OFF file " << file);
        return nullptr;
    }

    size_t num_verts = stoul(values[0]);
    size_t num_faces = stoul(values[1]);

    // get vertices
    pmesh mesh = make_shared<gnut::gfx::mesh>();
    vector<glm::vec3>& vertices = mesh->m_vertices;
    unordered_map<unsigned int, face>& faces = mesh->m_faces;
    while(num_verts-- > 0 && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        float v0 = stof(values[0]);
        float v1 = stof(values[1]);
        float v2 = stof(values[2]);
        glm::vec3 & min = mesh->m_min;
        glm::vec3 & max = mesh->m_max;
        min.x = std::min(min.x, v0);
        max.x = std::max(max.x, v0);
        min.y = std::min(min.y, v1);
        max.y = std::max(max.y, v1);
        min.z = std::min(min.z, v2);
        max.z = std::max(max.z, v2);

        float tmax = std::max(std::abs(v0), std::max(std::abs(v1), std::abs(v2)));
        if(tmax > mesh->m_maxvertice) {
            mesh->m_maxvertice = tmax;
        }
        vertices.push_back(glm::vec3(v0, v1, v2));
    }

    int i = 0;
    size_t face_verts = 0;
    while(i < num_faces && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        face_verts = stoul(values[0]);
        if(face_verts != 3) {
            LOG_ERROR("Error. Invalid number of vertices per face. " << file);
            return nullptr;
        }
        faces[i++] = face(stoul(values[1]), stoul(values[2]), stoul(values[3]));
    }

    mesh->compute_vfadjacency();
    mesh->compute_fnormals();
    mesh->compute_vnormals();
    mesh->generate_colors();

    return mesh;
}

