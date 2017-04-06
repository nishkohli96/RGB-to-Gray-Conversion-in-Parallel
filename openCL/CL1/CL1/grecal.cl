__kernel void revWord(__global int *r,__global int *g , __global int *b,__global float *res) 
{
	int id = get_global_id(0);
	res[id] = r[id]*0.299 + g[id]*0.587 + b[id]*0.114;
}