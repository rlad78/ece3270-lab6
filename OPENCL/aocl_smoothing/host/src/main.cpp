#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "CL/opencl.h"
#include "AOCL_Utils.h"

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

// Problem data.
int N; // problem size
unsigned int *output;
bool test;
unsigned short *inputs_a;
unsigned short *inputs_b;
unsigned int *c_output;

// Function prototypes
bool init_opencl();
void init_problem();
void run();
void cleanup();

// Entry point.
int main(int argc, char **argv) {
	test = false;

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
			sizeof(unsigned short) * N, NULL, &status);
	checkError(status, "Failed to create buffer for input");

	input_b_buf = clCreateBuffer(context, CL_MEM_READ_ONLY, 
			sizeof(unsigned short) * N, NULL, &status);
	checkError(status, "Failed to create buffer for input");

  	// Output buffer.
  	output_buf = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
        		sizeof(unsigned int) * N, NULL, &status);
  	checkError(status, "Failed to create buffer for output");

  	return true;
}

//Initialize data for the problem.
void init_problem() {

	int i;
	N = NUM_SHORTS;

	output = (unsigned int*) malloc(sizeof(int) * N);
	inputs_a = (unsigned short*) malloc(sizeof(short) * N);  
	inputs_b = (unsigned short*) malloc(sizeof(short) * N);
	c_output = (unsigned int*) malloc(sizeof(int) * N);
	
	for(i = 0; i < N; i++){
		inputs_a[i] = (unsigned short) rand();
		inputs_b[i] = (unsigned short) rand();
	}
	
}

void run() {
	int i, num_errors;
  	cl_int status;
	


  	// Launch the problem for each device.
  	cl_event kernel_event;
  	cl_event finish_event;
	cl_event write_event;

	status = clEnqueueWriteBuffer(queue, input_a_buf, CL_FALSE,
			0, sizeof(unsigned short) * N, inputs_a, 0, NULL, &write_event);
	checkError(status, "Failed to write input buffer");

	status = clEnqueueWriteBuffer(queue, input_b_buf, CL_FALSE,
			0, sizeof(unsigned short) * N, inputs_b, 0, NULL, &write_event);
	checkError(status, "Failed to write input buffer");
		

	// Set kernel arguments.
  	unsigned argi = 0;
 	status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_a_buf);
  	checkError(status, "Failed to set argument %d", argi - 1);
 	status = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &input_b_buf);
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
  	const size_t global_work_size = N;
	//printf("Launching for device %d (%d elements)\n", 0, global_work_size);


  	double start_time = getCurrentTimestamp();

  	status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
       		&global_work_size, NULL, 1, &write_event, &kernel_event);
  	checkError(status, "Failed to launch kernel");

	clWaitForEvents(num_devices, &kernel_event);
  
  	double end_time = getCurrentTimestamp();

  	// Read the result. This the final operation.
	status = clEnqueueReadBuffer(queue, output_buf, CL_FALSE,
       		0, sizeof(unsigned int)*N, output, 1, &kernel_event, &finish_event);


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
	for(i = 0; i < N; i++){
		c_output[i] = (unsigned int)inputs_a[i] * (unsigned int)inputs_b[i];
	}
	end_time = getCurrentTimestamp();
	
	int correct = 1;	
	// Verify host and device match
	for(i = 0; i < N; i++){
		if(c_output[i] != output[i]){
			num_errors++;
			correct = 0;

			if(num_errors < 6)
				printf("Output %d, %d * %d, is incorrect.\nExpected: %d\nResult: %d\n\n",
					i, inputs_a[i], inputs_b[i], c_output[i], output[i]);

		}
	}

	// Print if output is valid and host execution time.
	if(correct == 1)
		printf("\nOutput Valid\n\n");
	printf("\nC time: %0.3f ms\n", (end_time - start_time) * 1e3);

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

