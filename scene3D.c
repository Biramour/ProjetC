#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "scene3D.h"
#define square(a) (a)*(a)

//VECTEURS
void coord3D_print(struct coord3D_s c){
	printf("<%g,%g,%g>",c.x,c.y,c.z);
}

vector3D_t vector3D(double x, double y, double z){
	vector3D_t c;
	c.x=x;
	c.y=y;
	c.z=z;
	return c;
}

vector3D_t vector3D_add(vector3D_t u, vector3D_t v){
	return vector3D(u.x+v.x,u.y+v.y,u.z+v.z);
}

vector3D_t vector3D_sub(vector3D_t u, vector3D_t v){
	return vector3D(u.x-v.x,u.y-v.y,u.z-v.z);
}

vector3D_t vector3D_scale(double a,vector3D_t u){
	return vector3D(u.x*a,u.y*a,u.z*a);
}

double vector3D_dot_product(vector3D_t u,vector3D_t v){
	return u.x*v.x+u.y*v.y+u.z*v.z;
}

vector3D_t vector3D_cross_product(vector3D_t u,vector3D_t v){
	return vector3D(u.y*v.z-u.z*v.y,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x);
}

double vector3D_norm2(vector3D_t u){
	return vector3D_dot_product(u,u);
}

double vector3D_norm(vector3D_t u){
	return sqrt(vector3D_norm2(u));
}

vector3D_t vector3D_normalize(vector3D_t u){
	return vector3D((u.x/vector3D_norm(u)),(u.y/vector3D_norm(u)),(u.z/vector3D_norm(u)));
}


//POINTS
point3D_t point3D(double x, double y, double z){
	point3D_t p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

point3D_t point3D_translate(point3D_t p, vector3D_t u){
	return point3D(p.x + u.x, p.y + u.y, p.z + u.z);
}

vector3D_t point3D_subtract(point3D_t p, point3D_t q){
	return vector3D(p.x - q.x, p.y - q.y, p.z - q.z);
}

//RAY
ray_t ray(point3D_t p,vector3D_t d){
	ray_t r;
	r.p=p;
	r.d=d;
	return r;
}

//SCENE 3D

//PRIMITIVES

//SPHERE
typedef struct{
	point3D_t o;
	double radius;
}sphere_t;

sphere_t sphere(point3D_t c,double r){
	sphere_t s;
	s.o = c;
	s.radius = r;
	return s;
}

sphere_t sphere_translate(sphere_t s,vector3D_t v){
	s.o = point3D_translate(s.o,v);
	return s;
}

bool sphere_intersect(sphere_t s,ray_t r,double* pl, point3D_t* pq, vector3D_t* pn){
	bool ret=false;
	double b = vector3D_dot_product(r.d,point3D_subtract(r.p,s.o));
	double c = vector3D_norm2(point3D_subtract(r.p,s.o)) - square(s.radius);
	if((square(b)-c)>=0){
		double l = -b - sqrt(square(b)-c);
		if(l>=0){
			point3D_t q = point3D_translate(r.p,vector3D_scale(l,r.d));
			vector3D_t n = vector3D_normalize(point3D_subtract(q,s.o));
			if(pl!=NULL) *pl = l;
			if(pq!=NULL) *pq = q;
			if(pn!=NULL) *pn = n;
			ret= true;
		}
	}
	return ret;
}

//PLANE

typedef struct{
	point3D_t o;
	vector3D_t n;
}plane_t;

plane_t plane(point3D_t o,vector3D_t n){
	plane_t p;
	p.o = o;
	p.n = vector3D_normalize(n);
	return p;
}

plane_t plane_translate(plane_t p, vector3D_t v){
	return plane(point3D_translate(p.o,v),p.n);
}

bool plane_intersect(plane_t p, ray_t r,double* pl, point3D_t* pq, vector3D_t* pn){
	bool ret = false;
	if(vector3D_dot_product(r.d,p.n)!= 0){
		double l=vector3D_dot_product(point3D_subtract(p.o,r.p),p.n)/vector3D_dot_product(r.d,p.n);
		if(l>=0){
			point3D_t q = point3D_translate(r.p,vector3D_scale(l,r.d));
			vector3D_t n = p.n;
			if(pl!=NULL) *pl = l;
			if(pq!=NULL) *pq = q;
			if(pn!=NULL) *pn = n;
			ret= true;
		}
	}
	return ret;
}

//PRIMITIVE

typedef struct{
	primitive_type_t type;
	union{
		sphere_t sphere;
		plane_t plane;
	};
	color_t color;
}primitive_t;

primitive_t sphere_primitive(sphere_t s,color_t c){
	primitive_t p;
	p.type=SPHERE;
	p.sphere=s;
	p.color=c;
	return p;
}

primitive_t plane_primitive(plane_t pl,color_t c){
	primitive_t p;
	p.type=PLANE;
	p.plane=pl;
	p.color=c;
	return p;
}

color_t primitive_get_color(primitive_t* p){
	return p->color;
}

void primitive_set_color(primitive_t* p, color_t c){
	p->color=c;
}

bool primitive_intersect(primitive_t* p,ray_t* r, double* pl, point3D_t* pq, vector3D_t* pn){
	if(p->type == SPHERE) return sphere_intersect(p->sphere,*r,pl,pq,pn);
	if(p->type == PLANE) return plane_intersect(p->plane,*r,pl,pq,pn);
	else exit(EXIT_FAILURE);
}


//LIGHT
typedef struct{
	color_t color;
	light_type_t type;
	union{
		point3D_t p;
		vector3D_t d;
	};
}light_t;

light_t ambient(color_t c){
	light_t l;
	l.color = c;
	l.type=AMBIENT;
	return l;
}

light_t omni(color_t c, point3D_t p){
	light_t l;
	l.color = c;
	l.type=OMNI;
	l.p=p;
	return l;
}

light_t uni(color_t c,vector3D_t d){
	light_t l;
	l.color=c;
	l.type=UNI;
	l.d = d;
	return l;
}

ray_t light_ray(light_t l, point3D_t q){
	switch(l.type){
		case AMBIENT:exit(EXIT_SUCCESS);break;
		case OMNI:return ray(q,point3D_subtract(l.p,q)); break;
		case UNI:return ray(q,l.d);break;
		default:exit(EXIT_SUCCESS);break;
	}
}

//SCENE 3D
struct scene3D_s{
	int p_length;
	int l_length;
	primitive_t* primitives;
	light_t* lights;
};

scene3D_t* scene3D_new(){
	scene3D_t* new = malloc(sizeof(scene3D_t*));
	new->p_length=0;
	new->l_length=0;
	new->primitives = malloc(0*sizeof(primitive_t));
	new->lights=malloc(0*sizeof(light_t));
	return new;
}

void scene3D_free(scene3D_t* s){
	free(s->primitives);
	free(s->lights);
	free(s);
}


//PRINT
void scene3D_primitives_print(scene3D_t* s){
	if(s->p_length==0){
		printf("no primitive\n");
	}
	primitive_t p;
	for(int i=0;i<s->p_length;i++){
		p=s->primitives[i];
		printf("(%d) : ",i);
	switch(p.type){
		case SPHERE:printf("Sphere of radius %G centred on ",p.sphere.radius);coord3D_print(p.sphere.o);printf(" with color ");color_print(p.color);break;
		case PLANE:printf("Plane of normal ");coord3D_print(p.plane.n);printf(" passing through ");coord3D_print(p.plane.o);printf(" with color ");color_print(p.color);break;
	}
	printf("\n");
}
}

void scene3D_primitive_print(scene3D_t* s, unsigned int primitiveID){
	primitive_t p = s->primitives[primitiveID];
	printf("(%d) : ",primitiveID);
	switch(p.type){
		case SPHERE:printf("Sphere of radius %f centred on ",p.sphere.radius);coord3D_print(p.sphere.o);printf(" with color ");color_print(p.color);break;
		case PLANE:printf("Plane of normal ");coord3D_print(p.plane.n);printf(" passing through ");coord3D_print(p.plane.o);printf(" with color ");color_print(p.color);break;
	}
	printf("\n");

}

void scene3D_lights_print(scene3D_t* s){
	light_t l;
	for(int i=0;i<s->l_length;i++){
		l=s->lights[i];
		printf("(%d) : ",i);
		switch(l.type){
			case AMBIENT:printf("Ambien light with color ");color_print(l.color);break;
			case OMNI:printf("Omnidirectional light centered on ");coord3D_print(l.p);printf(" with color ");color_print(l.color);break;
			case UNI:printf("Unidirectional light of direction ");coord3D_print(l.d);printf(" with color ");color_print(l.color);break;
		}
		printf("\n");
	}
}

void scene3D_light_print(scene3D_t* s,unsigned int lightID){
	light_t l = s->lights[lightID];
		printf("(%d) : ",lightID);
		switch(l.type){
			case AMBIENT:printf("Ambien light with color ");color_print(l.color);break;
			case OMNI:printf("Omnidirectional light centered on ");coord3D_print(l.p);printf(" with color ");color_print(l.color);break;
			case UNI:printf("Unidirectional light of direction ");coord3D_print(l.d);printf(" with color ");color_print(l.color);break;
		}
		printf("\n");
	}

//PRIMITIVES 3D
void scene3D_sphere_add(scene3D_t* s, color_t color, point3D_t c,double r){
	sphere_t sp = sphere(c,r);
	primitive_t p = sphere_primitive(sp,color);
	s->primitives = realloc(s->primitives,(s->p_length+1)*sizeof(primitive_t));
	s->primitives[s->p_length]=p;
	s->p_length=s->p_length+1;
}

void scene3D_plane_add(scene3D_t* s, color_t color, point3D_t o,vector3D_t n){
	plane_t pl = plane(o,n);
	primitive_t p = plane_primitive(pl,color);
	s->primitives = realloc(s->primitives,(s->p_length+1)*sizeof(primitive_t));
	s->primitives[s->p_length]=p;
	s->p_length=s->p_length+1;
}

primitive_type_t scene3D_primitive_type(scene3D_t* s, unsigned int primitiveId){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene",primitiveId);
		exit(EXIT_FAILURE);
	}
	else return s->primitives[primitiveId].type;
}

void scene3D_sphere_set_radius(scene3D_t* s, unsigned int primitiveId, double radius){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene\n",primitiveId);
		exit(EXIT_FAILURE);
	}
	else if(scene3D_primitive_type(s,primitiveId) == SPHERE){
		s->primitives[primitiveId].sphere.radius = radius; 
	}
	
}

void scene3D_plane_set_normal(scene3D_t* s, unsigned int primitiveId, vector3D_t n){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene",primitiveId);
		exit(EXIT_FAILURE);
	}
	else if(scene3D_primitive_type(s,primitiveId) == PLANE){
		s->primitives[primitiveId].plane.n=n; 
	}
}

void scene3D_primitive_set_color(scene3D_t* s, unsigned int primitiveId, color_t color){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene",primitiveId);
		exit(EXIT_FAILURE);
	}
	else s->primitives[primitiveId].color = color;
}

void scene3D_primitive_translate(scene3D_t* s, unsigned int primitiveId, vector3D_t v){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene",primitiveId);
		exit(EXIT_FAILURE);
	}
	else switch(scene3D_primitive_type(s,primitiveId)){
		case SPHERE:s->primitives[primitiveId].sphere = sphere_translate(s->primitives[primitiveId].sphere,v);break;
		case PLANE:s->primitives[primitiveId].plane= plane_translate(s->primitives[primitiveId].plane,v);break;
	}
}

void scene3D_primitive_delete(scene3D_t* s, unsigned int primitiveId){
	if((int)primitiveId> s->p_length){
		printf("Primitive (%d) is not in this 3D scene",primitiveId);
		exit(EXIT_FAILURE);
	}
	for(int i=(int)primitiveId;i<s->p_length-1;i++){
		s->primitives[i]=s->primitives[i+1];
	}
	s->primitives=realloc(s->primitives,(s->p_length-1)*sizeof(primitive_t));
	s->p_length=s->p_length-1;

}

void scene3D_ambient_add(scene3D_t* s, color_t color){
	light_t l = ambient(color);
	s->lights = realloc(s->lights,(s->l_length+1)*sizeof(light_t));
	s->lights[s->l_length]=l;
	s->l_length=s->l_length+1;
}

void scene3D_omni_add(scene3D_t* s, color_t color, point3D_t p){
	light_t l = omni(color,p);
	s->lights = realloc(s->lights,(s->l_length+1)*sizeof(light_t));
	s->lights[s->l_length]=l;
	s->l_length=s->l_length+1;
}

void scene3D_uni_add(scene3D_t* s, color_t color, vector3D_t d){
	light_t l = uni(color,d);
	s->lights = realloc(s->lights,(s->l_length+1)*sizeof(light_t));
	s->lights[s->l_length]=l;
	s->l_length=s->l_length+1;
}

light_type_t scene3D_light_type(scene3D_t* s, unsigned int lightId){
	if((int)lightId> s->l_length){
		printf("Light (%d) is not in this 3D scene",lightId);
		exit(EXIT_FAILURE);
	}
	else return s->lights[lightId].type;
}

void scene3D_light_set_color(scene3D_t* s, unsigned int lightId, color_t color){
	if((int)lightId> s->l_length){
		printf("Light (%d) is not in this 3D scene",lightId);
		exit(EXIT_FAILURE);
	}
	else s->lights[lightId].color = color;
}

void scene3D_omni_set_position(scene3D_t* s, unsigned int lightId, point3D_t p){
	if((int)lightId> s->l_length){
		printf("Light (%d) is not in this 3D scene",lightId);
		exit(EXIT_FAILURE);
	}
	else if(scene3D_light_type(s,lightId)==OMNI){
		s->lights[lightId].p=p;
	}
}

void scene3D_uni_set_direction(scene3D_t* s, unsigned int lightId, vector3D_t d){
	if((int)lightId> s->l_length){
		printf("Light (%d) is not in this 3D scene",lightId);
		exit(EXIT_FAILURE);
	}
	else if(scene3D_light_type(s,lightId)==UNI){
		s->lights[lightId].d=d;
	}
}

void scene3D_light_delete(scene3D_t* s, unsigned int lightId){
	if((int)lightId> s->l_length){
		printf("Light (%d) is not in this 3D scene",lightId);
		exit(EXIT_FAILURE);
	}
	for(int i=(int)lightId;i<s->l_length-1;i++){
		s->lights[i]=s->lights[i+1];
	}
	s->lights=realloc(s->lights,(s->l_length-1)*sizeof(light_t));
	s->l_length=s->l_length-1;
}

bool scene3D_intersect(scene3D_t* s, ray_t* r, double* ret_l, point3D_t* ret_q, vector3D_t* ret_n, color_t* ret_c){
	bool b=false;
	for(int i=0;i<s->p_length+1;i++){
		switch (s->primitives[i].type){
			case SPHERE: b=sphere_intersect(s->primitives[i].sphere,*r,ret_l,ret_q,ret_n);
			if(b==true) {return b;}break;
			case PLANE: b= plane_intersect(s->primitives[i].plane,*r,ret_l,ret_q,ret_n);
			if(b==true) {return b;}break;
		}
		if(b==false){
			return b;
		}
	}
	return b;
}

json_element_t* coord3D_to_json(struct coord3D_s c){
	json_element_t* e = json_object();
	json_object_set(e, "x", json_double(c.x));
	json_object_set(e, "y", json_double(c.y));
	json_object_set(e, "z", json_double(c.z));
	return e;
	json_free(e);
}

json_element_t* scene3D_to_json(scene3D_t* s){
	json_element_t* e = json_object();
	json_element_t* primitives = json_array();
	for(int i=0;i<s->p_length;i++){
		json_element_t* p = json_object();
		switch(s->primitives[i].type){
		case SPHERE:json_object_set(p,"type",json_string("SPHERE")); json_object_set(p,"color",color_to_json(s->primitives[i].color)); json_object_set(p,"radius",json_double(s->primitives[i].sphere.radius)); json_object_set(p,"center",coord3D_to_json(s->primitives[i].sphere.o)); break;
		case PLANE: json_object_set(p,"type",json_string("PLANE")); json_object_set(p,"color",color_to_json(s->primitives[i].color)); json_object_set(p,"normal",coord3D_to_json(s->primitives[i].plane.n)); json_object_set(p,"point",coord3D_to_json(s->primitives[i].plane.o));break;
		}
		json_free(p);
	}

	json_element_t* lights = json_array();
	for(int i=0;i<s->l_length;i++){
		json_element_t* l = json_object();
		switch(s->lights[i].type){
			case AMBIENT: json_object_set(l,"type",json_string("AMBIENT")); json_object_set(l,"color",color_to_json(s->lights[i].color));break;
			case OMNI: json_object_set(l,"type",json_string("OMNI")); json_object_set(l,"color",color_to_json(s->lights[i].color)); json_object_set(l,"position",coord3D_to_json(s->lights[i].p));break;
			case UNI: json_object_set(l,"type",json_string("UNI")); json_object_set(l,"color",color_to_json(s->lights[i].color)); json_object_set(l,"direction",coord3D_to_json(s->lights[i].d));break;
		}
		json_free(l);
	}

	json_object_set(e,"primitives",primitives);
	json_object_set(e,"lights",lights);
	return e;
	json_free(e);
}
/////////////////////////////////////////////////////////

/*int main() {
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
  color_t c=color(0.2,0.2,0.2);
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

 
  return EXIT_SUCCESS;
}*/