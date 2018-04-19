
int bitpair_cl(int);

__kernel void bitpair_kernel(global short* restrict A, global short* restrict B, global int* restrict Product)

{
	int i = get_global_id(0);
	int temp = 0;
	temp = temp | A[i];
	temp = temp << 16;
	temp = temp | B[i];

	Product[i] = bitpair_cl(temp);
	
}
