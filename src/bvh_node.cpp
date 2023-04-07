#include "bvh_node.hpp"

#include "world.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bvh_node::bvh_node(const world& world, const bool isLeaf, const int first, const int second) : _isLeaf(isLeaf), _world(world)
{
    if (!isLeaf)
    {
        _firstNodeIndex = first;
        _secondNodeIndex = second;

        _box = _world.Nodes[_firstNodeIndex].bounding_box();

        const aabb additionalBox = _world.Nodes[_secondNodeIndex].bounding_box();

        _box = aabb::surrounding_box(_box, additionalBox);
    }
    else
    {
        _firstSphereIndex = first;
        _secondSphereIndex = second;

        _box = _world.Spheres[_firstSphereIndex].bounding_box();

        const aabb additionalBox = _world.Spheres[_secondSphereIndex].bounding_box();

        _box = aabb::surrounding_box(_box, additionalBox);
    }
}

bool bvh_node::hit(const ray& r, const double t_min, const double t_max, hit_record& rec) const
{
    if (!_box.hit(r, t_min, t_max))
        return false;

    bool hit_left;
    bool hit_right;

    if (_isLeaf)
    {
        hit_left = _world.Spheres[_firstSphereIndex].hit(r, t_min, t_max, rec);
        hit_right = _world.Spheres[_secondSphereIndex].hit(r, t_min, hit_left ? rec.T : t_max, rec);
    }
    else
    {
        hit_left = _world.Nodes[_firstNodeIndex].hit(r, t_min, t_max, rec);
        hit_right = _world.Nodes[_secondNodeIndex].hit(r, t_min, hit_left ? rec.T : t_max, rec);
    }

    return hit_left || hit_right;
}
