#include <stdlib.h>
#include "filter.h"
#include "ppm.h"

// private function prototypes
float matrix_convolude(float **m1, const float m2[5][5]);
void matrix_multiply_constant(float ***FM, float f);
float ** matrix_char2float(char **CM);
char ** matrix_float2char(float **FM);
char ** matrix_get_image_nibble(ppmimg *img, int x, int y, int color);
// char ** matrix_extract5(ppmimg *img, int x, int y, int color);
void matirx_store(char **CM, ppmimg *img, int x, int y, int color);
void matrix_free_CM(char **CM);
void matrix_free_FM(float **FM);
char value_float2char(float f);
char isInImage(ppmimg *img, int n, int m);
char findNearestPixel(ppmimg *img, int n, int m, int color);

// #################
// ## GLOBAL VARS ##
// #################

const float MASK_ARR[8][5][5] = {
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
	1/9.0,
	1/16.0,
	1/256.0
};

// ######################
// ## PUBLIC FUNCTIONS ##
// ######################

/* 
 */
void filter_image(ppmimg *img, enum filter ftr){
	int i,j,k;
	ppmimg *img_copy;
	float **f_matrix;
	char **c_matrix;

	img_copy = ppm_copy(img);

	for (i = 0; i < img->height; i++)
	{
		for (j = 0; j < img->width; j++)
		{
			for (k = 0; k < 3; k++)
			{
				c_matrix = matrix_get_image_nibble(img_copy,i,j,k);
				f_matrix = matrix_char2float(c_matrix);
				matrix_free_CM(c_matrix);
				img->data[i][j][k] = value_float2char(MOD_ARR[ftr]*matrix_convolude(f_matrix, MASK_ARR[ftr]));

			}
		}
	}

	ppm_free(img_copy);
}

// #######################
// ## PRIVATE FUNCTIONS ##
// #######################

/* 
 */
float matrix_convolude(float **m1, const float m2[5][5]){
	int i,j;
	float sum = 0;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			sum += m1[i][j] * m2[4-i][4-j];
		}
	}

	return sum;
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

/* [x,y] should be the CENTROID
 */
char ** matrix_get_image_nibble(ppmimg *img, int x, int y, int color){
	int i,j;
	char **c_out;

	c_out = (char**) malloc(sizeof(char*)*5);
	for (i = 0; i < 5; i++)
	{
		c_out[i] = (char*) malloc(sizeof(char)*5);
		for (j = 0; j < 5; j++)
		{
			if(isInImage(img,(x-2)+i,(y-2)+j) == 1)
			{
				c_out[i][j] = img->data[(x-2)+i][(y-2)+j][color];
			}
			else
			{
				c_out[i][j] = findNearestPixel(img,(x-2)+i,(y-2)+j,color);
			}
		}
	}

	return c_out;

}

/* 
 */
char isInImage(ppmimg *img, int n, int m){
	if (n >= 0 && n <= img->width-1 && m >= 0 && m <= img->height-1)
	{
		return 1;
	}
	return 0;
}

/* 
 */
char findNearestPixel(ppmimg *img, int n, int m, int color){
	// left side
	if (n < 0)
	{
		// left-top-corner
		if (m < 0)
		{
			return img->data[0][0][color];
		}
		// left-bottom-corner
		else if (m > img->height-1)
		{
			return img->data[img->height-1][0][color];
		}
		// left-side
		else
		{
			return img->data[m][0][color];
		}
	}
	// right side
	else if (n > img->width-1)
	{
		// right-top-corner
		if (m < 0)
		{
			return img->data[0][img->width-1][color];
		}
		// right-bottom-corner
		else if (m > img->height-1)
		{
			return img->data[img->height-1][img->width-1][color];
		}
		// right-side
		else
		{
			return img->data[m][img->width-1][color];
		}
	}
	// top
	else if (m < 0)
	{
		return img->data[0][n][color];
	}
	// bottom
	else
	{
		return img->data[img->height-1][n][color];
	}

}

/* 
 */
// char ** matrix_extract5(ppmimg *img, int x, int y, int color){
// 	int i,j;
// 	char **m_out;

// 	m_out = (char**) malloc(sizeof(char*)*5);
// 	for (i = 0; i < 5; i++)
// 	{
// 		m_out[i] = (char*) malloc(sizeof(char)*5);
// 		for (j = 0; j < 5; j++)
// 		{
// 			m_out[i][j] = img->data[x+i][y+j][color];
// 		}
// 	}

// 	return m_out;
// }

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
	int i;
	for (i = 0; i < 5; i++)
	{
		free(CM[i]);
	}
	free(CM);
}

/* 
 */
void matrix_free_FM(float **FM){
	int i;
	for (i = 0; i < 5; i++)
	{
		free(FM[i]);
	}
	free(FM);
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
