#include <stdlib.h>
#include "filter.h"
#include "ppm.h"

// private function prototypes
float ** matrix_convolude(float **m1, float **m2);
void matrix_multiply_constant(float ***FM, float f);
float ** matrix_char2float(char **CM);
char ** matrix_float2char(float **FM);
float ** matrix_edge_handling(char **CM);
char ** matrix_extract5(ppmimg *img, int x, int y, int color);
void matirx_store(char **CM, ppmimg *img, int x, int y, int color);
void matrix_free_CM(char **CM);
void matrix_free_FM(float **FM);
char value_float2char(float f);

// #################
// ## GLOBAL VARS ##
// #################

const char MASK_ARR[8][5][5] = {
//IDENTITY_ARR
	{
		{0,0,0,0,0},
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
		{0,0,0,0,0},
	},

//EDGE1_ARR
	{
		{0,0,0,0,0},
		{0,1,0,-1,0},
		{0,0,0,0,0},
		{0,-1,0,1,0},
		{0,0,0,0,0},
	},

//EDGE2_ARR
	{
		{0,0,0,0,0},
		{0,0,1,0,0},
		{0,1,-4,1,0},
		{0,0,1,0,0},
		{0,0,0,0,0},
	},

//EDGE3_ARR
	{
		{0,0,0,0,0},
		{0,-1,-1,-1,0},
		{0,-1,8,-1,0},
		{0,-1,-1,-1,0},
		{0,0,0,0,0},
	},


//SHARPEN_ARR
	{
		{0,0,0,0,0},
		{0,0,-1,0,0},
		{0,-1,5,-1,0},
		{0,0,-1,0,0},
		{0,0,0,0,0},
	},

//BOX_ARR
	{
		{0,0,0,0,0},
		{0,1,1,1,0},
		{0,1,1,1,0},
		{0,1,1,1,0},
		{0,0,0,0,0},
	},

//GAUSS1_ARR
	{
		{0,0,0,0,0},
		{0,1,2,1,0},
		{0,2,4,1,0},
		{0,1,2,1,0},
		{0,0,0,0,0},
	},

//GAUSS2_ARR
	{
		{1,4,6,4,1},
		{4,16,24,16,4},
		{6,24,36,24,6},
		{4,16,24,16,4},
		{1,4,6,4,1},
	}
};

const float MOD_ARR[8] = {
	1,
	1,
	1,
	1,
	1,
	1/9,
	1/16,
	1/256
};

// ######################
// ## PUBLIC FUNCTIONS ##
// ######################

/* 
 */
void filter_image(ppmimg *img, enum filter ftr){
	
}

// #######################
// ## PRIVATE FUNCTIONS ##
// #######################

/* 
 */
float ** matrix_convolude(float **m1, float **m2){
	
}

/* 
 */
void matrix_multiply_constant(float ***FM, float f){
	int i,j;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			*(FM)[i][j] *= f;
		}
	}
}

/* 
 */
float ** matrix_char2float(char **CM){
	int i,j;
	float **FM;

	FM = (float**) malloc(sizeof(float*)*5);
	for (i = 0; i < 5; i++)
	{
		FM[i] = (float*) malloc(sizeof(float)*5);
		for (j = 0; j < 5; j++)
		{
			FM[i][j] = (float)CM[i][j];
		}
	}

	return FM;
}

/* 
 */
char ** matrix_float2char(float **FM){
	int i,j;
	char **CM;

	CM = (char**) malloc(sizeof(char*)*5);
	for (i = 0; i < 5; i++)
	{
		CM[i] = (char*) malloc(sizeof(char)*5);
		for (j = 0; j < 5; j++)
		{
			CM[i][j] = value_float2char(FM[i][j]);
		}
	}
}

/* 
 */
float ** matrix_edge_handling(char **CM){
	
}

/* 
 */
char ** matrix_extract5(ppmimg *img, int x, int y, int color){
	int i,j;
	char **m_out;

	m_out = (char**) malloc(sizeof(char*)*5);
	for (i = 0; i < 5; i++)
	{
		m_out[i] = (char*) malloc(sizeof(char)*5);
		for (j = 0; j < 5; j++)
		{
			m_out[i][j] = img->data[x+i][y+j][color];
		}
	}

	return m_out;
}

/* 
 */
void matirx_store(char **CM, ppmimg *img, int x, int y, int color){
	int i,j;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			img->data[i+x][j+y][color] = CM[i][j];
		}
	}
}


/* 
 */
void matrix_free_CM(char **CM){
	
}

/* 
 */
void matrix_free_FM(float **FM){
	
}

/* 
 */
char value_float2char(float f){
	if (f >= 255)
	{
		return 255;
	}
	else if (f <= 0)
	{
		return 0;
	}
	else
	{
		return (int)f;
	}
}
