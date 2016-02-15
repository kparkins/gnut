/**
 * Copyright Kyle Parkinson 2016. All rights reserved.
 */
#include "material.h"

void gnut::gfx::apply(material mat) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat.emissive.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat.specular.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat.diffuse.data());
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, mat.ambient.data());
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mat.shininess);
}



