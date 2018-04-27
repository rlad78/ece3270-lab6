#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"
#include "ppm.h"
#include "filter.h"

using namespace aocl_utils;

#define NUM_SHORTS 100000

// OpenCL runtime configuration
cl_platform_id platform = NULL;
unsigned num_devices = 0;
cl_device_id *device; 
cl_context context = NULL;
cl_command_queue queue; 
cl_program program = NULL;
cl_kernel kernel; 
cl_mem output_buf; 
cl_mem input_a_buf;
cl_mem input_b_buf;
cl_mem input_c_buf;
cl_mem input_k_buf;

// Problem data.
// int N; // problem size
// unsigned int *output;
bool test;
// unsigned short *inputs_a;
// unsigned short *inputs_b;
// unsigned int *c_output;
int X, Y;
ppmimg *img_in;
ppmimg *img_out;
const char *filename_out;
char *compressed_output;
// char *compressed_input;
char *compressed_R;
char *compressed_G;
char *compressed_B;
float *compressed_kernel_reversed;
float kernel_mult;
enum filter ftr;

// Function prototypes
bool init_opencl();
void init_problem();
void run();
void cleanup();

// Entry point.
int main(int argc, const char *argv[]) {
	test = false;

    if (argc < 3)
    {
        printf("FORMAT:\nsmoothing <input filename> <output filename> [filter]\n");
        printf("FILTER OPTIONS:\n(0)identity (1)edge1 (2)edge2 (3)edge3 (4)sharp (5)box (6)gauss1 (7)gauss2\n");
        return -1;
    }

    img_in = ppm_read(argv[1]);
    img_out = ppm_copy(img_in);
    filename_out = argv[2];
    ftr = (filter)atoi(argv[3]);

    X = img_in->width;
    Y = img_in->height;

  	// Initialize the problem data.
  	init_problem();

  	// Initialize OpenCL.
  	if(!init_opencl()) {
    		return -1;
  	}

  	// Run the kernel.
  	run();

  	// Free the resources allocated
  	cleanup();

  	return 0;
}

/////// HELPER FUNCTIONS ///////

// Initializes the OpenCL objects.
bool init_opencl() {
  	cl_int status;

  	//printf("Initializing OpenCL\n");

  	if(!setCwdToExeDir()) {
    		return false;
  	}

  	// Get the OpenCL platform.
  	platform = findPlatform("Altera");
  	if(platform == NULL) {
    		printf("ERROR: Unable to find Altera OpenCL platform.\n");
    		return false;
  	}

  	// Query the available OpenCL device.
  	device = getDevices(platform, CL_DEVICE_TYPE_ALL, &num_devices);
  	//printf("Platform: %s\n", getPlatformName(platform).c_str());
  	//printf("Using %d device(s)\n", num_devices);
  	//printf("  %s\n", getDeviceName(*device).c_str());

  	// Create the context.
  	context = clCreateContext(NULL, num_devices, device, NULL, NULL, &status);
  	checkError(status, "Failed to create context");

  	// Create the program for all device. Use the first device as the
  	// representative device (assuming all device are of the same type).
  	std::string binary_file = getBoardBinaryFile("smoothing_cl", *device);
  	//printf("Using AOCX: %s\n", binary_file.c_str());
  	program = createProgramFromBinary(context, binary_file.c_str(), device, num_devices);

  	// Build the program that was just created.
  	status = clBuildProgram(program, 0, NULL, "", NULL, NULL);
  	checkError(status, "Failed to build program");

  	// Command queue.
  	queue = clCreateCommandQueue(context, *device, CL_QUEUE_PROFILING_ENABLE, &status);
  	checkError(status, "Failed to create command queue");

  	// Kernel.
  	const char *kernel_name = "smoothing_kernel";
  	kernel = clCreateKernel(program, kernel_name, &status);
  	checkError(status, "Failed to create kernel");

	//Input buffer.
	input_a_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			sizeof(char) * (Y+4) * (X+4), NULL, &status);
	checkError(status, "Failed to create buffer for input");

  input_b_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, 
      sizeof(char) * (Y+4) * (X+4), NULL, &status);
  checkError(status, "Failed to create buffer for input");

  input_c_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, 
      sizeof(char) * (Y+4) * (X+4), NULL, &status);
  checkError(status, "Failed to create buffer for input");

  input_k_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, 
      sizeof(float) * 5 * 5, NULL, &status);
  checkError(status, "Failed to create buffer for input");


  	// Output buffer.
  	output_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
        		sizeof(char) * Y, NULL, &status);
  	checkError(status, "Failed to create buffer for output");

  	return true;
}

//Initialize data for the problem.
void init_problem() {

	// int i;
	// N = NUM_SHORTS;

	// output = (unsigned int*) malloc(sizeof(int) * N);
	// inputs_a = (unsigned short*) malloc(sizeof(short) * N);  
	// inputs_b = (unsigned short*) malloc(sizeof(short) * N);
	// c_output = (unsigned int*) malloc(sizeof(int) * N);
	
	// for(i = 0; i < N; i++){
	// 	inputs_a[i] = (unsigned short) rand();
	// 	inputs_b[i] = (unsigned short) rand();
	// }

  int i, j;

  compressed_output = (char*) malloc(sizeof(char) * X * Y * 3);
  compressed_R = (char*) malloc(sizeof(char) * (X+4) * (Y+4));
  compressed_G = (char*) malloc(sizeof(char) * (X+4) * (Y+4));
  compressed_B = (char*) malloc(sizeof(char) * (X+4) * (Y+4));
  compressed_kernel_reversed = (float*) malloc(sizeof(float) * 5 * 5);
  kernel_mult = MOD_ARR[ftr];
	
  for (i = 0; i < Y+4; i++)
  {
      for (j = 0; j < X+4; j++)
      {
          if(isInImage(img_in,(X-2)+j,(Y-2)+i) == 1)
          {
              compressed_R[i*(Y+4) + j] = img_in->data[i][j][0];
              compressed_G[i*(Y+4) + j] = img_in->data[i][j][1];
              compressed_B[i*(Y+4) + j] = img_in->data[i][j][2];
          }
          else
          {
              compressed_R[i*(Y+4) + j] = findNearestPixel(img_in,(X-2)+j,(Y-2)+i,0);
              compressed_G[i*(Y+4) + j] = findNearestPixel(img_in,(X-2)+j,(Y-2)+i,1);
              compressed_B[i*(Y+4) + j] = findNearestPixel(img_in,(X-2)+j,(Y-2)+i,2);
          }
      }
  }  

  for (i = 0; i < 5; i++)
  {
      for (j = 0; j < 5; j++)
      {
          compressed_kernel_reversed[(i*5)+j] = MASK_ARR[ftr][4-i][4-j];
      }
  }
}

void run() {
	int i,j,k, num_errors;
  	cl_int status;
	


  	// Launch the problem for each device.
  	cl_event kernel_event;
  	cl_event finish_event;
  	cl_event write_event;

	status = clEnqueueWriteBuffer(queue, input_a_buf, CL_FALSE,
			0, sizeof(char)*(X+4)*(Y+4), compressed_R, 0, NULL, &write_event);
	checkError(status, "Failed to write input buffer");

	status = clEnqueueWriteBuffer(queue, input_b_buf, CL_FALSE,
			0, sizeof(char)*(X+4)*(Y+4), compressed_G, 0, NULL, &write_event);
	checkError(status, "Failed to write input buffer");

  status = clEnqueueWriteBuffer(queue, input_c_buf, CL_FALSE,
      0, sizeof(char)*(X+4)*(Y+4), compressed_B, 0, NULL, &write_event);
  checkError(status, "Failed to write input buffer");

  status = clEnqueueWriteBuffer(queue, input_k_buf, CL_FALSE,
      0, sizeof(float)*5*5, compressed_kernel_reversed, 0, NULL, &write_event);
  checkError(status, "Failed to write input buffer");
		

	// Set kernel arguments.
  	unsigned argi = 0;
 	status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_a_buf);
  	checkError(status, "Failed to set argument %d", argi - 1);
 	status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_b_buf);
  	checkError(status, "Failed to set argument %d", argi - 1);
  status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_c_buf);
    checkError(status, "Failed to set argument %d", argi - 1);
  status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_k_buf);
    checkError(status, "Failed to set argument %d", argi - 1);
  status = clSetKernelArg(kernel, argi++, sizeof(float), &kernel_mult);
    checkError(status, "Failed to set argument %d", argi - 1);
  status = clSetKernelArg(kernel, argi++, sizeof(int), &X);
    checkError(status, "Failed to set argument %d", argi - 1);
  status = clSetKernelArg(kernel, argi++, sizeof(int), &Y);
    checkError(status, "Failed to set argument %d", argi - 1);
	status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &output_buf);
  	checkError(status, "Failed to set argument %d", argi - 1);
	
  	// Enqueue kernel.
	// Use a global work size corresponding to the number of elements to add
  	// for this device.
	// 
  	// We don't specify a local work size and let the runtime choose
	// (it'll choose to use one work-group with the same size as the global
 	// work-size).
	//
  	// Events are used to ensure that the kernel is not launched until
	// the writes to the input buffers have completed.
  	const size_t global_work_size = X*Y;
	//printf("Launching for device %d (%d elements)\n", 0, global_work_size);


  	double start_time = getCurrentTimestamp();

  	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
       		&global_work_size, NULL, 1, &write_event, &kernel_event);
  	checkError(status, "Failed to launch kernel");

	 clWaitForEvents(num_devices, &kernel_event);
  
  	double end_time = getCurrentTimestamp();

  	// Read the result. This the final operation.
	status = clEnqueueReadBuffer(queue, output_buf, CL_FALSE,
       		0, sizeof(char)*X*Y*3, compressed_output, 1, &kernel_event, &finish_event);


  	// Wait for all devices to finish.
	clWaitForEvents(num_devices, &finish_event);


	// Wall-clock time taken.
 	printf("\nTime: %0.3f ms\n", (end_time - start_time) * 1e3);

	// Get kernel times using the OpenCL event profiling API.
  	cl_ulong time_ns = getStartEndTime(kernel_event);
	printf("Kernel time (device 0): %0.3f ms\n", double(time_ns) * 1e-6);

	num_errors = 0;
	
	// Measure host execution time.
	start_time = getCurrentTimestamp();
	   filter_image(img_in,ftr);
	end_time = getCurrentTimestamp();
	
	int correct = 1;	
	// Verify host and device match
	for(i = 0; i < Y; i++){
		  for (j = 0; j < X; j++)
      {
          for (k = 0; k < 3; k++)
          {
              img_out->data[i][j][k] = compressed_output[(i*X)+(j*3)+k];
              if (img_out->data[i][j][k] != img_in->data[i][j][k])
              {
                correct = 0;
              }
          }
      }
	}

	// Print if output is valid and host execution time.
	if(correct == 1)
		printf("\nOutput Valid\n\n");
  else
    printf("\nOutput INVALID\n\n");
	printf("\nC time: %0.3f ms\n", (end_time - start_time) * 1e3);

  ppm_write(img_out,filename_out);

	// Release all events.
  	clReleaseEvent(kernel_event);
	clReleaseEvent(finish_event);

}

// Free the resources allocated during initialization
void cleanup() {
  	if(kernel) {
    		clReleaseKernel(kernel);
  	}
  	if(queue) {
      		clReleaseCommandQueue(queue);
  	}

  	if(output_buf) {
      		clReleaseMemObject(output_buf);
  	}
  

  	if(program) {
    		clReleaseProgram(program);
  	}
  	if(context) {
    		clReleaseContext(context);
  	}
}

