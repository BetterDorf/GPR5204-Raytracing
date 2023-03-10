#pragma once
#include "rtweekend.hpp"

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() : Values{ 0,0,0 } {}
    vec3(const double e0, const double e1, const double e2) : Values{ e0, e1, e2 } {}

    [[nodiscard]] double x() const { return Values[0]; }
    [[nodiscard]] double y() const { return Values[1]; }
    [[nodiscard]] double z() const { return Values[2]; }

    vec3 operator-() const { return {-Values[0], -Values[1], -Values[2]}; }
    double operator[](const int i) const { return Values[i]; }
    double& operator[](const int i) { return Values[i]; }

    vec3& operator+=(const vec3& v) {
        Values[0] += v.Values[0];
        Values[1] += v.Values[1];
        Values[2] += v.Values[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        Values[0] *= t;
        Values[1] *= t;
        Values[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    [[nodiscard]] double length() const {
        return sqrt(length_squared());
    }

    [[nodiscard]] double length_squared() const {
        return Values[0] * Values[0] + Values[1] * Values[1] + Values[2] * Values[2];
    }

    // Return true if the vector is close to zero in all dimensions.
    [[nodiscard]] bool near_zero() const {
        constexpr auto smallValue = 1e-8;
        return (std::fabs(Values[0]) < smallValue) && (std::fabs(Values[1]) < smallValue) && (std::fabs(Values[2]) < smallValue);
    }

    static  vec3 random()
	{
        return { random_double(), random_double(), random_double() };
    }

    static vec3 random(const double min, const double max)
	{
        return { random_double(min, max), random_double(min, max), random_double(min, max) };
    }

public:
    double Values[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.Values[0] << ' ' << v.Values[1] << ' ' << v.Values[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return { u.Values[0] + v.Values[0], u.Values[1] + v.Values[1], u.Values[2] + v.Values[2] };
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return {u.Values[0] - v.Values[0], u.Values[1] - v.Values[1], u.Values[2] - v.Values[2]};
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return { u.Values[0] * v.Values[0], u.Values[1] * v.Values[1], u.Values[2] * v.Values[2] };
}

inline vec3 operator*(const double t, const vec3& v) {
    return {t * v.Values[0], t * v.Values[1], t * v.Values[2]};
}

inline vec3 operator*(const vec3& v, const double t) {
    return t * v;
}

inline vec3 operator/(const vec3 v, const double t) {
    return (1 / t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.Values[0] * v.Values[0]
        + u.Values[1] * v.Values[1]
        + u.Values[2] * v.Values[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return { u.Values[1] * v.Values[2] - u.Values[2] * v.Values[1],
        u.Values[2] * v.Values[0] - u.Values[0] * v.Values[2],
        u.Values[0] * v.Values[1] - u.Values[1] * v.Values[0] };
}

inline vec3 unit_vector(const vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, const double etai_over_etat) {
	const auto cos_theta = fmin(dot(-uv, n), 1.0);
	const vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	const vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}