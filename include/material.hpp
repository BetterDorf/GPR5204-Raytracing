#pragma once

#include "ray.hpp"
#include "hittable.hpp"

class material {
public:
    // Returns true if the ray is scattered
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	explicit lambertian(const color& a) : Albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    color Albedo;
};

class metal : public material {
public:
    metal(const color& a, const double f) : Albedo(a), Fuzz(f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

public:
    color Albedo;
    double Fuzz;
};

class dielectric : public material {
public:
	explicit dielectric(const double index_of_refraction) : Refrac_index(index_of_refraction) {}

    bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override;

private:
    static double reflectance(double cosine, double ref_idx);
public:
    double Refrac_index;
};