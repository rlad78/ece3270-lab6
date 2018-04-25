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

const char UNSHARP_ARR[5][5] =
	{
		{1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,-476,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1},
	};

 const float IDENTITY_CONST = 0;
 const float EDGE1_CONST = 0;
 const float EDGE2_CONST = 0;
 const float EDGE3_CONST = 0;
 const float SHARPEN_CONST = 0;
 const float BOX_CONST = 1/9;
 const float GAUSS1_CONST = 1/16;
 const float GAUSS2_CONST = 1/256;
 const float UNSHARP_CONST = -1/256;

#endif
