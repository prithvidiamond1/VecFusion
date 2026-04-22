#include <stdbool.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <math.h>

        float s312(int iterations, int LEN_1D, float* a)
{
    float prod;

    for (int nl = 0; nl < 10*iterations; nl++) {
        prod = (float)1.;

        // Loop distribution - separate the reduction into independent chunks
        const int CHUNK_SIZE = 64;  // Cache-friendly chunk size
        float chunk_prod[CHUNK_SIZE];

        // Initialize chunk products
        for (int j = 0; j < CHUNK_SIZE; j++) {
            chunk_prod[j] = (float)1.;
        }

        // Process in chunks to break the long dependency chain
        int i = 0;
        for (; i + CHUNK_SIZE <= LEN_1D; i += CHUNK_SIZE) {
            // Process each chunk with independent reduction
            for (int j = 0; j < CHUNK_SIZE; j++) {
                chunk_prod[j] *= a[i + j];
            }
        }

        // Combine chunk products (preserving original order)
        for (int j = 0; j < CHUNK_SIZE; j++) {
            prod *= chunk_prod[j];
        }

        // Handle remaining elements sequentially
        for (; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    return prod;
}

        typedef float v8f32 __attribute__((__vector_size__(32), __aligned__(32)));

float vectorized_s312(int iterations, int LEN_1D, float* a)
{
    float prod;
    const int CHUNK_SIZE = 64;

    for (int nl = 0; nl < 10 * iterations; nl++) {
        prod = 1.0f;

        // Vectorized chunk processing
        v8f32 vec_prod[CHUNK_SIZE / 8];
        for (int j = 0; j < CHUNK_SIZE / 8; j++) {
            vec_prod[j] = (v8f32){1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
        }

        int i = 0;
        for (; i + CHUNK_SIZE <= LEN_1D; i += CHUNK_SIZE) {
            // Process 8 elements at a time within each chunk
            for (int j = 0; j < CHUNK_SIZE / 8; j++) {
                v8f32 vec_a = *(v8f32*)&a[i + j * 8];
                vec_prod[j] *= vec_a;
            }
        }

        // Combine vector products
        float chunk_prod[CHUNK_SIZE];
        for (int j = 0; j < CHUNK_SIZE / 8; j++) {
            float* fp = (float*)&vec_prod[j];
            for (int k = 0; k < 8; k++) {
                chunk_prod[j * 8 + k] = fp[k];
            }
        }

        // Final reduction of chunk products
        for (int j = 0; j < CHUNK_SIZE; j++) {
            prod *= chunk_prod[j];
        }

        // Scalar tail
        for (; i < LEN_1D; i++) {
            prod *= a[i];
        }
    }
    return prod;
}

        static uint32_t next_u32(uint32_t *state) {
            *state = (*state * 1664525u) + 1013904223u;
            return *state;
        }

        static void fill_i32(int *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = (int)(next_u32(state) % 2001u) - 1000;
            }
        }

        static void fill_f32(float *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((float)(next_u32(state) % 2001u) - 1000.0f) / 17.0f;
            }
        }

        static void fill_f64(double *buf, int n, uint32_t *state) {
            for (int i = 0; i < n; ++i) {
                buf[i] = ((double)(next_u32(state) % 2001u) - 1000.0) / 17.0;
            }
        }

        int main(void) {
            const int arr_len = 128;
            uint32_t seed = 7u;
            int iterations = 5; int LEN_1D = arr_len; float a_scalar[128]; float a_vector[128]; float ret_scalar; float ret_vector;

            for (int trial = 0; trial < 64; ++trial) {
                fill_f32(a_scalar, arr_len, &seed); memcpy(a_vector, a_scalar, sizeof(a_scalar));
                ret_scalar = s312(iterations, LEN_1D, a_scalar); ret_vector = vectorized_s312(iterations, LEN_1D, a_vector); if (fabsf(ret_scalar - ret_vector) > 1e-5f) {
    fprintf(stderr, "Return mismatch on trial %d\n", trial);
    return 2;
}
                for (int i = 0; i < arr_len; ++i) {
    if (fabsf((a_scalar[i]) - (a_vector[i])) > 1e-5f) {
        fprintf(stderr, "Mismatch in parameter a on trial %d at index %d\n", trial, i);
        return 2;
    }
}
            }

            printf("PASS trials=%d\n", 64);
            return 0;
        }
