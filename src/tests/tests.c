#include <CL/cl.h>
#include <stdio.h>
#include <string.h>

#define MAX_SOURCE_SIZE (0x100000)

int main() {
    // Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // Get platform and device information
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for each vector
    cl_mem inbuffer = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(char) * 6, NULL, &ret);
    cl_mem outbuffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(char) * 32, NULL, &ret);

    // Copy the lists A and B to their respective memory buffers
    char input[] = "HELLO";
    ret = clEnqueueWriteBuffer(command_queue, inbuffer, CL_TRUE, 0, sizeof(char) * 6, input, 0, NULL, NULL);

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "hash_main", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inbuffer);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&outbuffer);

    // Execute the OpenCL kernel on the list
    size_t global_item_size = 1; // Process the entire lists
    size_t local_item_size = 1; // Divide work items into groups of 64
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

    // Read the memory buffer C on the device to the local variable C
    char output[32];
    ret = clEnqueueReadBuffer(command_queue, outbuffer, CL_TRUE, 0, sizeof(char) * 32, output, 0, NULL, NULL);

    // Display the result to the screen
    for(int i = 0; i < 32; i++) {
        printf("%02x", output[i]);
    }
    printf("\n");

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(inbuffer);
    ret = clReleaseMemObject(outbuffer);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
    free(source_str);
    return 0;
}