#ifndef GNUT_MESH_LOADER_H
#define GNUT_MESH_LOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <unordered_map>

#include "mesh.h"
#include "logger.h"

using std::string;
using std::getline;
using std::ifstream;
using std::function;
using std::transform;
using std::make_shared;
using std::stringstream;
using std::unordered_map;

using namespace gnut::gfx;

namespace gnut {
    namespace gfx {

        typedef function<pmesh(const string&)> loader_function;

        class mesh_loader {
        public:

            static pmesh load(const string & file);
            static pmesh load_off(const string & file);

        private:

            static unordered_map<string, loader_function> m_loaders;

        };

    }
}

#endif //GNUT_MESH_LOADER_H
