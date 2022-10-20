#pragma once
#include <stdint.h>
#include "cglm/call.h"

void gen_tex_slices(
    vec3 view_dir,
    uint32_t num_slices,
    float** vert_data,
    uint32_t* vert_size,
    uint32_t** index_data,
    uint32_t* index_size
);
