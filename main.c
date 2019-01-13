#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "json.h"
#include "color.h"
#include "scene3D.h"
#define prl { printf("\n"); }
 
int main() {
  point3D_t A = point3D(1.0,2.0,3.0);
  point3D_t B = point3D(1.0,4.0,9.0);
  vector3D_t u = point3D(3.0,2.0,1.0);
  vector3D_t v = point3D(6.0,7.0,8.0);
  coord3D_print(A); prl;
  coord3D_print(B); prl;
  coord3D_print(u); prl;
  coord3D_print(v); prl;
  coord3D_print(vector3D_add(u,v)); prl;
  coord3D_print(vector3D_sub(u,v)); prl;
  coord3D_print(vector3D_scale(0.5,v)); prl;
  printf("%g\n", vector3D_dot_product(u,v));
  coord3D_print(vector3D_cross_product(u,v)); prl;
  printf("%g\n", vector3D_norm2(u));
  printf("%g\n", vector3D_norm(u));
  coord3D_print(vector3D_normalize(u)); prl;
  printf("%g\n", vector3D_norm(vector3D_normalize(u)));
  coord3D_print(point3D_translate(B,point3D_subtract(B, A))); prl;
  ray_t r = ray(A, v);
  coord3D_print(r.p); prl;
  coord3D_print(r.d); prl;  
 
  scene3D_t* s = scene3D_new();
  scene3D_primitives_print(s);
  scene3D_sphere_add(s,color(1,1,1),point3D(0,0,0),1);
  scene3D_plane_add(s,color(1,1,1),point3D(0,0,0),vector3D(1,0,0));
  scene3D_plane_add(s,color(1,1,1),point3D(0,0,0),vector3D(1,0,0));
  scene3D_sphere_set_radius(s, 0, 3);
  scene3D_primitive_translate(s, 0, vector3D(10,0,0));
  scene3D_plane_set_normal(s, 2, vector3D(-2,0,0));
  scene3D_primitive_translate(s, 2, vector3D(9,0,0));
  scene3D_primitives_print(s);
  scene3D_primitive_delete(s,1);
  scene3D_primitives_print(s);
  scene3D_primitive_set_color(s, 0, color(0.2,0.2,0.2));
  scene3D_primitive_set_color(s, 1, color(0.5,0.5,0.5));
  scene3D_primitives_print(s);
 
  scene3D_lights_print(s);
  scene3D_ambient_add(s, color(0,0,0));
  scene3D_omni_add(s, color(0,0,0), point3D(0,0,0));
  scene3D_ambient_add(s, color(1,1,1));
  scene3D_uni_add(s, color(0,0,0), vector3D(1,0,0));
  scene3D_uni_set_direction(s,3,vector3D(10,20,30));
  scene3D_omni_set_position(s,1,point3D(10,20,30));
  scene3D_lights_print(s);
  scene3D_light_delete(s, 2);
  scene3D_lights_print(s);
  scene3D_light_set_color(s, 0, color(0.1,0.1,0.1));
  scene3D_light_set_color(s, 1, color(0.2,0.2,0.2));
  scene3D_light_set_color(s, 2, color(0.3,0.3,0.3));
  scene3D_lights_print(s);
 
  double l;
  vector3D_t n;
  point3D_t q;
  color_t c;
  r = ray(point3D(0,3.0*sqrt(2.0)/2.0,3.0*sqrt(2.0)/2.0), vector3D(0,1,1));
  if (scene3D_intersect(s,&r,&l,&q,&n,&c)) {
    printf("%g / ", l); coord3D_print(q); printf(" / "); coord3D_print(n); printf(" / "); color_print(c); printf("\n");
  } else
    printf("no intersection\n");
  r = ray(point3D(0,0,0), vector3D(1,0,0));
  if (scene3D_intersect(s,&r,&l,&q,&n,&c)) {
    printf("%g / ", l); coord3D_print(q); printf(" / "); coord3D_print(n); printf(" / "); color_print(c); printf("\n");
  } else
    printf("no intersection\n");
  r = ray(point3D(0,3.0*sqrt(2.0)/2.0,0.0), vector3D(1,0,0));
  if (scene3D_intersect(s,&r,&l,&q,&n,&c)) {
    printf("%g / ", l); coord3D_print(q); printf(" / "); coord3D_print(n); printf(" / "); color_print(c); printf("\n");
  } else
    printf("no intersection\n");
  r = ray(point3D(0,3.0*sqrt(2.0)/2.0,3.0*sqrt(2.0)/2.0), vector3D(1,0,0));
  if (scene3D_intersect(s,&r,&l,&q,&n,&c)) {
    printf("%g / ", l); coord3D_print(q); printf(" / "); coord3D_print(n); printf(" / "); color_print(c); printf("\n");
  } else
    printf("no intersection\n");
 
  json_element_t* e;
  json_put(stdout, e = scene3D_to_json(s)); prl;
  json_free(e);
  scene3D_free(s);
  s = scene3D_from_json(e = json_load("scene.json"));
  json_free(e);
  scene3D_primitives_print(s);
  scene3D_lights_print(s);
  scene3D_free(s);
 
  return EXIT_SUCCESS;
}