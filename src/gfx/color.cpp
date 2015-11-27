#include "color.h"

#define clampf(min,max,num) (num < min ? min : num > max ? max : num)


gfx::color::color() {
    m_u8bit = 0xFF;
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
}

gfx::color::color(float r, float g, float b) {
    m_u8bit = 0xFF;
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
    a = 1.f;
}

gfx::color::color(float r, float g, float b, float a) {
    m_u8bit = 0xFF;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

gfx::color::color(unsigned int hex) {
    m_u8bit = 0xFF;
    //construct a bit mask 0xff000000
    unsigned int mask = 0xff << 0x18;
    
    //Unpack each 8bit segment into a float, and normalize such that 255 ~= 1.0
    for(int i = 0; mask; ++i, mask >>= 0x8) {
        (&r)[i] = ((float) ((hex & mask) >> ((3 - i) * 0x8))) / m_u8bit;
    }
}

gfx::color::~color() {
    //Delete any dynamically allocated memory/objects here
}


float* gfx::color::ptr() {
    //Returns a pointer to the m_color array
    return &r;
}

float& gfx::color::operator [] (int i) {
    //Returns a reference to the specified element
    return (&r)[i];
}

gfx::color gfx::color::interpolate(gfx::color & c1, float t) {
    t = static_cast<float>(clampf(0.0, 1.0, t));
    return color((1.f - t) * r + t * c1[0],
                 (1.f - t) * g + t * c1[1],
                 (1.f - t) * b + t * c1[2],
                 (1.f - t) * a + t * c1[3]);
}

gfx::color gfx::color::red() {
    return color(0xff0000ff);
}

gfx::color gfx::color::blue() {
    return color(0x0000ffff);
}

gfx::color gfx::color::green() {
    return color(0x00ff00ff);
}

gfx::color gfx::color::yellow() {
    return color(0xffff00ff);
}

gfx::color gfx::color::orange() {
    return color(0xff8800ff);
}

gfx::color gfx::color::purple() {
    return color(0xff00ffff);
}

gfx::color gfx::color::white() {
    return color(0xffffffff);
}

gfx::color gfx::color::black() {
    return color(0x00000000);
}

gfx::color gfx::color::light_brown() {
    return color(0xFFCCAAFF);
}

gfx::color gfx::color::random_pastel() {
    unsigned int c =
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gfx::color gfx::color::dark_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gfx::color gfx::color::bright_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x08) +
        0xFF;
    
    return color(c);
}

gfx::color gfx::color::dark_shade() {
    unsigned int shade = 0x0 + (rand() % 80);
    
    unsigned int c =
        (shade << 0x18) +
        (shade << 0x10) +
        (shade << 0x08) +
        0xFF;
    
    return color(c);
}

gfx::color gfx::color::ambient() {
    return color(0x111111FF);
}

gfx::color gfx::color::diffuse() {
    return color(0xffffffFF);
}

gfx::color gfx::color::specular() {
    return color(0xffffffFF);
}

gfx::color gfx::color::ambient_material() {
    return color(0x434343FF);
}

gfx::color gfx::color::diffuse_material() {
    return color(0xbcbcbcFF);
}

gfx::color gfx::color::specular_material() {
    return color(0xffffffFF);
}

gfx::color gfx::color::emissive_material() {
    return color(0x000000FF);
}

