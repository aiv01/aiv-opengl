#include <glad/glad.h>
#define _USE_MATH_DEFINES
#include <math.h>

typedef struct aiv_opengl_context
{
    unsigned int width;
    unsigned int height;
    int major;
    int minor;
    char *title;
    int full_screen;
    int depth;
    int closed;
    void *window;
    void *gl_context;
} aiv_opengl_context_t;

int aiv_opengl_context_init(aiv_opengl_context_t *);
void aiv_opengl_context_swap(aiv_opengl_context_t *);
void aiv_opengl_context_set(aiv_opengl_context_t *);
void aiv_opengl_context_destroy(aiv_opengl_context_t *);