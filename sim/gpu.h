#ifndef GPU_H
#define GPU_H

#include <stddef.h>
#include <stdint.h>

// CUDA error types and constants
typedef enum {
  cudaSuccess = 0,
  cudaErrorInvalidDevice = 10,
  cudaErrorMemoryAllocation = 2,
  cudaErrorInvalidValue = 11,
  cudaErrorInitializationError = 3,
} cudaError_t;
// CUDA memory copy types
typedef enum {
  cudaMemcpyHostToHost = 0,
  cudaMemcpyHostToDevice = 1,
  cudaMemcpyDeviceToHost = 2,
  cudaMemcpyDeviceToDevice = 3,
  cudaMemcpyDefault = 4
} cudaMemcpyKind;
// GPU device properties structure
struct cudaDeviceProp {
  char name[256];
  size_t totalGlobalMem;
  int major;
  int minor;
  int multiProcessorCount;
  int maxThreadsPerBlock;
  int maxGridSize[3];
  int maxBlockDim[3];
  int memoryClockRate;
  int memoryBusWidth;
  int canMapHostMemory;
  int integrated; // if GPU shares memory with CPU vs having dedicated VRAM
};
// GPU context structure for tracking state
struct gpu_context {
  int current_device;
  int device_count;
  int initialized;
  struct cudaDeviceProp device_props[8]; // Support up to 8 GPUs in one context
};
// CUDA runtime API functions
cudaError_t cudaSetDevice(int device);
cudaError_t cudaGetDevice(int *device);
cudaError_t cudaGetDeviceCount(int *count);
cudaError_t cudaMalloc(void **devPtr, size_t size);
cudaError_t cudaFree(void *devPtr);
cudaError_t cudaMemcpy(void *dst, const void *src, size_t count,
                       cudaMemcpyKind kind);
cudaError_t cudaMemset(void *devPtr, int value, size_t count);
cudaError_t cudaDeviceSynchronize(void);
cudaError_t cudaGetLastError(void);
const char *cudaGetErrorString(cudaError_t error);
// GPU memory management helpers
cudaError_t gpu_malloc_and_register(void **gpu_ptr, size_t size, int gpu_id);
cudaError_t gpu_free_and_deregister(void *gpu_ptr, int gpu_id);
// GPU direct RDMA specific functions
int gpu_check_rdma_support(int gpu_id);
int gpu_init_for_rdma(void);
int gpu_cleanup_rdma(void);
// GPU direct RDMA context management
int gpu_init_context(struct gpu_context *ctx);
void gpu_cleanup_context(struct gpu_context *ctx);
int gpu_set_current_device(struct gpu_context *ctx, int device_id);
int gpu_get_device_info(struct gpu_context *ctx, int device_id,
                        struct cudaDeviceProp **prop);
// GPU memory utilities
size_t gpu_get_free_memory(int gpu_id);
size_t gpu_get_total_memory(int gpu_id);
int gpu_is_pointer_on_device(void *ptr, int *device_id);

#endif // GPU_H