#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

SDL_Window *win = nullptr;
SDL_GLContext ctx;

const char *vertex_source[] = { "#version 140\nin vec2 vertex_pos; void main() { gl_Position = vec4(vertex_pos.x, vertex_pos.y, 0, 1); }" };
const char *frag_source[] = { "#version 140\nout vec4 frag; void main() { frag = vec4( 1.0, 1.0, 1.0, 1.0 ); }" };

int
init_gl()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    win = SDL_CreateWindow(
        "MineClone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        600,
        400,
        SDL_WINDOW_OPENGL
    );
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    ctx = SDL_GL_CreateContext(win);
    assert(ctx != NULL);

    GLenum glew_ok = glewInit();
    assert(glew_ok == GLEW_OK);
    
    GLuint prog = glCreateProgram();

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

        return EXIT_FAILURE;
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

        return EXIT_FAILURE;
    }

    glAttachShader(prog, frag_shader);
    glLinkProgram(prog);

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    GLint link_ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
    if (link_ok != GL_TRUE) {
        std::fprintf(stderr, "couldn't link gpu program %d\n", frag_shader);

        char log[1024];
        glGetProgramInfoLog(prog, 1024, NULL, log);
        std::printf("error: %s\n", log);

        return EXIT_FAILURE;
    }

    glUseProgram(prog);

    return 0;
}

int
main(void)
{
    init_gl();

    glClearColor(0, 0, 0, 1);
    
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(win);
        
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}