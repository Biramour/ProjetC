#ifndef __RAYTRACER_SCENE3D_H__
#define __RAYTRACER_SCENE3D_H__
#include "color.h"
#include "json.h"
#include <stdbool.h>
#include <stdio.h>

struct coord3D_s {
  double x;
  double y;
  double z;
};
 
typedef struct coord3D_s vector3D_t;
typedef struct coord3D_s point3D_t;
 
void coord3D_print(struct coord3D_s c);
json_element_t* coord3D_to_json(struct coord3D_s c);
 
vector3D_t vector3D(double x, double y, double z); 
vector3D_t vector3D_add(vector3D_t u, vector3D_t v);
vector3D_t vector3D_sub(vector3D_t u, vector3D_t v);
vector3D_t vector3D_scale(double a, vector3D_t u);
double vector3D_dot_product(vector3D_t u, vector3D_t v);
vector3D_t vector3D_cross_product(vector3D_t u, vector3D_t v);
double     vector3D_norm2(vector3D_t u);
double     vector3D_norm(vector3D_t u);
vector3D_t vector3D_normalize(vector3D_t u);
 
point3D_t  point3D(double x, double y, double z);
point3D_t  point3D_translate(point3D_t p, vector3D_t u);
vector3D_t point3D_subtract(point3D_t p, point3D_t q);
 
typedef struct {
  point3D_t p;
  vector3D_t d;
} ray_t;
 
ray_t ray(point3D_t p, vector3D_t d);
 
struct scene3D_s;
typedef struct scene3D_s scene3D_t;
 
typedef enum { SPHERE, PLANE } primitive_type_t;
typedef enum { AMBIENT, OMNI, UNI } light_type_t;
 
scene3D_t* scene3D_new();
void scene3D_free(scene3D_t* s);
 
void scene3D_primitives_print(scene3D_t* s);
void scene3D_primitive_print(scene3D_t* s, unsigned int primitiveId);
void scene3D_sphere_add(scene3D_t* s, color_t color, point3D_t c, double r);
void scene3D_plane_add(scene3D_t* s, color_t color, point3D_t o, vector3D_t n);
primitive_type_t scene3D_primitive_type(scene3D_t* s, unsigned int primitiveId);
void scene3D_sphere_set_radius(scene3D_t* s, unsigned int primitiveId, double radius);
void scene3D_plane_set_normal(scene3D_t* s, unsigned int primitiveId, vector3D_t n);
void scene3D_primitive_set_color(scene3D_t* s, unsigned int primitiveId, color_t color);
void scene3D_primitive_translate(scene3D_t* s, unsigned int primitiveId, vector3D_t v);
void scene3D_primitive_delete(scene3D_t* s, unsigned int primitiveId);
bool scene3D_intersect(scene3D_t* s, ray_t* r, double* ret_l, point3D_t* ret_q, vector3D_t* ret_n, color_t* ret_c);
 
void scene3D_lights_print(scene3D_t* s);
void scene3D_light_print(scene3D_t* s, unsigned int lightId);
void scene3D_ambient_add(scene3D_t* s, color_t color);
void scene3D_omni_add(scene3D_t* s, color_t color, point3D_t p);
void scene3D_uni_add(scene3D_t* s, color_t color, vector3D_t d);
light_type_t scene3D_light_type(scene3D_t* s, unsigned int lightId);
void scene3D_light_set_color(scene3D_t* s, unsigned int lightId, color_t color);
void scene3D_omni_set_position(scene3D_t* s, unsigned int lightId, point3D_t p);
void scene3D_uni_set_direction(scene3D_t* s, unsigned int lightId, vector3D_t d);
void scene3D_light_delete(scene3D_t* s, unsigned int lightId);
ray_t scene3D_light_ray(scene3D_t* s, unsigned int lightId, point3D_t q);
 
json_element_t* scene3D_to_json(scene3D_t* s);
scene3D_t* scene3D_from_json(json_element_t* e);
 
#endif