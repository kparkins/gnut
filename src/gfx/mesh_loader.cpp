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
    if(values.size() != 3) {
        LOG_ERROR("Error. Missing vertex, face, or edge size information in OFF file " << file);
        return nullptr;
    }

    size_t num_verts = stoul(values[0]);
    size_t num_faces = stoul(values[1]);

    // get vertices
    pmesh mesh = make_shared<gnut::gfx::mesh>();
    vector<glm::vec3>& vertices = mesh->m_vertices;
    vector<glm::vec3>& faces = mesh->m_faces;
    while(num_verts-- > 0 && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        vertices.push_back(glm::vec3(stof(values[0]), stof(values[1]), stof(values[2])));
    }

    size_t face_verts = 0;
    while(num_faces-- > 0 && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        face_verts = stoul(values[0]);
        if(face_verts != 3) {
            LOG_ERROR("Error. Invalid number of vertices per face. " << file);
            return nullptr;
        }
        faces.push_back(glm::vec3(stoul(values[1]), stoul(values[2]), stoul(values[3])));
    }
    cout << mesh->m_vertices.size() << " " << mesh->m_faces.size() << endl;
    mesh->compute_fnormals();
    std::cout << mesh->m_fnormals.size() << std::endl;
    mesh->compute_vnormals();
    return mesh;
}

