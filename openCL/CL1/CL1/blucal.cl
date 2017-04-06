__kernel void blue(__global int *b)	
{
int id = get_global_id(0);
float r = b[id] * 0.114;
b[id] = r;
}


