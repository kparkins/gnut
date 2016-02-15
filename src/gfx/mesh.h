#ifndef GNUT_MESH_H
#define GNUT_MESH_H

#include <set>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/func_geometric.hpp>

using std::set;
using std::pair;
using std::vector;
using std::shared_ptr;
using std::unordered_map;

using namespace glm;

namespace gnut {
    namespace gfx {

        class mesh {
        public:

            mesh();
            virtual ~mesh();

        protected:

            void compute_fnormals();
            void compute_vnormals();

            void compute_vfadjacency();

            vector<vec3> m_vertices;
            vector<vec3> m_vnormals;

            vector<vec3> m_faces;
            vector<vec3> m_fnormals;

            unordered_map<unsigned int, set<unsigned int>> m_vfadjacency;

            friend class mesh_loader;

        };

        typedef shared_ptr<mesh> pmesh;
    }
}

#endif //GNUT_MESH_H