#include <aiv_opengl.h>
#include <stdio.h>

GLuint aiv_compile_shader(char *, char *);

aiv_opengl_context_t context;

int main(int argc, char *argv[])
{
    context.width = 800;
    context.height = 800;
    context.title = "OpenGL Example";
    context.major = 3;
    context.minor = 3;

    if (aiv_opengl_context_init(&context))
    {
        fprintf(stderr, "error while initializing aiv_opengl_context\n");
        return -1;
    }

    
    glViewport(0, 0, context.width, context.height);


    GLuint program = aiv_compile_shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    if (!program)
    {
        fprintf(stderr,"unable to build shader program\n");
        aiv_opengl_context_destroy(&context);
        return -1;
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    if (vao == 0)
    {
        fprintf(stderr,"unable to create VAO\n");
        aiv_opengl_context_destroy(&context);
        return -1;
    }

    fprintf(stdout, "VAO created with id %d\n", vao);

    glClearColor(0, 0, 0, 1);

    glUseProgram(program);
    glBindVertexArray(vao);


    while(!context.closed)
    {
        
        glClear(GL_COLOR_BUFFER_BIT);
      
        glDrawArrays(GL_TRIANGLES, 0, 3);

        aiv_opengl_context_swap(&context);
    }

    aiv_opengl_context_destroy(&context);

    return 0;
}