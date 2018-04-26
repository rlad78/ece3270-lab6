#ifndef FILTER_H_
#define FILTER_H_ 

#include "ppm.h"

// global vars
extern const char MASK_ARR[8][5][5];
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

#endif
