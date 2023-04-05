#include "bvh_node.hpp"

bvh_node::bvh_node(hittable* first, hittable* second) : _first(first), _second(second)
{
	_box = _first->bounding_box();

	const aabb additionalBox = _second->bounding_box();

	_box = aabb::surrounding_box(_box, additionalBox);
}

bool bvh_node::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const
{
    if (!_box.hit(r, t_min, t_max))
        return false;

    const bool hit_left = _first->hit(r, t_min, t_max, rec);
    const bool hit_right = _second->hit(r, t_min, hit_left ? rec.T : t_max, rec);

    return hit_left || hit_right;
}
