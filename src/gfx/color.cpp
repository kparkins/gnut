#include "color.h"

#define clampf(min,max,num) (num < min ? min : num > max ? max : num)


gnut::gfx::color::color() {
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
}

gnut::gfx::color::color(float r, float g, float b) {
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
    a = 1.f;
}

gnut::gfx::color::color(float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

gnut::gfx::color::color(unsigned int hex) {
    unsigned int mask = 0xff << 0x18;
    
    for(int i = 0; mask; ++i, mask >>= 0x8) {
        (&r)[i] = ((float) ((hex & mask) >> ((3 - i) * 0x8))) / 0xFF;
    }
}

gnut::gfx::color::~color() {

}

float* gnut::gfx::color::data() {
    return &r;
}

float& gnut::gfx::color::operator [] (int i) {
    return (&r)[i];
}

gnut::gfx::color gnut::gfx::color::interpolate(color & c1, float t) {
    t = static_cast<float>(clampf(0.0, 1.0, t));
    return color((1.f - t) * r + t * c1[0],
                 (1.f - t) * g + t * c1[1],
                 (1.f - t) * b + t * c1[2],
                 (1.f - t) * a + t * c1[3]);
}

gnut::gfx::color gnut::gfx::color::red() {
    return color(0xff0000ff);
}

gnut::gfx::color gnut::gfx::color::blue() {
    return color(0x0000ffff);
}

gnut::gfx::color gnut::gfx::color::green() {
    return color(0x00ff00ff);
}

gnut::gfx::color gnut::gfx::color::yellow() {
    return color(0xffff00ff);
}

gnut::gfx::color gnut::gfx::color::orange() {
    return color(0xff8800ff);
}

gnut::gfx::color gnut::gfx::color::purple() {
    return color(0xff00ffff);
}

gnut::gfx::color gnut::gfx::color::white() {
    return color(0xffffffff);
}

gnut::gfx::color gnut::gfx::color::black() {
    return color(0x00000000);
}

gnut::gfx::color gnut::gfx::color::light_brown() {
    return color(0xFFCCAAFF);
}

gnut::gfx::color gnut::gfx::color::random_pastel() {
    unsigned int c =
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::gfx::color gnut::gfx::color::dark_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::gfx::color gnut::gfx::color::bright_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::gfx::color gnut::gfx::color::dark_shade() {
    unsigned int shade = 0x0 + (rand() % 80);
    
    unsigned int c =
        (shade << 0x18) +
        (shade << 0x10) +
        (shade << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::gfx::color gnut::gfx::color::ambient() {
    return color(0x111111FF);
}

gnut::gfx::color gnut::gfx::color::diffuse() {
    return color(0xffffffFF);
}

gnut::gfx::color gnut::gfx::color::specular() {
    return color(0xffffffFF);
}

gnut::gfx::color gnut::gfx::color::ambient_material() {
    return color(0x434343FF);
}

gnut::gfx::color gnut::gfx::color::diffuse_material() {
    return color(0xbcbcbcFF);
}

gnut::gfx::color gnut::gfx::color::specular_material() {
    return color(0xffffffFF);
}

gnut::gfx::color gnut::gfx::color::emissive_material() {
    return color(0x000000FF);
}

