#pragma once

// This file is kind of a catch-all, until we move it somewhere more principled/cleaner

#ifndef _COCL_H  // since pragma once doesnt work if two files have same name and content, but different location...
#define _COCL_H

#ifdef __CUDA_ARCH__
// #pragma message("cuda arch")
#endif

#ifdef __CUDACC__
// #pragma message("cudacc")
#endif

// #if defined(__CUDACC__) || defined(__CUDA_ARCH__)
// #pragma message("defining device and host")
// #ifdef __CUDACC__ // cu file?
#define __device__ __attribute__((device))
#define __host__ __attribute__((host))
// #else
// #define __device__
// #define __host__
// #endif

// #ifdef __CUDACC__ // cu file?
#ifdef __CUDA_ARCH__ // means device-side
// #pragma message("deviceside")
#define __devicehost__ __attribute__((device))
#define __DEVICESIDE__
#else
// #pragma message("hostside")
// #define __devicehost__ __attribute__((host))
#define __HOSTSIDE__
#define __devicehost__ __attribute__((device)) __attribute__((host))
#endif

#ifndef __CUDACC__  // normal cpp file
// remove these defs: these functions will be hostside by default, in a normal cpp file
#undef __devicehost__
#define __devicehost__ 
#undef __host__
#define __host__ 
#endif

// #else // cpp file, not cu file
// #define __HOSTSIDE__
// #define __devicehost__
// #endif

// #else
// #pragma message("stubbing device and host")
// #include <stdexcept>
// #define __host__
// #define __device__
// #endif

#include <stdexcept>
#include <cmath>
#include <random>

// #if defined(__CUDACC__) || defined(__CUDA_ARCH__)
// #define __device__ __attribute__((device))
// #define __host__ __attribute__((host))
// #else
// #include <stdexcept>
// #define __host__
// #define __device__
// #endif

// #define __host__ __attribute__((host))
#define __global__ __attribute__((global))
#define __constant__ __attribute__((constant))
#define __shared__ __attribute__((shared))
#define __align__(x) __attribute__((aligned(x)))

// #define __launch_bounds__(x) __attribute__((launch_bounds(x)))
// #define __launch_bounds__(x)
// #define __launch_bounds__(x, y)

// found this at https://gist.github.com/eliben/b014ac17cbe5a452803f
#define __launch_bounds__(...) __attribute__((launch_bounds(__VA_ARGS__)))

#include "cocl/cocl_memory.h"
#include "cocl/cocl_streams.h"
#include "cocl/cocl_context.h"
#include "cocl/cocl_device.h"
#include "cocl/cocl_error.h"
#include "cocl/cocl_properties.h"
#include "cocl/cocl_blas.h"
#include "cocl/cocl_kernellaunch.h"
#include "cocl/cocl_funcs.h"
#include "cocl/hostside_opencl_funcs.h"
// #include "cocl/EasyCL/EasyCL.h"
#include "cocl/vector_types.h"

#include <iostream>

__devicehost__ inline long long __double_as_longlong(double val) {
    return (long long)val;
}

__devicehost__ inline double __longlong_as_double(long long val) {
    return (double)val;
}

// __attribute__((device)) <template typename T> T atomicExch(volatile T *p, T val);
// __attribute__((host)) int atomic_xchg


// __attribute__((host)) <template typename T> T atomicExch(volatile T *p, T val) {
//     throw std::runtime_error("not implemented: atomicExch on host");
//     // std::atomic<T> global;
//     // return atomic::exchange(p, val)
// }

// __device__ unsigned long long atomicExch(volatile unsigned long long *p, unsigned long long val);

#ifdef __CUDA_ARCH__
__device__ unsigned long long atomicExch(volatile unsigned long long *p, unsigned long long val);
#else
__host__ inline unsigned long long atomicExch(volatile unsigned long long *p, unsigned long long val) {
    throw std::runtime_error("not implemented: atomicExch on host");
    // std::atomic<T> global;
    // return atomic::exchange(p, val)
}
#endif

// #define atomicExch atomic_xchg

// int cudaConfigureCall(int gridx, int blockx, long long shared = 0,  char * stream = 0);

// stuff for device
// struct float4 {
// // public:
//     float4();
//     float4(const float4 &second);
//     float x;
//     float y;
//     float z;
//     float w;
// };

#define clock64() 0

typedef unsigned int CUjit_option;

#define CU_JIT_ERROR_LOG_BUFFER_SIZE_BYTES 65536
#define CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES 65536

enum cujitenum {
    CU_JIT_LOG_VERBOSE = 50000,
    CU_JIT_INFO_LOG_BUFFER,
    CU_JIT_ERROR_LOG_BUFFER
};

#define CUDART_CB

// we'll need to hack this call later on, when we write out the OpenCL.  At least, this placeholder lets us compile to IR
// for now
// __device___ bool __isGlobal(void *ptr);

// ==========================================
#ifdef __CUDA_ARCH__  // deviceside?
namespace std {
__device__ void *malloc(size_t count);
__device__ void free(void *ptr);
} // namespace std

// used by util_ptx.cuh, warpAny etc
#endif // __CUDA_ARCH__ deviceside

typedef int64_t cudaTextureObject_t;
typedef int64_t TextureWord;
struct cudaChannelFormatDesc {
};
struct cudaTextureDesc {
};
// typedef int64_t cudaCreateChannelDesc;
struct cudaResourceDesc {
};
typedef int64_t cudaResourceTypeLinear;

template<typename T> cudaChannelFormatDesc cudaCreateChannelDesc();
template<typename T> TextureWord tex1Dfetch();
// end of thrust stuff ===============================


// end of thrust bits
// =====================

// bool __isGlobal(const void *ptr) {
//     return true;
// }

#endif // _COCL_H
