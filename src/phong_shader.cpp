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
    color = color_ambient * (world.ambient_color * world.ambient_intensity);
    
    vec3 tempDiff;

    // Move this to Cast_Ray
   /* if(is_exiting) { //If the ray point is exiting that means the view is inside the object? this means we have to turn it around
      same_side_normal *= -1; //Flip the vector around if it's facing the wrong way? 
      //Note:Not really sure why I'm even checking this if my normals are passed in properly. 
    }*/
    Ray lightRay;
    vec3 lightColor;
    for(unsigned int i = 0; i < world.lights.size(); ++i) {//Loop through lights
	//Calculate Diffuse and Add to color
	lightRay.direction = world.lights.at(i)->position - intersection_point;
	lightColor = world.lights.at(i)->Emitted_Light(lightRay);
	color += lightColor * color_diffuse * max(dot(same_side_normal,lightRay.direction.normalized()),0);

        //Calculate Specular and Add to color
       vec3 reflectDirection = (2 * dot(lightRay.direction.normalized(), same_side_normal) * (same_side_normal - lightRay.direction.normalized()));
       double specInt = max(dot(reflectDirection,ray.direction.normalized()), 0);
       specInt = pow(specInt, specular_power);
       color += (lightColor * color_specular * specInt);

    }

    
    return color;
}
