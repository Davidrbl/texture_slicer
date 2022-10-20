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

    uint32_t main_program;
    create_program(
        "src/shaders/main.vert",
        NULL,
        NULL,
        NULL,
        "src/shaders/main.frag",
        &main_program
    );

    uint32_t texture_view_program;
    create_program(
        "src/shaders/texture.vert",
        NULL,
        NULL,
        NULL,
        "src/shaders/texture.frag",
        &texture_view_program
    );

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
    glTextureStorage3D(bmp_tex, 1, GL_R8, bitmap_res, bitmap_res, bitmap_res);
    glTextureSubImage3D(
        bmp_tex, 0,
        0, 0, 0,
        bitmap_res, bitmap_res, bitmap_res,
        GL_RED, GL_UNSIGNED_BYTE,
        bitmap_data
    );

    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTextureParameteri(bmp_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(bmp_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTextureUnit(0, bmp_tex);

    // float vertices[] = {
    //     -1.0, -1.0,
    //      1.0, -1.0,
    //     -1.0,  1.0,

    //      1.0,  1.0,
    //     -1.0,  1.0,
    //      1.0, -1.0
    // };

    // Buffer_t texture_VBO;
    // VAO_t texture_VAO;
    // glCreateBuffers(1, &texture_VBO);
    // glCreateVertexArrays(1, &texture_VAO);
    
    // glNamedBufferData(texture_VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glEnableVertexArrayAttrib(texture_VAO, 0);
    // glVertexArrayAttribBinding(texture_VAO, 0, 0);
    // glVertexArrayAttribFormat(texture_VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

    // glVertexArrayVertexBuffer(texture_VAO, 0, texture_VBO, 0, 2 * sizeof(float));

    vec3 view_dir = {
        1.0, 0.0, 0.0
    };

    float* vert_data = NULL;
    uint32_t vert_size = 0;

    gen_tex_slices(view_dir, 1, &vert_data, &vert_size, NULL, NULL);

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

    float depth = 0.0;

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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) glfwSetWindowShouldClose(window, 1);

        if (glfwGetKey(window, GLFW_KEY_UP)) depth += 0.01;
        if (glfwGetKey(window, GLFW_KEY_DOWN)) depth -= 0.01;

        double time = glfwGetTime();

        
        if (vert_data) free(vert_data);

        gen_tex_slices(
            (vec3){sinf(time), 0.0, cosf(time)},
            0, &vert_data, &vert_size, NULL, NULL
        );

        glNamedBufferData(main_VBO, vert_size, vert_data, GL_DYNAMIC_DRAW);

        glUseProgram(main_program);

        glBindVertexArray(main_VAO);



        glm_look(cam_pos, cam_for, (vec3){0.0, 1.0, 0.0}, view_matrix);

        glUniform1i(glGetUniformLocation(main_program, "texture_ID"), 0);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "model"), 1, GL_FALSE, &model_matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "view"), 1, GL_FALSE, &view_matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(main_program, "projection"), 1, GL_FALSE, &projection_matrix[0][0]);

        // TODO: (david) set the first vertex of the slice to the view_dir * dl, and then use that to create every triangle, maybe...
        glDrawArrays(GL_TRIANGLE_FAN, 0, vert_size/6/sizeof(float));

        // glUseProgram(texture_view_program);
        // glBindVertexArray(texture_VAO);
        // glUniform1f(glGetUniformLocation(texture_view_program, "depth"), depth);

        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);

        dt = glfwGetTime() - frame_begin_time;
    }

    free(vert_data);
    free(bitmap_data);

    glfwDestroyWindow(window);
    glfwTerminate();
}
