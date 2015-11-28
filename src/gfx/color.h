#ifndef GNUT_COLOR_H
#define GNUT_COLOR_H

#include <iostream>
#include <string.h>

#include "macros.h"

namespace gnut {
    namespace gfx {
        class color {
        public:

            color();
            color(float r, float g, float b);
            color(float r, float g, float b, float a);
            color(unsigned int hex);
            ~color();

            float* data();

            float& operator[](int);

            color interpolate(color &, float);

            static color red();
            static color blue();
            static color green();
            static color yellow();
            static color orange();
            static color purple();
            static color white();
            static color black();
            static color light_brown();

            static color random_pastel();
            static color dark_pastel();
            static color bright_pastel();
            static color dark_shade();

            static color ambient();
            static color diffuse();
            static color specular();

            static color ambient_material();
            static color diffuse_material();
            static color specular_material();
            static color emissive_material();

#if defined(__GNUC__) | defined(__clang__)
            struct {
                float r, g, b, a;
            }ALIGN_16;
#elif defined(_MSC_VER)
            ALIGN_16 struct {
                float r, g, b, a;
            };
#endif
        };
    }
}
#endif
