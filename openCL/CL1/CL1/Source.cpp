#include<stdio.h>
#include<CL/cl.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

int main()
{
	int n, i, k = 0, t = 0;
	int m, *mat, *mat2;
	int r[5000], g[5000], b[5000];
	float res[5000];
	printf("\nEnter number of pixels:");
	fflush(stdout);
	scanf("%d",&n);
	printf("\nEnter the rgb values of pixels:");
	fflush(stdout);
	
	for (i = 0; i<n; i++) {
		scanf("%d", &r[i]);
	}

	for (i = 0; i<n; i++) {
		scanf("%d", &g[i]);
	}

	for (i = 0; i<n; i++) {
		scanf("%d", &b[i]);
	}
	FILE *f1;
	char *src_str;
	size_t s_size;

	f1 = fopen("grecal.cl", "r");

	if (f1 == NULL)
	{
		fprintf(stdout, "File not found: ");
		fflush(stdout);
		exit(0);
	}
	src_str = (char *)malloc(100000);
	s_size = fread(src_str, sizeof(char), 100000, f1);

	fclose(f1);

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices, ret_num_platform;

	cl_int status = clGetPlatformIDs(1, &platform_id, &ret_num_platform);
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &status);

	cl_command_queue cq = clCreateCommandQueue(context, device_id, NULL, &status);

	cl_mem r_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(int), NULL, &status);
	cl_mem g_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(int), NULL, &status);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(int), NULL, &status);
	cl_mem res_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, n*sizeof(float), NULL, &status);

	
	status = clEnqueueWriteBuffer(cq, r_mem_obj, CL_TRUE, 0, n*sizeof(int), r, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cq, g_mem_obj, CL_TRUE, 0, n * sizeof(int), g, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cq, b_mem_obj, CL_TRUE, 0, n * sizeof(int), b, 0, NULL, NULL);

	//status= clEnqueueWriteBuffer(cq,g_mem_obj,CL_TRUE,0,t*sizeof(int),ws,0,NULL,NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&src_str, (const size_t *)&s_size, &status);
	status = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	cl_kernel kernel1 = clCreateKernel(program, "revWord", &status);

	size_t global_item_size = n;
	size_t local_item_size = 1;

	status = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &r_mem_obj);
	status = clSetKernelArg(kernel1, 1, sizeof(cl_mem), &g_mem_obj);
	status = clSetKernelArg(kernel1, 2, sizeof(cl_mem), &b_mem_obj);
	status = clSetKernelArg(kernel1, 3, sizeof(cl_mem), &res_mem_obj);

	status = clEnqueueNDRangeKernel(cq, kernel1, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	status = clFinish(cq);

	status = clEnqueueReadBuffer(cq, res_mem_obj, CL_TRUE, 0, n*sizeof(float), res, 0, NULL, NULL);
	for (i = 0; i<n; i++) {
		//if (i % 4 == 0) printf("\n");
		printf("%.2f\t", res[i]);

	}
	printf("\n");


	status = clFlush(cq);
	status = clReleaseKernel(kernel1);
	status = clReleaseProgram(program);
	status = clReleaseMemObject(r_mem_obj);
	status = clReleaseMemObject(g_mem_obj);
	status = clReleaseMemObject(b_mem_obj);
	status = clReleaseMemObject(res_mem_obj);
	status = clReleaseCommandQueue(cq);
	status = clReleaseContext(context);

//	free(mat);
	int fds;
	scanf("%d",&fds);
	getch();
	return 0;
}