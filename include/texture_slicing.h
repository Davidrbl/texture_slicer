#pragma once
#include <stdint.h>

#include <cglm/call.h>

#define MAX_VERTS_PER_SLICE 6

void gen_texture_slices(
    vec3 normal,
    uint32_t num_slices,
    float** vert_data,
    uint32_t* vert_size,
    uint32_t* slice_len
);
