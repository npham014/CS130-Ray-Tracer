#include "refractive_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Refractive_Shader::
Shade_Surface(const Ray& ray, const vec3& intersection_point,
        const vec3& same_side_normal, int recursion_depth,bool is_exiting) const
{
    //Hints: Use REFRACTIVE_INDICES::AIR for air refractive_index
    //       Use is_exiting to decide the refractive indices on the ray and transmission sides
    vec3 reflection_color;
    vec3 refraction_color;
    double reflectance_ratio=-1;
    if(!world.disable_fresnel_refraction)
    {
        //TODO (Test 27+): Compute the refraction_color:
        // - Check if it is total internal reflection. 
        //      If so update the reflectance_ratio for total internal refraction
        //
        //      else, follow the instructions below
        //
        //        (Test 28+): Update the reflectance_ratio 
        //
        //        (Test 27+): Cast the refraction ray and compute the refraction_color
        //
	double nair = 1.00;
	double ni,nr;
	if(is_exiting) {
		ni = refractive_index;
		nr = nair;
	}
	else {
		ni = nair;
		nr = refractive_index;
	}
	
	double x = pow( (ni/nr), 2);
	double cosi = dot(same_side_normal, ray.direction);
	double cosrsqr = 1 - x * (1 - pow(cosi, 2));
	if(cosrsqr < 0) { //check for total internal reflection
		reflectance_ratio = 1;
	}
	else {
		double cosr = sqrt(cosrsqr);
		vec3 r = ray.direction - (dot(ray.direction, same_side_normal) * same_side_normal);
		vec3 T = (ni/nr) * (r - cosr * same_side_normal);
		Ray refract_ray;
		refract_ray.endpoint = intersection_point;
		refract_ray.direction = T.normalized();
		refraction_color = world.Cast_Ray(refract_ray, recursion_depth + 1);
	}
		
    }

    if(!world.disable_fresnel_reflection){
        // - Cast Reflection Ray and get color
        Ray reflect_ray;
	reflect_ray .endpoint = intersection_point;
	vec3 r = ray.direction - (2 * dot(ray.direction, same_side_normal) * same_side_normal);
	reflect_ray.direction = r;
	
	reflection_color = world.Cast_Ray(reflect_ray, recursion_depth + 1);
	//this might be wrong; This might have needed to be the full calculation from reflect_shader
	//edit: test 26 seems to work so this might be right
    }

    Enforce_Refractance_Ratio(reflectance_ratio);
    vec3 color;
    color = reflectance_ratio * reflection_color + (1 - reflectance_ratio) * refraction_color;
    return color;
}

void Refractive_Shader::
Enforce_Refractance_Ratio(double& reflectance_ratio) const
{
    if(world.disable_fresnel_reflection) reflectance_ratio=0;
    else if(world.disable_fresnel_refraction) reflectance_ratio=1;
}

