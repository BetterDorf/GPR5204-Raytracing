#include "material.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool lambertian::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
{
#ifdef TRACY_ENABLE
    ZoneScopedN("LambertianScatter");
#endif

	auto scatter_direction = rec.Normal + random_unit_vector();

	// Catch degenerate scatter direction
	if (scatter_direction.near_zero())
		scatter_direction = rec.Normal;

	scattered = ray(rec.P, scatter_direction);

	attenuation = Albedo;
	return true;
}

bool metal::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
{
#ifdef TRACY_ENABLE
    ZoneScopedN("MetalScatter");
#endif
	const vec3 reflected = reflect(unit_vector(r_in.direction()), rec.Normal);
	scattered = ray(rec.P, reflected + Fuzz * random_in_unit_sphere());
	attenuation = Albedo;
	return (dot(scattered.direction(), rec.Normal) > 0);
}

bool dielectric::scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const
{
#ifdef TRACY_ENABLE
    ZoneScopedN("DielectricScatter");
#endif
    attenuation = color(1.0, 1.0, 1.0);
    const double refraction_ratio = rec.Front_face ? (1.0 / Refrac_index) : Refrac_index;

    const vec3 unit_direction = unit_vector(r_in.direction());

    // Calculate if refraction happens
    const double cos_theta = fmin(dot(-unit_direction, rec.Normal), 1.0);
    const double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
        direction = reflect(unit_direction, rec.Normal);
    else
        direction = refract(unit_direction, rec.Normal, refraction_ratio);

    scattered = ray(rec.P, direction);

    const vec3 refracted = refract(unit_direction, rec.Normal, refraction_ratio);

    scattered = ray(rec.P, refracted);
    return true;
}

double dielectric::reflectance(const double cosine, const double ref_idx)
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}