#ifndef COLOR_H
#define COLOR_H
#define COLOR_H_
#include <stdio.h>
#include "json.h"




typedef struct {
	double r,g,b;
}color_t;

color_t color (double r, double g, double b);
color_t color_add(color_t c1, color_t c2);
color_t color_sub(color_t c1, color_t c2);
color_t color_mul(color_t c1, color_t c2);
color_t color_scale(double a, color_t c);
void color_to_BGR(color_t c, unsigned char* buffer);
unsigned char color_to_graylevel(color_t c);
bool color_to_BW(color_t c);
void color_print(color_t c);
json_element_t* color_to_json(color_t c);
color_t color_from_json(json_element_t* e);

#endif //COLOR_H