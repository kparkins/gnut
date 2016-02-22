#ifndef GNUT_MESH_H
#define GNUT_MESH_H

#include <set>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/func_geometric.hpp>

using std::set;
using std::pair;
using std::vector;
using std::shared_ptr;
using std::unordered_map;
using std::unordered_set;

using namespace glm;

namespace gnut {
    namespace gfx {

        typedef struct face {
            face();
            face(unsigned int v0, unsigned int v1, unsigned int v2);
            ~face();

            bool contains(unsigned int v);
            void replace(unsigned int vold, unsigned int vnew);

            unsigned int v0;
            unsigned int v1;
            unsigned int v2;
        }face;

        class mesh {
        public:

            mesh();
            virtual ~mesh();

            void remove_face(unsigned int f);
            void edge_collapse(unsigned int vi0, unsigned int vi1);
            void generate_buffer();
            void generate_dbuffer();
            void generate_colors();
            void debug(bool on);
            void draw();

        protected:

            void compute_fnormals();
            void compute_vnormals();

            void compute_vfadjacency();

            vector<float> m_bufferdata;

            vector<vec3> m_vertices;
            vector<vec3> m_vnormals;
            vector<vec3> m_colors;

            vector<face> m_faces;
            vector<vec3> m_fnormals;

            unordered_map<unsigned int, set<unsigned int>> m_vfadjacency;

            GLuint m_vao;
            GLuint m_vbo;

            bool m_debug;

            friend class mesh_loader;

        };

        typedef shared_ptr<mesh> pmesh;
    }
}

#endif //GNUT_MESH_H
