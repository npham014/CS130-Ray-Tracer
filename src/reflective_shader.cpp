#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const
{
    vec3 color;

    Ray reflect_ray;
    reflect_ray.endpoint = intersection_point;
    
    
    vec3 r = ray.direction - (2 * dot(ray.direction, same_side_normal) * same_side_normal);
    reflect_ray.direction = r;

    vec3 ref_color = world.Cast_Ray(reflect_ray,recursion_depth+1);

    vec3 local_color = shader->Shade_Surface(ray, intersection_point, same_side_normal,recursion_depth,is_exiting);

    color = reflectivity * ref_color + (1 - reflectivity) * local_color;
    
    
    return color;
}
