#include "color.h"

#define clampf(min,max,num) (num < min ? min : num > max ? max : num)


gnut::color::color() {
    m_u8bit = 0xFF;
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
}

gnut::color::color(float r, float g, float b) {
    m_u8bit = 0xFF;
    memset(static_cast<void*>(&r), 1.f, sizeof(float) * 4);
    a = 1.f;
}

gnut::color::color(float r, float g, float b, float a) {
    m_u8bit = 0xFF;
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

gnut::color::color(unsigned int hex) {
    m_u8bit = 0xFF;
    //construct a bit mask 0xff000000
    unsigned int mask = 0xff << 0x18;
    
    //Unpack each 8bit segment into a float, and normalize such that 255 ~= 1.0
    for(int i = 0; mask; ++i, mask >>= 0x8) {
        (&r)[i] = ((float) ((hex & mask) >> ((3 - i) * 0x8))) / m_u8bit;
    }
}

gnut::color::~color() {
    //Delete any dynamically allocated memory/objects here
}


float* gnut::color::ptr() {
    //Returns a pointer to the m_color array
    return &r;
}

float& gnut::color::operator [] (int i) {
    //Returns a reference to the specified element
    return (&r)[i];
}

gnut::color gnut::color::interpolate(gnut::color & c1, float t) {
    t = static_cast<float>(clampf(0.0, 1.0, t));
    return color((1.f - t) * r + t * c1[0],
                 (1.f - t) * g + t * c1[1],
                 (1.f - t) * b + t * c1[2],
                 (1.f - t) * a + t * c1[3]);
}

gnut::color gnut::color::red() {
    return color(0xff0000ff);
}

gnut::color gnut::color::blue() {
    return color(0x0000ffff);
}

gnut::color gnut::color::green() {
    return color(0x00ff00ff);
}

gnut::color gnut::color::yellow() {
    return color(0xffff00ff);
}

gnut::color gnut::color::orange() {
    return color(0xff8800ff);
}

gnut::color gnut::color::purple() {
    return color(0xff00ffff);
}

gnut::color gnut::color::white() {
    return color(0xffffffff);
}

gnut::color gnut::color::black() {
    return color(0x00000000);
}

gnut::color gnut::color::light_brown() {
    return color(0xFFCCAAFF);
}

gnut::color gnut::color::random_pastel() {
    unsigned int c =
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x50 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::color gnut::color::dark_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 128)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::color gnut::color::bright_pastel() {
    unsigned int c =
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x18) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x10) +
        ((0x10 + (static_cast<unsigned int>(rand()) % 200)) << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::color gnut::color::dark_shade() {
    unsigned int shade = 0x0 + (rand() % 80);
    
    unsigned int c =
        (shade << 0x18) +
        (shade << 0x10) +
        (shade << 0x08) +
        0xFF;
    
    return color(c);
}

gnut::color gnut::color::ambient() {
    return color(0x111111FF);
}

gnut::color gnut::color::diffuse() {
    return color(0xffffffFF);
}

gnut::color gnut::color::specular() {
    return color(0xffffffFF);
}

gnut::color gnut::color::ambient_material() {
    return color(0x434343FF);
}

gnut::color gnut::color::diffuse_material() {
    return color(0xbcbcbcFF);
}

gnut::color gnut::color::specular_material() {
    return color(0xffffffFF);
}

gnut::color gnut::color::emissive_material() {
    return color(0x000000FF);
}

