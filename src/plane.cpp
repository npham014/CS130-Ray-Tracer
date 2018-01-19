#include "plane.h"
#include "ray.h"
#include <cfloat>


// Intersect with the half space defined by the plane.  The plane's normal
// points outside.  If the ray starts on the "inside" side of the plane, be sure
// to record a hit with t=0 as the first entry in hits.
bool Plane::
Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    double top = dot(this->normal, (this->x1 - ray.endpoint));
    double bot = dot(this->normal, ray.direction);
    if(bot) {
	double t1 = top/bot;
	Hit firstHit;
        firstHit.object = this;
	firstHit.t=t1;
	firstHit.ray_exiting = false;
	hits.push_back(firstHit);
	return true;
    }
    return false;
}

vec3 Plane::
Normal(const vec3& point) const
{
    return normal;
}
