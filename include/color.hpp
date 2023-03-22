#pragma once
#include "vec3.hpp"
#include <iostream>

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

inline void write_color(std::ostream& out, double r, double g, double b) {
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
