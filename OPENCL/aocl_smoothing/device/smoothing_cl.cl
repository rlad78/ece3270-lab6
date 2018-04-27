char f2c(float f){
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

__kernel void smoothing_kernel(global char* restrict A, global char* restrict B, global char* restrict C, global float* restrict K, float k_mult, int X, int Y, global char* restrict Result)

{
	int i, a, b;
	float dot_R = 0, dot_G = 0, dot_B = 0;

	i = get_global_id(0);
	for (a = -2; a < 3; a++)
	{
		for (b = -2; b < 3; b++)
		{
			dot_R += ((float)A[(2+a+(i/X))*(Y+4) + 2+(i % X)+b] * K[(2+a)*5 + 2+b]);
			dot_G += ((float)B[(2+a+(i/X))*(Y+4) + 2+(i % X)+b] * K[(2+a)*5 + 2+b]);
			dot_B += ((float)C[(2+a+(i/X))*(Y+4) + 2+(i % X)+b] * K[(2+a)*5 + 2+b]);
		}
	}
	Result[i*3] = f2c(k_mult*dot_R);
	Result[(i*3)+1] = f2c(k_mult*dot_G);
	Result[(i*3)+2] = f2c(k_mult*dot_B);
}