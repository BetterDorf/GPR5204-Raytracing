#pragma once

#include "ray.hpp"
#include "vec3.hpp"

class camera {
public:
	/**
	 * \param vfov vertical field of view in degrees
	 */
    camera(point3 lookfrom, point3 lookat, vec3   vup, double vfov, double aspect_ratio, double aperture, double focus_dist);

    /**
     * \brief obtain a ray from the camera in the direction of a point in the viewport
     * \param s from 0 to 1, 0 being to the left of the viewport and 1 being the right
     * \param t from 0 to 1, 0 being the bottom of the viewport and 1 being the top
     * \return the ray from the camera
     */
	[[nodiscard]] ray get_ray(double s, double t) const;

private:
    point3 _origin;
    point3 _lower_left_corner;
    vec3 _horizontal;
    vec3 _vertical;
    vec3 _u, _v, _w;
    double _lens_radius;
};
