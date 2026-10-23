#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define DEFAULT_WIDTH  600
#define DEFAULT_HEIGHT 400

SDL_Window *win = nullptr;
SDL_GLContext ctx;
GLuint prog = 0;
unsigned win_w = DEFAULT_WIDTH, win_h = DEFAULT_HEIGHT;

const char *vertex_source[] = {
R"(
#version 330 core

layout (location = 0) in vec2 vertex_pos;
layout (location = 1) in vec2 i_tex_c;

uniform mat4 model, view, proj;

out vec2 tex_c;

void main() {
    gl_Position = proj * view * model * vec4(vertex_pos.x, vertex_pos.y, 0, 1);
    tex_c = i_tex_c;
}
)"
};

const char *frag_source[] = {
R"(
#version 330 core

in vec2 tex_c;

out vec4 frag;

uniform sampler2D tex;

void main() {
    frag = texture(tex, tex_c);
}
)" };

const float square_vs[] = {
    -0.5, -0.5, 0.0, 0.0,
    -0.5,  0.5, 0.0, 1.0,
     0.5, -0.5, 1.0, 0.0,
     0.5,  0.5, 1.0, 1.0
};

const unsigned square_idx[] = {
    0, 1, 2,
    2, 3, 1
};

int
init_gl()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    win = SDL_CreateWindow(
        "MineClone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        win_w,
        win_h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    
    ctx = SDL_GL_CreateContext(win);
    assert(ctx != NULL);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    GLenum glew_ok = glewInit();
    assert(glew_ok == GLEW_OK);
    
    prog = glCreateProgram();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, vertex_source, NULL);
    glCompileShader(vertex_shader);

    GLint vshader_ok;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vshader_ok);
    if (vshader_ok != GL_TRUE) {
        std::fprintf(stderr, "couldn't compile shader %d\n", vertex_shader);

        char shader_log[1024];
        glGetShaderInfoLog(vertex_shader, sizeof(shader_log), NULL, shader_log);
        std::printf("error: %s\n", shader_log);

        return -1;
    }

    glAttachShader(prog, vertex_shader);

    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(frag_shader, 1, frag_source, NULL);
    glCompileShader(frag_shader);

    GLint fshader_ok;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &fshader_ok);
    if (fshader_ok != GL_TRUE) {
        std::fprintf(stderr, "couldn't compile shader %d\n", frag_shader);

        char shader_log[1024];
        glGetShaderInfoLog(frag_shader, sizeof(shader_log), NULL, shader_log);
        std::printf("error: %s\n", shader_log);

        return -1;
    }

    glAttachShader(prog, frag_shader);
    glLinkProgram(prog);

    GLint link_ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
    if (link_ok != GL_TRUE) {
        std::fprintf(stderr, "couldn't link gpu program %d\n", frag_shader);

        char log[1024];
        glGetProgramInfoLog(prog, 1024, NULL, log);
        std::printf("error: %s\n", log);

        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    glUseProgram(prog);

    glUniform1i(glGetUniformLocation(prog, "tex"), 0);

    return 0;
}

int
main(void)
{
    std::unordered_map<SDL_Keycode, bool> keyboard;

    if (init_gl() < 0) return EXIT_FAILURE;
    
    /* create square */
    GLuint vao, vbo, ebo, tex;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vs), square_vs, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_idx), square_idx, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void *)(2*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* create square texture */
    int x, y, n;
    unsigned char *tex_data = stbi_load("../res/grass.png", &x, &y, &n, 0);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_data);
    
    glm::mat4 model = glm::mat4(1.0); 
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    GLint model_loc = glGetUniformLocation(prog, "model");
    GLint view_loc = glGetUniformLocation(prog, "view");
    GLint proj_loc = glGetUniformLocation(prog, "proj");

    proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WIDTH/float(DEFAULT_HEIGHT), 0.1f, 100.0f);

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_dir = glm::vec3(0.0f, 0.0f, -1.0f);

    const float sensitivity = 0.1;
    const float camera_speed = 0.025;
    float yaw = -90.0, pitch = 0.0;
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Uint64 last_time = 0, curr_time = SDL_GetPerformanceCounter();
    float delta_time = 0;

    glClearColor(0.0, 0.0, 0.0, 1);
    
    bool running = true;
    while (running) {
        SDL_Event e;

        last_time = curr_time;
        curr_time = SDL_GetPerformanceCounter();
        delta_time = (curr_time-last_time)*1000/SDL_GetPerformanceFrequency();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
                int new_w, new_h;
                SDL_GetWindowSize(win, &new_w, &new_h);
                glViewport(0, 0, new_w, new_h);
            }

            if (e.type == SDL_MOUSEMOTION) {
                glm::vec3 look_dir;

                yaw   += e.motion.xrel*sensitivity;
                pitch -= e.motion.yrel*sensitivity;
                
                if (pitch> 89.0) pitch =  89.0;
                if (pitch<-89.0) pitch = -89.0;
                
                look_dir.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
                look_dir.y = sin(glm::radians(pitch));
                look_dir.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
                camera_dir = glm::normalize(look_dir);
            }

            if (e.type == SDL_KEYDOWN) {
                keyboard[e.key.keysym.sym] = true;
            }

            if (e.type == SDL_KEYUP) {
                keyboard[e.key.keysym.sym] = false;
            }
        }

        /* keyboard handling*/
        if (keyboard[SDLK_w]) camera_pos += camera_dir * (camera_speed * delta_time);
        if (keyboard[SDLK_s]) camera_pos -= camera_dir * (camera_speed * delta_time);
        if (keyboard[SDLK_a])
            camera_pos -= glm::normalize(glm::cross(camera_dir, glm::vec3(0.0f, 1.0f, 0.0f)))*(camera_speed*delta_time);
        if (keyboard[SDLK_d])
            camera_pos += glm::normalize(glm::cross(camera_dir, glm::vec3(0.0f, 1.0f, 0.0f)))*(camera_speed*delta_time);

        if (keyboard[SDLK_q]) running = false;

        view = glm::lookAt(camera_pos, camera_pos+camera_dir, glm::vec3(0.0f, 1.0f, 0.0f));

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc,  1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(proj_loc,  1, GL_FALSE, glm::value_ptr(proj));
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindTexture(GL_TEXTURE_2D, tex);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(win);
        
        SDL_Delay(1);
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}