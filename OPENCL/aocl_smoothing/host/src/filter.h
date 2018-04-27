#ifndef FILTER_H_
#define FILTER_H_ 

#include "ppm.h"

// global vars
extern const float MASK_ARR[8][5][5];
extern const float MOD_ARR[8];

// declarations
enum filter
{
	IDENTITY,
	EDGE1,
	EDGE2,
	EDGE3,
	SHARPEN,
	BOX,
	GAUSS1,
	GAUSS2,
};

// functions
void filter_image(ppmimg *img, enum filter ftr);
char isInImage(ppmimg *img, int n, int m);;
char findNearestPixel(ppmimg *img, int n, int m, int color);

#endif
