/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */

#ifndef GNUT_MATERIAL_H
#define GNUT_MATERIAL_H

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include "color.h"

namespace gnut {
    namespace gfx {
        typedef struct material {
            color ambient;
            color diffuse;
            color specular;
            color emissive;
            float shininess;
        } material;

        void apply(material mat);
    }
}

#endif
