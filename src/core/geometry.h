#include "linmath.h"
#include <math.h>

typedef struct {
	vec3 point;
	vec3 vector;
} Ray;

typedef struct {
	vec3 point;
	vec3 normal;
} Plane;

typedef struct {
	vec3 center;
	float radius;
} Sphere;

static inline int sphere_intersects_ray(Sphere sphere, Ray ray);
static inline float distance_between(vec3 point, Ray ray);
static inline void ray_intersection_point(vec3 result, Ray ray, Plane plane);

static inline int sphere_intersects_ray(Sphere sphere, Ray ray) {
	if (distance_between(sphere.center, ray) < sphere.radius)
		return 1;
	return 0;
}

static inline float distance_between(vec3 point, Ray ray) {
	vec3 p1_to_point;
	vec3_sub(p1_to_point, point, ray.point);
	vec3 p2_to_point;
	vec3 translated_ray_point;
	vec3_add(translated_ray_point, ray.point, ray.vector);
	vec3_sub(p2_to_point, point, translated_ray_point);

	// The length of the cross product gives the area of an imaginary
	// parallelogram having the two vectors as sides. A parallelogram can be
	// thought of as consisting of two triangles, so this is the same as
	// twice the area of the triangle defined by the two vectors.
	// http://en.wikipedia.org/wiki/Cross_product#Geometric_meaning
	vec3 cross_product;
	vec3_mul_cross(cross_product, p1_to_point, p2_to_point);
	float area_of_triangle_times_two = vec3_len(cross_product);
	float length_of_base = vec3_len(ray.vector);

	// The area of a triangle is also equal to (base * height) / 2. In
	// other words, the height is equal to (area * 2) / base. The height
	// of this triangle is the distance from the point to the ray.
	float distance_from_point_to_ray = area_of_triangle_times_two / length_of_base;
	return distance_from_point_to_ray;
}

// http://en.wikipedia.org/wiki/Line-plane_intersection
// This also treats rays as if they were infinite. It will return a
// point full of NaNs if there is no intersection point.
static inline void ray_intersection_point(vec3 result, Ray ray, Plane plane) {
	vec3 ray_to_plane_vector;
	vec3_sub(ray_to_plane_vector, plane.point, ray.point);

	float scale_factor = vec3_mul_inner(ray_to_plane_vector, plane.normal)
					   / vec3_mul_inner(ray.vector, plane.normal);

	vec3 intersection_point;
	vec3 scaled_ray_vector;
	vec3_scale(scaled_ray_vector, ray.vector, scale_factor);
	vec3_add(intersection_point, ray.point, scaled_ray_vector);
	memcpy(result, intersection_point, sizeof(intersection_point));
}
