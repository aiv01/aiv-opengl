#define SDL_MAIN_HANDLED 1
#include <SDL.h>

#include <aiv_opengl.h>

int aiv_opengl_context_init(aiv_opengl_context_t *context)
{
    if (SDL_Init(SDL_INIT_VIDEO))
        return -1;

    unsigned int flags = SDL_WINDOW_OPENGL;

    if (context->full_screen)
        flags |= SDL_WINDOW_FULLSCREEN;

    if (!context->major)
        context->major = 3;

    if (!context->minor)
        context->minor = 3;

    if (!context->depth)
        context->depth = 24;

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context->major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context->minor);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context->depth);

    SDL_Window *window = SDL_CreateWindow(context->title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            context->width,
            context->height,
            flags);

    if (!window)
    {
        SDL_Quit();
        return -1;
    }

    context->window = window;

    context->gl_context = SDL_GL_CreateContext(window);
    if (!context->gl_context)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    return 0;
}

void aiv_opengl_context_swap(aiv_opengl_context_t *context)
{
    SDL_Event event;
    SDL_GL_SwapWindow(context->window);
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            context->closed = 1;
        }
    }  
}

void aiv_opengl_context_destroy(aiv_opengl_context_t *context)
{
    SDL_GL_DeleteContext(context->gl_context);  
    SDL_DestroyWindow(context->window);
    SDL_Quit();
}
