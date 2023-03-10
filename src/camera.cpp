#include "camera.hpp"

camera::camera(const point3 lookfrom, const point3 lookat, const vec3 vup, const double vfov, const double aspect_ratio, const double aperture,
               const double focus_dist)
{
	const auto theta = degrees_to_radians(vfov);
	const auto h = tan(theta / 2);
	const auto viewport_height = 2.0 * h;
	const auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    // Calculate viewport given camera angle
    _w = unit_vector(lookfrom - lookat);
    _u = unit_vector(cross(vup, _w));
    _v = cross(_w, _u);

    _origin = lookfrom;

    _horizontal = focus_dist * viewport_width * _u;
    _vertical = focus_dist * viewport_height * _v;
    _lower_left_corner = _origin - _horizontal / 2 - _vertical / 2 - focus_dist * _w;

    _lens_radius = aperture / 2;
}

ray camera::get_ray(const double s, const double t) const
{
	const vec3 rd = _lens_radius * random_in_unit_disk();
	const vec3 offset = _u * rd.x() + _v * rd.y();

    return { _origin + offset,
    	_lower_left_corner + s * _horizontal + t * _vertical - _origin - offset };
}
