#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "gl.h"
#include "glfw3.h"
#include "shader.h"

#include "bmp_imp.h"
#include "texture_slicing.h"

typedef GLuint Buffer_t;
typedef GLuint Program_t;
typedef GLuint Texture_t;
typedef GLuint VAO_t;

void panic(uint32_t err, char* error)
{
    printf("%s", error);
    exit(err);
}

void GLAPIENTRY gl_debug_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    int32_t length,
    const GLchar* message,
    const void* userParam
){
    // GL_DEBUG_SEVERITY_HIGH;
    // GL_DEBUG_SEVERITY_MEDIUM;
    // GL_DEBUG_SEVERITY_LOW;

    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
        fprintf(stderr, "GL_ERROR \nsource: %x \ntype: %x \nid: %u \nseverity: %x \nlength: %d \nmessage: %s\nuserParam: %p\n", 
            source, type, id, severity, length, message, userParam);
}

int main()
{
    if (!glfwInit()) panic(1, "GLFW init fail!\n");

    uint32_t window_width = 1024;
    uint32_t window_height = 1024;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(
        window_width, 
        window_height, 
        "Window title", 
        NULL, 
        NULL
    );

    if (!window) panic(1, "Window create fail!\n");

    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) panic(1, "GL load fail\n");

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_debug_callback, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    uint32_t main_program;
    create_program(
        "src/shaders/main.vert",
        NULL,
        NULL,
        NULL,
        "src/shaders/main.frag",
        &main_program
    );

    // uint32_t test_compute_program;
    // create_program_compute(
    //     "src/shaders/test.comp",
    //     &test_compute_program
    // );

    // uint32_t test_counter = 0;

    // uint32_t test_counter_buffer;
    // glCreateBuffers(1, &test_counter_buffer);
    // glNamedBufferData(test_counter_buffer, sizeof(uint32_t), &test_counter, GL_DYNAMIC_READ);
    // glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, test_counter_buffer);
    // // glBindBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, test_counter_buffer, 0, sizeof(uint32_t));

    // glUseProgram(test_compute_program);
    // glDispatchCompute(10, 10, 10);
    // glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // uint32_t* result = glMapNamedBuffer(test_counter_buffer, GL_READ_ONLY);

    // printf("counter -> %u\n", *result);

    // glUnmapNamedBuffer(test_counter_buffer);

    const char* addresses[] = {
        "ct/I36.bmp",
        "ct/I37.bmp",
        "ct/I38.bmp",
        "ct/I39.bmp",
        "ct/I40.bmp",
        "ct/I41.bmp",
        "ct/I42.bmp",
        "ct/I43.bmp",
        "ct/I44.bmp",
        "ct/I45.bmp",
        "ct/I46.bmp",
        "ct/I47.bmp",
        "ct/I48.bmp",
        "ct/I49.bmp",
        "ct/I50.bmp",
        "ct/I51.bmp",
        "ct/I52.bmp",
        "ct/I53.bmp",
        "ct/I54.bmp",
        "ct/I55.bmp",
        "ct/I56.bmp",
        "ct/I57.bmp",
        "ct/I58.bmp",
        "ct/I59.bmp",
        "ct/I60.bmp",
        "ct/I61.bmp",
        "ct/I62.bmp",
        "ct/I63.bmp",
        "ct/I64.bmp",
        "ct/I65.bmp",
        "ct/I66.bmp",
        "ct/I67.bmp",
        "ct/I68.bmp",
        "ct/I69.bmp",
        "ct/I70.bmp",
        "ct/I71.bmp",
        "ct/I72.bmp",
        "ct/I73.bmp",
        "ct/I74.bmp",
        "ct/I75.bmp",
        "ct/I76.bmp",
        "ct/I77.bmp",
        "ct/I78.bmp",
        "ct/I79.bmp",
        "ct/I80.bmp",
        "ct/I81.bmp",
        "ct/I82.bmp",
        "ct/I83.bmp",
        "ct/I84.bmp",
        "ct/I85.bmp",
        "ct/I86.bmp",
        "ct/I87.bmp",
        "ct/I88.bmp",
        "ct/I89.bmp",
        "ct/I90.bmp",
        "ct/I91.bmp",
        "ct/I92.bmp",
        "ct/I93.bmp",
        "ct/I94.bmp",
        "ct/I95.bmp",
        "ct/I96.bmp",
        "ct/I97.bmp",
        "ct/I98.bmp",
        "ct/I99.bmp",
        "ct/I100.bmp",
        "ct/I101.bmp",
        "ct/I102.bmp"
    };

    uint32_t num_addresses = sizeof(addresses)/sizeof(char*);

    uint8_t* bitmap_data = NULL;
    uint32_t bitmap_res = 0;
    bmp_load(addresses, num_addresses, &bitmap_data, &bitmap_res);
    printf("Loaded bmp succesfully!\nres = %u\n", bitmap_res);

    Texture_t bmp_tex;
    glCreateTextures(GL_TEXTURE_3D, 1, &bmp_tex);
    glTextureStorage3D(bmp_tex, 1, GL_R8, bitmap_res, bitmap_res, num_addresses);
    glTextureSubImage3D(
        bmp_tex, 0,
        0, 0, 0,
        bitmap_res, bitmap_res, num_addresses,
        GL_RED, GL_UNSIGNED_BYTE,
        bitmap_data
    );

    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(bmp_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTextureUnit(0, bmp_tex);

    uint32_t num_slices = 400;
    uint32_t* slice_len = malloc(num_slices*sizeof(uint32_t));

    vec3 view_dir = {
        0.0, 0.0, 1.0
    };

    float* vert_data = NULL;
    uint32_t vert_size = 0;

    vert_data = malloc(num_slices * MAX_VERTS_PER_SLICE * (6*sizeof(float))); // number of slices * max ammount of verts per slice * size of vert
    vert_size = num_slices * MAX_VERTS_PER_SLICE * 6 * sizeof(float);

    gen_texture_slices(
        view_dir,   // normal
        num_slices,             // num_slices 
        &vert_data,     // vert_data 
        &vert_size,     // vert_size
        slice_len       // slice_len 
    );

    // printf("%u\n", vert_size);
    // for (uint32_t i = 0; i < vert_size/6/sizeof(float); i++)
    // {
    //     printf("vertex %u:\n", i);
    //     printf("\tpos -> %f\n", vert_data[i*6 + 0]);
    //     printf("\tpos -> %f\n", vert_data[i*6 + 1]);
    //     printf("\tpos -> %f\n", vert_data[i*6 + 2]);

    //     printf("\ttex -> %f\n", vert_data[i*6 + 3]);
    //     printf("\ttex -> %f\n", vert_data[i*6 + 4]);
    //     printf("\ttex -> %f\n", vert_data[i*6 + 5]);
    // }

    Buffer_t main_VBO;
    glCreateBuffers(1, &main_VBO);
    glNamedBufferData(main_VBO, vert_size, vert_data, GL_DYNAMIC_DRAW);

    VAO_t main_VAO;
    glCreateVertexArrays(1, &main_VAO);
    glEnableVertexArrayAttrib(main_VAO, 0);
    glVertexArrayAttribBinding(main_VAO, 0, 0);
    glVertexArrayAttribFormat(main_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(main_VAO, 1);
    glVertexArrayAttribBinding(main_VAO, 1, 0);
    glVertexArrayAttribFormat(main_VAO, 1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float));

    glVertexArrayVertexBuffer(main_VAO, 0, main_VBO, 0, 6*sizeof(float));

    mat4 model_matrix =         GLM_MAT4_IDENTITY_INIT;
    mat4 view_matrix =          GLM_MAT4_IDENTITY_INIT;
    mat4 projection_matrix =    GLM_MAT4_IDENTITY_INIT;

    vec3 cam_pos = {0.0, 0.0, 5.0};
    vec3 cam_for = {0.0, 0.0, -1.0};
    vec2 cam_rot = GLM_VEC2_ZERO_INIT;

    glm_perspective(90.0, window_width/window_height, 0.001f, 100.00f, projection_matrix);

    double frame_begin_time = glfwGetTime();
    double dt = 0.0;


    while (!glfwWindowShouldClose(window))
    {
        frame_begin_time = glfwGetTime();

        glfwPollEvents();



        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);

        const float ROTATION_SPEED = 1.0;
        const float CAMERA_SPEED = 4.0;

        {
            cam_rot[0] += (glfwGetKey(window, GLFW_KEY_I) - glfwGetKey(window, GLFW_KEY_K)) * dt * ROTATION_SPEED;
            cam_rot[1] += (glfwGetKey(window, GLFW_KEY_J) - glfwGetKey(window, GLFW_KEY_L)) * dt * ROTATION_SPEED;

            if (cam_rot[0] < -0.49*GLM_PIf) cam_rot[0] = -0.49*GLM_PIf;
            if (cam_rot[0] >  0.49*GLM_PIf) cam_rot[0] =  0.49*GLM_PIf;

            float tmp = cosf(cam_rot[0]);

            cam_for[0] = sinf(-cam_rot[1]) * tmp;
            cam_for[1] = sinf(cam_rot[0]);
            cam_for[2] = -cosf(cam_rot[1]) * tmp;

            // W and S
            float mul = (float)(glfwGetKey(window, GLFW_KEY_S) - glfwGetKey(window, GLFW_KEY_W));

            cam_pos[0] += sinf(cam_rot[1]) * mul * CAMERA_SPEED * dt;
            cam_pos[2] += cosf(cam_rot[1]) * mul * CAMERA_SPEED * dt;

            // A and D
            mul = (float)(glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A));

            cam_pos[0] += cosf(cam_rot[1]) * mul * CAMERA_SPEED * dt;
            cam_pos[2] += -sinf(cam_rot[1]) * mul * CAMERA_SPEED * dt;

            // Space and shift
            mul = (float)(glfwGetKey(window, GLFW_KEY_SPACE) - glfwGetKey(window, GLFW_KEY_LEFT_SHIFT));
            cam_pos[1] += mul * CAMERA_SPEED * dt;
        }

        double time = glfwGetTime();

        // if (vert_data) free(vert_data);

        // vec3 view_dir = GLM_VEC3_ZERO_INIT;
        glm_vec3_zero(view_dir);
        glm_vec3_sub(cam_pos, view_dir, view_dir);
        glm_vec3_normalize(view_dir);

        // void gen_texture_slices(
        //     vec3 normal,
        //     uint32_t num_slices,
        //     float** vert_data,
        //     uint32_t vert_size,
        //     uint32_t* slice_len
        // )

        // gen_texture_slices(
        //     view_dir,   // normal
        //     num_slices,             // num_slices 
        //     &vert_data,     // vert_data 
        //     &vert_size,     // vert_size
        //     slice_len       // slice_len 
        // );

        gen_texture_slices(
            view_dir,   // normal
            num_slices,             // num_slices 
            &vert_data,     // vert_data 
            &vert_size,     // vert_size
            slice_len       // slice_len 
        );
        glNamedBufferData(main_VBO, vert_size, vert_data, GL_DYNAMIC_DRAW);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glUseProgram(main_program);

        glBindVertexArray(main_VAO);

        glm_look(cam_pos, cam_for, (vec3){0.0, 1.0, 0.0}, view_matrix);

        glUniform1i(glGetUniformLocation(main_program, "texture_ID"), 0);
        glUniform1f(glGetUniformLocation(main_program, "alpha"), 1/(float)num_slices);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "model"), 1, GL_FALSE, &model_matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "view"), 1, GL_FALSE, &view_matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "projection"), 1, GL_FALSE, &projection_matrix[0][0]);

        // TODO: (david) set the first vertex of the slice to the view_dir * dl, and then use that to create every triangle, maybe...

        // glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        for (uint32_t i = 0; i < num_slices; i++)
        {
            uint32_t vert_begin = MAX_VERTS_PER_SLICE*i;
            uint32_t vert_count = slice_len[i]/6/sizeof(float);

            // printf("drawing the %u'th slice with %u vertices\n", i, vert_count);

            glDrawArrays(GL_TRIANGLE_FAN, vert_begin, vert_count);
            // vert_begin += slice_len[i];
        }

        glfwSwapBuffers(window);

        dt = glfwGetTime() - frame_begin_time;
    }

    free(vert_data);
    free(bitmap_data);

    glfwDestroyWindow(window);
    glfwTerminate();
}
