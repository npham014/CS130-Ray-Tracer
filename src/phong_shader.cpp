#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"


vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& same_side_normal,int recursion_depth,bool is_exiting) const 
{
    vec3 color;

    //Calculate Ambient Color
    color = color_ambient * (world.ambient_color * world.ambient_intensity);
    

    Ray lightRay;
    vec3 lightColor;

   
    for(unsigned int i = 0; i < world.lights.size(); ++i) {//Loop through lights
	lightRay.direction = world.lights.at(i)->position - intersection_point;
	lightRay.endpoint = world.lights.at(i)->position;
	if(world.enable_shadows) {
		Hit passToShadow;
		world.Closest_Intersection(lightRay, passToShadow);
		vec3 zeroVec(0,0,0);
		//If the intersection isn't the same as the closest point to the light
		if(cross(intersection_point , lightRay.Point(passToShadow.t)) == zeroVec ) { 
			return color;//Return ambient light only
		}	
	}
	//Calculate Diffuse and Add to color
	lightColor = world.lights.at(i)->Emitted_Light(lightRay);
	double temp = std::max(dot(lightRay.direction.normalized(), same_side_normal) , 0.0);
	color += lightColor * color_diffuse * temp;

       //Calculate Specular and Add to color
       vec3 reflectDirection = (2 * dot(lightRay.direction.normalized(), same_side_normal) * (same_side_normal - lightRay.direction.normalized()));
       double specInt = std::max(dot(reflectDirection,ray.direction.normalized()), 0.0);
       specInt = pow(specInt, specular_power);
       color += (lightColor * color_specular * specInt);

    }
 
    
    return color;
}
