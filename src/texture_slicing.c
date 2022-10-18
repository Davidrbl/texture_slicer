#include <stdint.h>
#include "cglm/call.h"

void gen_tex_slices(
    vec3 view_dir,
    uint32_t num_slices,
    float** vert_data,
    uint32_t* vert_size,
    uint32_t** index_data,
    uint32_t* index_size
)
{
    vec3 cube_verts[8] = {
                            // xyz
        {-1.0, -1.0, -1.0}, // ---
        {-1.0, -1.0, +1.0}, // --+
        {-1.0, +1.0, +1.0}, // -++
        {-1.0, +1.0, -1.0}, // -+-
        {+1.0, -1.0, -1.0}, // +--
        {+1.0, -1.0, +1.0}, // +-+
        {+1.0, +1.0, +1.0}, // +++
        {+1.0, +1.0, -1.0}  // ++-
    };

    uint8_t cube_edges[12][2] = {
        {0, 1}, // 
        {1, 2}, // 
        {2, 3}, // 
        {3, 0}, // 

        {4, 5}, // 
        {5, 6}, // 
        {6, 7}, // 
        {7, 4}, // 

        {0, 4}, // 
        {1, 5}, // 
        {2, 6}, // 
        {3, 7}, // 
    };

    uint8_t num_intersections = 0;

    bool intersections[12] = { false };
    float t_intersection[12] = { 0 };

    for (uint8_t i = 0; i < 12; i++)
    {
        uint8_t Vi_index = cube_edges[i][0];
        uint8_t Vj_index = cube_edges[i][1];

        vec3 Vi;
        Vi[0] = cube_verts[Vi_index][0];
        Vi[1] = cube_verts[Vi_index][1];
        Vi[2] = cube_verts[Vi_index][2];

        vec3 Vj;
        Vj[0] = cube_verts[Vj_index][0];
        Vj[1] = cube_verts[Vj_index][1];
        Vj[2] = cube_verts[Vj_index][2];

        float dl = 0.0;

        vec3 dV;
        glm_vec3_sub(Vj, Vi, dV);

        float dot_Vi_n = glm_vec3_dot(view_dir, Vi);
        float dot_dV_n = glm_vec3_dot(view_dir, dV);

        float t = (dl-dot_Vi_n)/dot_dV_n;

        if (t >= 0.0 && t <= 1.0)
        {
            intersections[i] = true;
            t_intersection[i] = t;
            num_intersections++;
            printf("between vertex %hhu and %hhu, t = %f\n", Vi_index, Vj_index, t);
        }
    }

    *vert_size = num_intersections * 6*sizeof(float);
    *vert_data = malloc(*vert_size);

    printf("num_intersections = %u\nvert_size = %u\n", num_intersections, *vert_size);

    uint32_t vertex_offset = 0;

    for (uint8_t i = 0; i < 12; i++)
    {
        if (!intersections[i]) continue;
        float t = t_intersection[i];

        vec3 intersection_pos;
        vec3 tex_coord;
        vec3 Vi;
        vec3 Vj;

        uint8_t Vi_index = cube_edges[i][0];
        uint8_t Vj_index = cube_edges[i][1];

        glm_vec3_copy(cube_verts[Vi_index], Vi);
        glm_vec3_copy(cube_verts[Vj_index], Vj);

        glm_vec3_lerp(Vi, Vj, t, intersection_pos);
        glm_vec3_copy(intersection_pos, tex_coord);
        glm_vec3_adds(tex_coord, 1.0, tex_coord);
        glm_vec3_divs(tex_coord, 2.0, tex_coord);

        glm_vec3_copy(intersection_pos, (*vert_data)+vertex_offset);
        vertex_offset += 3;
        glm_vec3_copy(tex_coord,        (*vert_data)+vertex_offset);
        vertex_offset += 3;
    }
}