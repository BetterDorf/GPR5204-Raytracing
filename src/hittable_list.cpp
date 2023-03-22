#include "hittable_list.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif

    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.T;
            rec = temp_rec;
        }
    }

    return hit_anything;
}
