#pragma once

#include "vec3.hpp"

class ray
{
public:
	ray() = default;
	ray(const point3& origin, const vec3& direction) : _origin(origin), _direction(direction) {}


	[[nodiscard]] point3 origin() const { return _origin; }
	[[nodiscard]] vec3 direction() const { return _direction; }

	[[nodiscard]] point3 at(const double t) const
	{
		return _origin + t * _direction;
	}

private:
	point3 _origin;
	vec3 _direction;
};