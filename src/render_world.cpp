#include <vector>
#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"


Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3),disable_fresnel_reflection(false),disable_fresnel_refraction(false)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find the closest object of intersection and return the object that was
// intersected.  Record the Hit structure in hit.  If no intersection occurred,
// return NULL.  Note that in the case of a Boolean, the object returned will be
// the Boolean, but the object stored in hit will be the underlying primitive.
// Any intersection with t<=small_t should be ignored.
Object* Render_World::Closest_Intersection(const Ray& ray,Hit& hit)
{
    Object* closestObject = 0;
    double min_t = 999999999999999999999999999999999.0;
    for(unsigned int i = 0; i < objects.size(); ++i) { //Loop through all objects in the world
	std::vector<Hit> tVals; //List of all hits for this object
	if(objects.at(i)->Intersection(ray,tVals)) { //If there was a hit:
            for(unsigned int j = 0; j < tVals.size(); ++j) { //Go through each hit
	        if(tVals.at(j).t < min_t) {// If this was the closest hit:
		    min_t = tVals.at(j).t;//Set this as the new val to be compared to
		    hit = tVals.at(j);// Change the referenced hit to this hit
		    closestObject = objects.at(i);//Set the current object to closest
	        }

	    }
	}
    }
    
    return closestObject;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    Ray ray;
    ray.endpoint = camera.position;
    ray.direction = (camera.World_Position(pixel_index) - camera.position).normalized();
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    Hit temp;
    Object* foundObject = Closest_Intersection(ray, temp);
    
    vec3 color;

    if(foundObject) {
	vec3 intersection = ray.Point(temp.t);
        vec3 normal = foundObject->Normal(intersection);
	if(temp.ray_exiting) {
		normal *= -1;
	}
	color = foundObject->material_shader->Shade_Surface(ray,intersection,normal,recursion_depth, temp.ray_exiting);
    }
    else {
	vec3 dummy;
	color = background_shader->Shade_Surface(ray, dummy, dummy, recursion_depth,false);
    }
    return color;
}
