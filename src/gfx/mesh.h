#ifndef GNUT_MESH_H
#define GNUT_MESH_H

#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

namespace gnut {
    namespace gfx {

        class mesh {
        public:

            mesh();
            virtual ~mesh();

        protected:

            vector<float> m_vertices;
            vector<int> m_faces;

            friend class mesh_loader;
        };

        typedef shared_ptr<mesh> pmesh;
    }
}

#endif //GNUT_MESH_H
