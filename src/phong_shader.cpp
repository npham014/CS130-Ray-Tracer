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
	lightRay.direction = world.lights.at(i)->position - intersection_point;//Calculate Direction
	double sqrdLight = lightRay.direction.magnitude_squared();
	lightRay.direction = lightRay.direction.normalized();//Normalize
	lightRay.endpoint = world.lights.at(i)->position;//Get the position
	if(world.enable_shadows) {//Check if shadows are on
		Hit passToShadow;
		Ray intToLight;
		intToLight.endpoint = intersection_point;
		intToLight.direction = lightRay.direction;
		intToLight.direction = intToLight.direction.normalized();
		if(world.Closest_Intersection(intToLight, passToShadow)){//If there's something in the ray
			if(passToShadow.t < sqrt(sqrdLight)) {//If the object is closer than int_point
				
				continue; //skip adding diffuse and specular
			}
		}
	}
	//Calculate Diffuse and Add to color
	lightColor = world.lights.at(i)->Emitted_Light(lightRay);
	lightColor /= sqrdLight;
	double temp = std::max(dot(lightRay.direction, same_side_normal) , 0.0);
	color += (lightColor * color_diffuse * temp);
	
       //Calculate Specular and Add to color
       vec3 reflectDirection = (2 * dot(lightRay.direction, same_side_normal) * same_side_normal) - lightRay.direction;
       vec3 oppRayDir = ray.direction.normalized() * -1;
       double specInt = std::max(dot(reflectDirection,oppRayDir), 0.0);
       specInt = pow(specInt, specular_power);
       color += (lightColor * color_specular * specInt);
	
    }
 
    
    return color;
}
