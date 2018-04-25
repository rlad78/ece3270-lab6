#ifndef FILTER_H_
#define FILTER_H_ 

const char IDENTITY_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
	};

const char EDGE1_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,1,0,-1,0},
		{0,0,0,0,0},
		{0,-1,0,1,0},
		{0,0,0,0,0},
	};

const char EDGE2_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,1,-4,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
	};

const char EDGE3_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,-1,-1,-1,0},
		{0,-1,8,-1,0},
		{0,-1,-1,-1,0},
		{0,0,0,0,0},
	};


const char SHARPEN_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,0,-1,0,0},
		{0,-1,5,-1,0},
		{0,0,-1,0,0},
		{0,0,0,0,0},
	};

const char BOX_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,1,1,1,0},
		{0,1,1,1,0},
		{0,0,0,0,0},
	};

const char GAUSS1_ARR[5][5] =
	{
		{0,0,0,0,0},
		{0,1,2,1,0},
		{0,2,4,1,0},
		{0,1,2,1,0},
		{0,0,0,0,0},
	};

const char GAUSS2_ARR[5][5] =
	{
		{1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,36,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1},
	};

const char ***MASK_ARR[8] = {
	IDENTITY_ARR,
	EDGE1_ARR,
	EDGE2_ARR,
	EDGE3_ARR,
	SHARPEN_ARR,
	BOX_ARR,
	GAUSS1_ARR,
	GAUSS2_ARR
};

const float MOD_ARR[8] = {
	0,
	0,
	0,
	0,
	0,
	1/9,
	1/16,
	1/256
}

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

#endif
