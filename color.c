#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "color.h"



color_t color (double r, double g, double b){
	if(r<0 || r>1 || g<0 || g>1 || b<0 || b>1  ){
		exit(EXIT_FAILURE);
	}
	color_t c;
	c.r = r;
	c.g = g;
	c.b = b;
	return c;
}

color_t color_add (color_t c1, color_t c2){
	color_t c3;
	if(c1.r+c2.r>1){
		c3.r = 1;
	} else c3.r = c1.r + c2.r;
	if(c1.g+c2.g>1){
		c3.g = 1;
	} else c3.g = c1.g + c2.g;
	if(c1.b+c2.b>1){
		c3.b = 1;
	}else c3.b = c1.b + c2.b;
	return c3;
}

color_t color_sub(color_t c1, color_t c2){
	color_t c3;
	if(c1.r-c2.r<0){
		c3.r = 0;
	} else c3.r = c1.r - c2.r;
	if(c1.g-c2.g<0){
		c3.g = 0;
	} else c3.g = c1.g - c2.g;
	if(c1.b-c2.b<0){
		c3.b = 0;
	}else c3.b = c1.b - c2.b;
	return c3;
}

color_t color_mul(color_t c1, color_t c2){
	color_t c3;
	if(c1.r*c2.r<0)c3.r=0;
	if(c1.r*c2.r>1)c3.r=1;
	else c3.r = c1.r*c2.r;
	if(c1.g*c2.g<0)c3.g=0;
	if(c1.g*c2.g>1)c3.g=1;
	else c3.g = c1.g*c2.g;
	if(c1.b*c2.b<0)c3.b=0;
	if(c1.b*c2.b>1)c3.b=1;
	else c3.b = c1.b*c2.b;
	return c3;
}

color_t color_scale (double a, color_t c){
	color_t c3;
	if(c.r*a<0)c3.r=0;
	if(c.r*a>1)c3.r=1;
	else c3.r = c.r*a;
	if(c.g*a<0)c3.g=0;
	if(c.g*a>1)c3.g=1;
	else c3.g = c.g*a;
	if(c.b*a<0)c3.b=0;
	if(c.b*a>1)c3.b=1;
	else c3.b = c.b*a;
	return c3; 
}

void color_to_BGR(color_t c, unsigned char* buffer){
	double blue = c.b * 256;
	if(blue>255) blue = 255;
	if(blue<0) blue = 0;
	buffer[0] = (unsigned char)blue;

	double green = c.g * 256;
	if(green>255) green = 255;
	if(green<0) green = 0;
	buffer[1] = (unsigned char)green;

	double red = c.r * 256;
	if(red>255) red = 255;
	if(red<0) red = 0;
	buffer[2] = (unsigned char)red;
}


unsigned char color_to_graylevel(color_t c){
	unsigned char buffer[3];
	color_to_BGR(c,buffer);
	unsigned char sum = (buffer[0] + buffer[1] + buffer[2])/3.0;
	return sum;
}

bool color_to_BW(color_t c){
	if (color_to_graylevel(c) > 128.0)
		return true;
	else return false;
}

void color_print(color_t c){
	printf("<r=%g, g=%g, b=%g>",c.r,c.g,c.b);
}

json_element_t* color_to_json(color_t c){
	json_element_t* e = json_object();
	json_object_set(e, "b", json_double(c.b));
	json_object_set(e, "g", json_double(c.g));
	json_object_set(e, "r", json_double(c.r));
	return e;
	json_free(e);
}



color_t color_from_json(json_element_t* e){
	if(json_search_key(e,"r")==false || json_search_key(e,"b")==false || json_search_key(e,"g")==false){
		printf("Color specification is not respected in this json element");
		exit(EXIT_FAILURE);
	}else
	return color(json_as_double(json_object_get(e,"r")),json_as_double(json_object_get(e,"g")),json_as_double(json_object_get(e,"b")));
}

/*int main() {
  color_t c = color(0.123456,0.987654,0.192837);
 
  color_print(c); prl;
  color_print(color_add(c,c)); prl;
  color_print(color_sub(c,c)); prl;
  color_print(color_mul(c,c)); prl;
  color_print(color_scale(0.5,c)); prl;
 
  json_element_t* e = color_to_json(c);
  json_put(stdout, e); prl; 
  color_print(color_from_json(e));
  json_free(e); prl;
 
  unsigned char tab[3];
  color_to_BGR(c, tab);
  printf("0x%hhx, 0x%hhx, 0x%hhx\n", tab[0], tab[1], tab[2]);
  printf("0x%hhx\n", color_to_graylevel(c));
  printf("%d\n", color_to_BW(c));
 
  return EXIT_SUCCESS;
}*/