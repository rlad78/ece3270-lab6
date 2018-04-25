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

const char GAUSS2_ARR[5][5] =
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

const char UNSHARP_ARR[5][5] =
	{
		{1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,-476,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1},
	};


#endif
