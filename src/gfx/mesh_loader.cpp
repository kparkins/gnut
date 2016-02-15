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
        return nullptr;
    }
    // check header
    getline(file_stream, line);
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    assert(line == "off");

    // get number of vertices/faces/edges
    getline(file_stream, line);
    vector<string> values = gnut::split(line, ' ');
    if(values.size() != 3) {
        LOG_ERROR("Error parsing off file.");
        return nullptr;
    }

    size_t num_verts = stoi(values[0]);
    size_t num_faces = stoi(values[1]);

    // get vertices
    pmesh mesh = make_shared<gnut::gfx::mesh>();
    vector<float>& vertices = mesh->m_vertices;
    vector<int>& faces = mesh->m_faces;
    while(num_verts-- > 0 && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        vertices.push_back(stoi(values[0]));
        vertices.push_back(stoi(values[1]));
        vertices.push_back(stoi(values[2]));
    }

    size_t face_verts = 0;
    while(num_faces-- > 0 && getline(file_stream, line)) {
        values = gnut::split(line, ' ');
        face_verts = stoi(values[0]);
        if(face_verts != 3 && face_verts != 4 ) {
            LOG_ERROR("Error. Invalid number of vertices per face.");
            return nullptr;
        }
        faces.push_back(stoi(values[1]));
        faces.push_back(stoi(values[2]));
        faces.push_back(stoi(values[3]));
        if(face_verts == 4) {
            faces.push_back(stoi(values[3]));
            faces.push_back(stoi(values[4]));
            faces.push_back(stoi(values[1]));
        }
    }
    cout << mesh->m_vertices.size() << " " << mesh->m_faces.size() << endl;
    return mesh;
}
