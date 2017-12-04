#include <aiv_opengl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char *_read_whole_file(char *filename)
{
    FILE *f = NULL;
#ifdef _WIN32
    if (fopen_s(&f, filename, "rb"))
        return NULL;
#else
    f = fopen(filename, "rb");
    if (!f)
        return NULL;
#endif
    
    if (fseek(f, 0, SEEK_END))
    {
        fclose(f);
        return NULL;
    }

    long size = ftell(f);
    if (size < 0)
    {
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_SET))
    {
        fclose(f);
        return NULL;
    }

    char *buf = malloc(size + 1);
    if (!buf)
    {
        fclose(f);
        return NULL;
    }

    if (fread(buf, 1, size, f) != size)
    {
        free(buf);
        fclose(f);
        return NULL;
    }

    buf[size] = 0;

    fclose(f);
    return buf;
}

static GLuint _compile_shader(char *filename, GLenum shader_type)
{
    char *source = _read_whole_file(filename);
    if (!source)
        return 0;
   
    GLuint shader = glCreateShader(shader_type);
    if (!shader)
    {
        free(source);
        return 0;
    }

    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar info_log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
      glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
      fprintf(stderr, "%s\n", info_log);
      glDeleteShader(shader);
      free(source);
      return 0;
    }

    return shader;
}

GLuint aiv_compile_shader(char *vertex_filename, char *fragment_filename)
{

    GLuint vertex = _compile_shader(vertex_filename, GL_VERTEX_SHADER);
    if (!vertex)
    {
        fprintf(stderr, "unable to compile vertex shader\n");
        return 0;
    }

    GLuint fragment = _compile_shader(fragment_filename, GL_FRAGMENT_SHADER);
    if (!fragment)
    {
        fprintf(stderr, "unable to compile fragment shader\n");
        glDeleteShader(vertex);
        return 0;
    }

    GLuint program = glCreateProgram();
    if (!program)
    {
        fprintf(stderr, "unable to create program\n");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
  
    GLint success;
    GLchar info_log[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "%s\n", info_log);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(program);
        return 0;
    }

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}