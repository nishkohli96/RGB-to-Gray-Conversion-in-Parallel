__kernel void red(__global int *a)
{
	int id = get_global_id(0);
	float r = a[id]*0.299;
	a[id] = r;
}
