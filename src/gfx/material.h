#ifndef GNUT_MATERIAL_H
#define GNUT_MATERIAL_H

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>

#include "color.h"

namespace gnut {

    typedef struct material {
        color ambient;
        color diffuse;
        color specular;
        color emissive;
        float shininess;
    } material;

    void apply(material mat);
}

#endif
