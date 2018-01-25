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
    color += color_ambient + (world.ambient_color * world.ambient_intensity);
    
    vec3 tempDiff;
    if(is_exiting) { //If the ray point is exiting that means the view is inside the object? this means we have to turn it around
      same_side_normal *= -1; //Flip the vector around if it's facing the wrong way? 
      //Note:Not really sure why I'm even checking this if my normals are passed in properly. 
    }
    for(unsigned int i = 0; i < world.
    return color;
}
