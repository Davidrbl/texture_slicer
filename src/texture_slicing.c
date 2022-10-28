#include <stdint.h>
#include "cglm/call.h"
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <string.h>

// #define SLICE_DEBUG_PRINT

#define MAX_VERTS_PER_SLICE 6

float cost_between_vertices
(
	float *a,
	float* b
){
	const float dx = a[0] - b[0];
	const float dy = a[1] - b[1];
	const float dz = a[2] - b[2];

	return sqrtf(dx*dx + dy*dy + dz*dz);
}

/* 
    tsp function partially from https://www.geeksforgeeks.org/travelling-salesman-problem-implementation-using-backtracking/
    but modified
*/ 

// Function to find the minimum weight Hamiltonian Cycle
void tsp(
	float graph[MAX_VERTS_PER_SLICE][MAX_VERTS_PER_SLICE], 
	bool* v, 
	int32_t currPos,
	int32_t n, 
	int32_t progress, 
	float cost, 
	float* ans,
	int32_t* route,
	int32_t* best_route
){
    // If last node is reached and it has a link
    // to the starting node i.e the source then
    // keep the minimum value out of the total cost
    // of traversal and "ans"
    // Finally return to check for more possible values
    if (progress == n && graph[currPos][0]) {
		if (cost + graph[currPos][0] < *ans)
		{
			// new quickest path!
			// set the best route to the current route
			for (int32_t i = 0; i < n; i++)
				best_route[i] = route[i];
			
        	*ans = cost + graph[currPos][0];
		}
			
        return;
    }
 
    // BACKTRACKING STEP
    // Loop to traverse the adjacency list
    // of currPos node and increasing the count
    // by 1 and cost by graph[currPos][i] value
    for (int32_t i = 0; i < n; i++) 
    {
		// if i haven't visited this yet, and i haven't chosen the begin
        if (v[i] == false && graph[currPos][i]) 
        {
 
            // Mark as visited
            v[i] = true;
			route[progress] = i;
			
            tsp(graph, v, i, n, progress + 1,
                cost + graph[currPos][i], ans, route, best_route);
 
            // Mark this node as unvisited

			route[progress] = -1;
            v[i] = false;
        }
    }
}

void shortest_path(vec3 vertices[MAX_VERTS_PER_SLICE], int n, int* best_route)
{
    float graph[MAX_VERTS_PER_SLICE][MAX_VERTS_PER_SLICE];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (i == j) graph[i][j] = 0.0f;
			else graph[i][j] = cost_between_vertices(vertices[i], vertices[j]);
		}
    // printf("a\n");
			
	}


 
    // Boolean array to check if a node
    // has been visited or not
    bool v[n];
    for (int i = 0; i < n; i++)
    {
        v[i] = false;
    }

	int route[n];
	for (int i = 0; i < n; i++)
    {
		route[i] = -1;
		best_route[i] = -1;
    }
 
    // Mark 0th node as visited
    v[0] = true;
	route[0] = 0;
    float ans = 9999999.9999;
 
    // Find the minimum weight Hamiltonian Cycle
    tsp(
		graph, 	// graph
		v,		// visited
		0,		// current pos 
		n, 		// the number of nodes
		1,		// how deep are we in the search? 1 bc we chose the 1st one
		0, 		// the cost so far
		&ans,	// where to put the final answer
		route,
		best_route
	);
}
 

void slice(
    vec3 view_dir,
    float dl,
    float* vert_data,
    uint32_t* vert_size,
    uint32_t vert_offset
)
{
    #ifdef SLICE_DEBUG_PRINT
    printf(
        "slice() called with:\n\tview_dir -> %f %f %f\n\tdl -> %f\n", 
        view_dir[0], view_dir[1], view_dir[2],
        dl     
    );
    #endif

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

    uint8_t intersection_edges[MAX_VERTS_PER_SLICE][2];
    float t_intersection[MAX_VERTS_PER_SLICE] = { 0 };

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

        vec3 dV;
        glm_vec3_sub(Vj, Vi, dV);

        float dot_Vi_n = glm_vec3_dot(view_dir, Vi);
        float dot_dV_n = glm_vec3_dot(view_dir, dV);

        float t = (dl-dot_Vi_n)/dot_dV_n;

        /* 
                dl - dot(n, Vi)
            t = ---------------
                 dot(n, Vj-Vi)
        */ 

        if (t >= 0.0 && t <= 1.0)
        {
            intersection_edges[num_intersections][0] = Vi_index;
            intersection_edges[num_intersections][1] = Vj_index;
            t_intersection[num_intersections] = t;
            num_intersections++;

#ifdef SLICE_DEBUG_PRINT
            printf("between vertex %hhu and %hhu, t = %f\n", Vi_index, Vj_index, t);
#endif
        }
    }

    *vert_size = num_intersections * 6*sizeof(float);
    // *vert_data = malloc(*vert_size);

#ifdef SLICE_DEBUG_PRINT
    printf("num_intersections = %u\nvert_size = %u\n", num_intersections, *vert_size);
#endif
    uint32_t vertex_offset = 0;

    vec3 positions[MAX_VERTS_PER_SLICE];
    int32_t shortest_route[MAX_VERTS_PER_SLICE];

    assert(num_intersections <= 6);

    for (uint32_t i = 0; i < num_intersections; i++)
    {
        uint32_t Vi_index = intersection_edges[i][0];
        uint32_t Vj_index = intersection_edges[i][1];
        float t =               t_intersection[i];

        vec3 Vi, Vj;
        glm_vec3_copy(cube_verts[Vi_index], Vi);
        glm_vec3_copy(cube_verts[Vj_index], Vj);


        vec3 intersection_pos;
        glm_vec3_lerp(Vi, Vj, t, intersection_pos);

        glm_vec3_copy(intersection_pos, positions[i]);
    }

    // printf("num_intersections -> %u\n", num_intersections);
    shortest_path(positions, num_intersections, shortest_route);

    for (uint32_t i = 0; i < num_intersections; i++)
    {
        uint32_t position_index = shortest_route[i]; 
        
        vec3 vertex_pos;
        glm_vec3_copy(positions[position_index], vertex_pos);

        vec3 vertex_tex_coord;
        glm_vec3_copy(vertex_pos, vertex_tex_coord);
        glm_vec3_adds(vertex_tex_coord, 1.0, vertex_tex_coord);
        glm_vec3_divs(vertex_tex_coord, 2.0, vertex_tex_coord);

        vert_data[vert_offset++] = vertex_pos[0];
        vert_data[vert_offset++] = vertex_pos[1];
        vert_data[vert_offset++] = vertex_pos[2];

        vert_data[vert_offset++] = vertex_tex_coord[0];
        vert_data[vert_offset++] = vertex_tex_coord[1];
        vert_data[vert_offset++] = vertex_tex_coord[2];

        // glm_vec3_copy(vertex_pos, vert_data+vertex_offset);
        // vertex_offset += 3;
        // glm_vec3_copy(vertex_tex_coord, vert_data+vertex_offset);
        // vertex_offset += 3;
    }
}

void gen_texture_slices(
    vec3 normal,
    uint32_t num_slices,
    float** vert_data,
    uint32_t* vert_size,
    uint32_t* slice_len
)
{
    // float max_dl = -sqrt(3); // this one is the furthest away from the camera
    // float min_dl = sqrt(3); // this one is the closest to the camera

    // // sqrt(3) because the furthest point on the cube from the center of the the cube

    // uint32_t vert_offset = 0;

    // for (uint32_t i = 0; i < num_slices; i++)
    // {
    //     float dl = 0.0;
    //     // if (num_slices > 1) 
    //     dl = glm_lerp(max_dl, min_dl, i/(float)(num_slices-1));
    //     printf("for slice %u dl -> %f\n", i, dl);
    //     // if num_slices == 1, dl == 0.0, otherwise it just goes from max_dl to min_dl

    //     uint32_t slice_size = 0;

    //     float* slice_vert_data = NULL;

    //     slice(
    //         normal,
    //         dl,
    //         &slice_vert_data,
    //         &slice_size,
    //         0
    //     );

    //     slice_len[i] = slice_size;

    //     realloc(*vert_data, vert_offset + slice_size);

    //     memcpy(
    //         *vert_data + vert_offset,       // dest
    //         slice_vert_data,                // from
    //         slice_size                      // size
    //     );

    //     free(slice_vert_data);

    //     vert_offset += slice_size;

    //     printf("slice %u: size -> %u\n", i, slice_size);
    // }

    *vert_data = malloc(num_slices * MAX_VERTS_PER_SLICE * (6*sizeof(float))); // number of slices * max ammount of verts per slice * size of vert
    *vert_size = num_slices * MAX_VERTS_PER_SLICE * 6 * sizeof(float);

    float max_dl = -sqrt(3); // this one is the furthest away from the camera
    float min_dl = sqrt(3); // this one is the closest to the camera
    
    for (uint32_t i = 0; i < num_slices; i++)
    {
        uint32_t vertex_offset = i * MAX_VERTS_PER_SLICE * 6;

        float dl = glm_lerp(max_dl, min_dl, i/(float)num_slices);

        uint32_t slice_size = 0;

        slice(
            normal,
            dl,
            *vert_data,
            &slice_size,
            vertex_offset
        );

        slice_len[i] = slice_size;
        // *vert_size += slice_size;
    }

}
