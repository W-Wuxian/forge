# forge

- [X] install latest version of fftw form sources
- [X] install latest version of hadi/fwht
- [] choose optim functions from hadi fwht
- [] split GAUSS sketch from CFWHT FFTW and HADI_FWHT sketch functions

I need to looks for:

# FWHT

https://github.com/FALCONN-LIB/FFHT

https://github.com/hadipourh/fwht

## CORE API - SIMPLE INTERFACE
All transforms are in-place and use the standard butterfly algorithm.
> fwht_status_t fwht_f64(double* data, size_t n);

### VECTORIZED BATCH API - HIGH PERFORMANCE FOR SMALL TRANSFORMS
Process multiple independent transforms simultaneously using SIMD.
Performance gain:
- n ≤ 256:  3-5× faster than sequential (uses AVX2/NEON)
- n > 256:  1.2-1.5× faster (memory-bound, less SIMD benefit)
Memory layout:
data_array[0] points to first transform (n elements)
data_array[1] points to second transform (n elements)
  ...
data_array[batch_size-1] points to last transform
> fwht_status_t fwht_f64_batch(double** data_array, size_t n, size_t batch_size);

### CORE API - BACKEND CONTROL
> fwht_status_t fwht_f64_backend(double* data, size_t n, fwht_backend_t backend);

### ADVANCED API - OUT-OF-PLACE TRANSFORMS
User must free() the result:
> double*  fwht_compute_f64(const double* input, size_t n);
> double*  fwht_compute_f64_backend(const double* input, size_t n, fwht_backend_t backend);

## ADVANCED API - CONTEXT FOR REPEATED CALLS
For applications that compute many WHTs, creating a context amortizes
setup costs (thread pools, GPU memory allocation, etc.)

### Compute using context (more efficient for repeated calls)
Compute using context (more efficient for repeated calls)
> fwht_status_t fwht_transform_f64(fwht_context_t* ctx, double* data, size_t n);

### ADVANCED API - BATCH PROCESSING
Compute multiple WHTs in parallel (optimal for GPU).
Batch transform: compute WHT for multiple arrays in parallel.
batch_size - Number of arrays to process
> fwht_status_t fwht_batch_f64(fwht_context_t* ctx, double** data_array, size_t n, int batch_size);


# BENCH

https://github.com/llnl/caliper

https://github.com/sheredom/ubench.h

# SIMD

https://github.com/simd-everywhere/simde?tab=readme-ov-file