#include "sphere.h"
#include "ray.h"


// Determine if the ray intersects with the sphere
bool Sphere::Intersection(const Ray& ray, std::vector<Hit>& hits) const
{
    vec3 p = ray.endpoint - this->center;
    double b = dot(ray.direction*2, p);
    double c =  dot(p,p) - (this->radius * this->radius);
    double disc = (b * b) - (4 * c);
    if ( disc>0 ){
			double t1 = (-1 * b) - sqrt(disc);
			t1 = t1/2;
			double t2 = (-1 * b) + sqrt(disc);
			t2 = t2/2;
	
			Hit firstHit;
			Hit secHit;
			if(t2 > 0) {
				if(t1 > 0) {	    
					firstHit.object = this;
					firstHit.t = t1;
					firstHit.ray_exiting = false;
					hits.push_back(firstHit);
       
				}
				secHit.object = this;
				secHit.t = t2;
				secHit.ray_exiting = true;
				hits.push_back(secHit);
				return true;
			}
	    
   } 
    return false;
}

vec3 Sphere::Normal(const vec3& point) const
{
    vec3 normal;
    normal = point - center;
    normal.normalized();
    return normal;
}
